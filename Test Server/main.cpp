//
//  main.cpp
//  Test Server
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <iostream>

#include <pthread.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <map>

#include "CommandPacket.h"
#include "PasscodePacket.h"
#include "VideoFramePacket.h"

//#include "UDPCommunicator.h"
#include "TCPServer.h"

void idk(cv::Mat &mat) {
	uchar* data = mat.data;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			data[mat.step * i + j*3 + 0] = 0;
			data[mat.step * i + j*3 + 1] = 0;
		}
	}
}

void randSeq(char* out) {
    char set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	size_t setLen = strlen(set);
	
    for (int i = 0; i < 16; i++) {
        int rind = rand() % setLen;
        out[i] = set[rind];
    }
}

VideoFramePacket* vfp = nullptr;

void unlock(Packet* up, CommunicationTask* ct) {
	CommandPacket accept = CommandPacket(Type::ACCEPT);
	accept.sequenceNumber = up->sequenceNumber;
	
	TCPServer::SendPacket(&accept, ct->sockfd_);
	TCPServer::CloseConnection(ct->sockfd_);
}

void passcode(Packet* up, CommunicationTask* ct) {
	char seq[17];
	seq[16] = '\0';
	randSeq(seq);
	PasscodePacket pc = PasscodePacket(seq, 233333);
	pc.sequenceNumber = up->sequenceNumber;
	TCPServer::SendPacket(&pc, ct->sockfd_);
	TCPServer::CloseConnection(ct->sockfd_);
}

struct clientinfo {
	uint32_t seqno;
	int sockfd;
};

std::map<int, struct clientinfo> monitorMap = std::map<int, struct clientinfo>();

void startMonitor(Packet* up, CommunicationTask* ct) {
	int key = ct->addr_ + (up->sequenceNumber << 19);
	monitorMap[key] = {up->sequenceNumber, ct->sockfd_};
}


void stopMonitor(Packet* up, CommunicationTask* ct) {
	CommandPacket accept = CommandPacket(Type::ACCEPT);
	accept.sequenceNumber = up->sequenceNumber;
	
	TCPServer::SendPacket(&accept, ct->sockfd_);
	
	int key = ct->addr_ + (up->sequenceNumber << 19);
	struct clientinfo ci = monitorMap[key];
	monitorMap.erase(key);
	TCPServer::CloseConnection(ci.sockfd);
}

void* startServer(void* sth) {
	TCPServer::RegisterCallback(Type::UNLOCK, unlock);
	TCPServer::RegisterCallback(Type::REQUEST_PASSCODE, passcode);
	TCPServer::RegisterCallback(Type::REQUEST_MONITOR, startMonitor);
	TCPServer::RegisterCallback(Type::STOP_MONITOR, stopMonitor);
	
	TCPServer::Run(2333, 10);
	return nullptr;
}


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
		
		std::vector<uchar> outbuf = std::vector<uchar>();
		cv::imencode(".jpg", frame, outbuf);
		
		if (vfp)
			delete vfp;
		
		vfp = new VideoFramePacket(outbuf.data(), outbuf.size());
//		
		for (auto i = monitorMap.begin(); i != monitorMap.end();) {
			int sock = i->first;
			struct clientinfo ci = i->second;
			
			vfp->sequenceNumber = ci.seqno;
			if (!TCPServer::SendPacket(vfp, ci.sockfd)) {
				monitorMap.erase(i++);
				TCPServer::CloseConnection(ci.sockfd);
			} else
				++i;
			
		}
		
		cv::imshow("", frame);
		cv::waitKey(75);
		
		outbuf.clear();
	}
	

	
    return 1;
}
