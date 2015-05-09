//
//  main.cpp
//  Test Server
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <iostream>

#include <pthread.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <map>

#include "../Common/CommandPacket.h"
#include "../Common/PasscodePacket.h"
#include "../Common/VideoFramePacket.h"
#include "../Common/SimplePacket.h"

#include <openssl/pem.h>
#include <openssl/rsa.h>

//#include "UDPCommunicator.h"
#include "../Common/TCPServer.h"

#define RSA_FILE "./private.pem"

RSA* rsaFromFile(const char* filename, bool pub) {
	FILE* fp = fopen(filename, "rb");
	
	if (!fp) {
		fprintf(stderr, "Unable to open RSA file.");
		return nullptr;
	}
	
	RSA* rsa = RSA_new(); //?!?!
	
	if (pub)
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	else
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	
	return rsa;
}

void randSeq(char* out) {
    char set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	size_t setLen = strlen(set);
	
    for (int i = 0; i < 16; i++) {
        int rind = rand() % setLen;
        out[i] = set[rind];
    }
}

// -- actions

void unlock(Packet* up, CommunicationTask* ct) {
	CommandPacket accept = CommandPacket(Type::ACCEPT);
	accept.sequenceNumber = up->sequenceNumber;
	
	TCPServer::SendPacket(&accept, ct->sockfd_, true);
	TCPServer::CloseConnection(ct->sockfd_);
}

void passcode(Packet* up, CommunicationTask* ct) {
	char seq[17];
	seq[16] = '\0';
	randSeq(seq);
	PasscodePacket pc = PasscodePacket(seq, 233333);
	pc.sequenceNumber = up->sequenceNumber;
	TCPServer::SendPacket(&pc, ct->sockfd_, true);
	TCPServer::CloseConnection(ct->sockfd_);
}

struct clientinfo {
	uint32_t seqno;
	int sockfd;
	uint64_t vidkey;
};

std::map<int, struct clientinfo> monitorMap = std::map<int, struct clientinfo>();

void startMonitor(Packet* up, CommunicationTask* ct) {
	int key = ct->addr_ + (up->sequenceNumber << 19);
	
	uint64_t vidKey =
  		(((uint64_t) rand() <<  0) & 0x00000000FFFFFFFFull) |
  		(((uint64_t) rand() << 32) & 0xFFFFFFFF00000000ull);
	
	SimplePacket sp = SimplePacket((uint8_t*)&vidKey, 8, Type::VIDEO_KEY, up->sequenceNumber);
	
	printf("Start streaming for fd %d, seqNum %d, video key %d.\n", ct->sockfd_, up->sequenceNumber, vidKey);
	
	TCPServer::SendPacket(&sp, ct->sockfd_, true);
	
	monitorMap[key] = {up->sequenceNumber, ct->sockfd_, vidKey};
}


void stopMonitor(Packet* up, CommunicationTask* ct) {
	CommandPacket accept = CommandPacket(Type::ACCEPT);
	accept.sequenceNumber = up->sequenceNumber;
	
	TCPServer::SendPacket(&accept, ct->sockfd_, true);
	
	int key = ct->addr_ + (up->sequenceNumber << 19);
	struct clientinfo ci = monitorMap[key];
	
	printf("Stop streaming for fd %d.\n", ci.sockfd);
	TCPServer::CloseConnection(ci.sockfd);

	monitorMap.erase(key);
}

void* startServer(void* sth) {
	RSA* rsa = rsaFromFile(RSA_FILE, false);

	TCPServer::RegisterCallback(Type::UNLOCK, unlock);
	TCPServer::RegisterCallback(Type::REQUEST_PASSCODE, passcode);
	TCPServer::RegisterCallback(Type::REQUEST_MONITOR, startMonitor);
	TCPServer::RegisterCallback(Type::STOP_MONITOR, stopMonitor);
	
	while (!TCPServer::Run(2333, 10, rsa)) {
		fprintf(stderr, "Failed to start server. wait 5 secs then retry.\n");
		sleep(5);
	}
	return nullptr;
}

// -- end actions


int main(int argc, const char * argv[]) {
	pthread_t serverThreadId;
	pthread_create(&serverThreadId, nullptr, startServer, nullptr);
	
	cv::VideoCapture cap;
	cap.open(0);
	
	if (!cap.isOpened()) {
		std::cerr << "***Could not initialize capturing...***\n";
		std::cerr << "Current parameter's value: \n";
		return -1;
	}
	
	cv::Mat frame;
	
	while(1){
		cap >> frame;
		if(frame.empty()){
			std::cerr<<"frame is empty"<<std::endl;
			break;
		}
		
		if (frame.cols > 400) {
			float scale = 400.0 / frame.cols;
			int newrows = (int)(scale*frame.rows);
			cv::Mat small;
			cv::resize(frame, small, cvSize(400, newrows));
			
			frame = small;
		}
		
//		idk(frame);
		
		if (TCPServer::IsRunning()) {
			std::vector<uchar> outbuf = std::vector<uchar>();
			cv::imencode(".jpg", frame, outbuf);
			
			
			for (auto i = monitorMap.begin(); i != monitorMap.end();) {
				struct clientinfo ci = i->second;
				
				VideoFramePacket vfp = VideoFramePacket(outbuf.data(), outbuf.size());
				vfp.sequenceNumber = ci.seqno;
				vfp.crypt(ci.vidkey);
				if (!TCPServer::SendPacket(&vfp, ci.sockfd)) {
					monitorMap.erase(i++);
					TCPServer::CloseConnection(ci.sockfd);
				} else
					++i;
				
			}
			outbuf.clear();
			
		}
		
		#ifndef NO_GUI
		cv::imshow("", frame);
		#endif
		cv::waitKey(75);
		
	}
	

	
    return 1;
}
