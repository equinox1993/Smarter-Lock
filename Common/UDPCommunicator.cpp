//
//  UDPCommunicator.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "UDPCommunicator.h"
#include "PacketAssembler.h"

#define RECV_BUFSIZE 65000

#include <arpa/inet.h>
//#include <stdio.h>
#include <stdlib.h>
#include <cstring>

sockaddr_in UDPCommunicator::resolveAddress(const char* hostname, bool& err, short port) {
	struct sockaddr_in raddr;
	
	memset((char*)&raddr, 0, sizeof(struct sockaddr_in));
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(port);
	
	if (inet_aton(hostname, &raddr.sin_addr) == 0) {
//		fprintf(stderr, "inet_aton() failed\n");
		err = true;
		return raddr;
	}
	
	err = false;
	return raddr;
}

UDPCommunicator::UDPCommunicator(bool& err, short port) {
	myaddr = nullptr;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
//		perror("socket can't be created");
		err = true;
		return;
	}
	
	myaddr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	memset(myaddr, 0, sizeof(struct sockaddr_in));
	myaddr->sin_family = AF_INET;
	myaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr->sin_port = htons(port);
	
	if (bind(fd, (struct sockaddr*)myaddr, sizeof(struct sockaddr_in)) < 0) {
//		perror("bind failed");
		err = true;
		return;
	}
	
	err = false;
}

UDPCommunicator::~UDPCommunicator() {
	if (myaddr)
		delete myaddr;
}

bool UDPCommunicator::sendString(const struct sockaddr* destaddr, socklen_t destaddrlen, const char* data, int len) {
	return (sendto(fd, data, len, 0, destaddr, sizeof(struct sockaddr_in)) != -1);
}

void UDPCommunicator::receiveString(struct sockaddr* destaddr, socklen_t& destaddrlen, char* output, int bufsize, ssize_t& recvsize) {
	recvsize = recvfrom(fd, output, bufsize, 0, destaddr, &destaddrlen);
}

bool UDPCommunicator::sendPacket(const struct sockaddr* destaddr, socklen_t destaddrlen, const Packet& packet) {
	size_t len;
	const char* data = (const char*)PacketAssembler::Assemble(&packet, len);
	
	return this->sendString(destaddr, destaddrlen, data, len);
}

Packet* UDPCommunicator::receivePacket(struct sockaddr* destaddr, socklen_t& destaddrlen) {
//	return nullptr;
	char buf[RECV_BUFSIZE];
	ssize_t recvsize;
	this->receiveString(destaddr, destaddrlen, buf, RECV_BUFSIZE, recvsize);
	
	return PacketAssembler::Disassemble((const uint8_t*)buf);
}