//
//  ServerThreads.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "ServerThreads.h"

#include "Helpers.h"

#include "../Common/CommandPacket.h"
#include "../Common/PasscodePacket.h"
#include "../Common/VideoFramePacket.h"
#include "../Common/SimplePacket.h"

//#include "UDPCommunicator.h"
#include "../Common/TCPServer.h"

#include "../Common/GPIO.h"

#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include <map>
#include <set>

#include <opencv2/highgui/highgui.hpp>

std::map<int, struct clientinfo> monitorMap;
std::set<std::string> devices;

static GPIO* io;
static std::string curPasscode;

void ServerThreads::unlockDoor() {
	io->write(ServerThreads::gpioUnlock, true);
	sleep(1);
	io->write(ServerThreads::gpioUnlock, false);
}

bool ServerThreads::unlockWithPasscode(const char* psc) {
	if (curPasscode == psc) {
		unlockDoor();
		return true;
	}
	
	return false;
}

std::string arrToHex(const uint8_t* arr, int len) {
	std::ostringstream ost;
	ost << std::hex;
	for (int i = 0; i < len; i++) {
		uint8_t cur = arr[i];
		
		if (cur < 16)
			ost << 0;
		
		ost << (int)cur;
	}
	
	return ost.str();
}

inline int hexToInt(char hex) {
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	else if (hex >= 'a' && hex <= 'f')
		return hex-'a'+10;
	else
		return 0;
}

void hexToArr(std::string hex, uint8_t* arr) {
	int j = 0;
	for (int i = 0; i < hex.length(); i+=2) {
		int b1 = hexToInt(hex[i]);
		int b2 = hexToInt(hex[i+1]);
		
		int full = b1*16+b2;
		
		arr[j] = (uint8_t)full;
		j++;
	}
}

// actions

void unlock(Packet* up, CommunicationTask* ct) {
	CommandPacket accept = CommandPacket(Type::ACCEPT);
	accept.sequenceNumber = up->sequenceNumber;
	
	TCPServer::SendPacket(&accept, ct->sockfd_, true);
	TCPServer::CloseConnection(ct->sockfd_);
	
	ServerThreads::unlockDoor();
	
	printf("Door unlocked\n");
}

void passcode(Packet* up, CommunicationTask* ct) {
	char seq[17];
	seq[16] = '\0';
	Helpers::randSeq(seq);
	curPasscode = seq;
	PasscodePacket pc = PasscodePacket(seq, 233333);
	pc.sequenceNumber = up->sequenceNumber;
	TCPServer::SendPacket(&pc, ct->sockfd_, true);
	TCPServer::CloseConnection(ct->sockfd_);
}

void startMonitor(Packet* up, CommunicationTask* ct) {
	int key = ct->addr_ + (up->sequenceNumber << 19);
	
	uint64_t vidKey = Helpers::rand64();
	
	SimplePacket sp = SimplePacket((uint8_t*)&vidKey, 8, Type::VIDEO_KEY, up->sequenceNumber);
	
	printf("Start streaming for fd %d, seqNum %d, video key %llu.\n", ct->sockfd_, up->sequenceNumber, vidKey);
	
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

void newToken(Packet* up, CommunicationTask* ct) {
	SimplePacket* tp = (SimplePacket*)up;
	
	const uint8_t* token = tp->payload;
	
	std::string tokenHex = arrToHex(token, tp->length());
	devices.insert(tokenHex);
}

// end actions

void ServerThreads::broadcastVideoFrame(cv::Mat frame) {
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
}

uint32_t ServerThreads::countMonitors() {
	return monitorMap.size();
}

void* ServerThreads::startServer(void* sth) {
	RSA* rsa = Helpers::rsaFromFile(rsaFile, false);

	TCPServer::RegisterCallback(Type::UNLOCK, unlock);
	TCPServer::RegisterCallback(Type::REQUEST_PASSCODE, passcode);
	TCPServer::RegisterCallback(Type::REQUEST_MONITOR, startMonitor);
	TCPServer::RegisterCallback(Type::STOP_MONITOR, stopMonitor);
	TCPServer::RegisterCallback(Type::DEVICE_TOKEN, newToken);
	
	io = new GPIO();
	io->setup(ServerThreads::gpioUnlock, GPIO::Direction::OUT);
	
	while (!TCPServer::Run(port, numThreads, rsa)) {
		fprintf(stderr, "Failed to start server. wait 5 secs then retry.\n");
		sleep(5);
	}
	return nullptr;
}

void ServerThreads::cleanup() {		
	delete io;
	TCPServer::Kill();
}

uint16_t ServerThreads::port;
uint32_t ServerThreads::numThreads;
const char* ServerThreads::gpioUnlock;
const char* ServerThreads::rsaFile;