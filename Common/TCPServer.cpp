//
//  TCPServer.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/2.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <unistd.h>
#include <map>

#include <stdio.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstring>

#include "TCPServer.h"
#include "Packets/PacketAssembler.h"

#if defined(__APPLE__) || defined(__MACH__)
# ifndef MSG_NOSIGNAL
#   define MSG_NOSIGNAL 0
# endif
#endif

#define MAX_READ_CHUNK_SIZE 1024

std::map<uint32_t, TCPCallbackFunction> callbackMap = std::map<uint32_t, TCPCallbackFunction>();
bool TCPServer::running = false;
ThreadPool* TCPServer::pool = nullptr;
RSA* TCPServer::rsa = nullptr;
int TCPServer::sockfd;

int TCPServer::Encrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_private_encrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}
int TCPServer::Decrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_private_decrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}

bool TCPServer::SendPacket(Packet* packet, int sock, bool crypt) {
	size_t tl;
	const uint8_t* pkbuf = PacketAssembler::Assemble(packet, tl, crypt);
	
	if (send(sock, pkbuf, tl, MSG_NOSIGNAL) < 0) {
		perror("not sent");
		
		delete[] pkbuf;
		return false;
	}
	
	delete[] pkbuf;
	return true;
}

bool TCPServer::IsRunning() {
	return running;
}

void TCPServer::RegisterCallback(uint32_t type, TCPCallbackFunction cb) {
	callbackMap[type] = cb;
}

void communicate(ThreadPool::Task* t) {
	CommunicationTask* ct = (CommunicationTask*)t;
	
	uint8_t readbuf[MAX_READ_CHUNK_SIZE];
	ssize_t n = recv(ct->sockfd_, readbuf, MAX_READ_CHUNK_SIZE, MSG_NOSIGNAL);
	
	int32_t testlen = PacketAssembler::GetLength(readbuf, n);
	
	if (testlen > 0) { // full packet
		Packet* packet = PacketAssembler::Disassemble(readbuf, true);
		
		if (!packet)
			return;
		
		uint32_t type = packet->type();
		if (callbackMap.count(type) != 0) {
			 TCPCallbackFunction cb = callbackMap[type];
			
			 cb(packet, ct);
		}
		
		delete packet;
	} else if (testlen < 0) { // broken packet
		// ignore
	}
}

bool TCPServer::Run(uint16_t port, uint32_t maxThreadCount, RSA* r) {
	printf("Server running on port %d, #threads=%d\n", port, maxThreadCount);
	if (IsRunning()) {
		fprintf(stderr, "Server already running");
		return false;
	}
	
	rsa = r;
	
	PacketAssembler::SetEncryptor(Encrypt);
	PacketAssembler::SetDecryptor(Decrypt);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Failed to create socket");
		return false;
	}
	
	
	#ifdef SO_NOSIGPIPE
	int enb = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &enb, sizeof(int)) < 0) {
    	perror("setsockopt(SO_NOSIGPIPE) failed");
		return false;
	}
	#endif
	
	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    	perror("setsockopt(SO_REUSEADDR) failed");
		return false;
	}
	
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Failed to bind");
		return false;
	}
	
	if (listen(sockfd, 16) < 0) {
		perror("Failed to listen");
		return false;
	}
	
	pool = new ThreadPool(maxThreadCount);
	running = true;
	
	while (1) {
		struct sockaddr_in* cli_addr = new struct sockaddr_in;
		bzero(cli_addr, sizeof(struct sockaddr_in));
		socklen_t clilen = sizeof(struct sockaddr_in);
		int csock = accept(sockfd, (struct sockaddr*)cli_addr, &clilen);
//		printf("%d\n", cli_addr->sin_addr.s_addr);
		CommunicationTask* task = new CommunicationTask(communicate, csock, cli_addr->sin_addr.s_addr, cli_addr->sin_port);
		
		pool->Dispatch(task);
	}
	
//	delete pool;
}

void TCPServer::Kill() {
	close(sockfd);
}

void TCPServer::CloseConnection(int sock) {
	close(sock);
}