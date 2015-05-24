//
//  TCPServer.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/2.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__TCPServer__
#define __Smarter_Lock__TCPServer__

#include <pthread.h>
#include <stdint.h>

#include <openssl/rsa.h>

#include "ThreadPool.h"
#include "Packets/Packet.h"


class CommunicationTask : public ThreadPool::Task {
	public:
	
	CommunicationTask(ThreadPool::thread_task_fn fn, int sockfd, int addr, int port)
		: sockfd_(sockfd), addr_(addr), port_(port), ThreadPool::Task(fn) {}
	
	int sockfd_, addr_, port_;
};


typedef void (*TCPCallbackFunction)(Packet*, CommunicationTask*);  // don't store Communication Task ref

class TCPServer {
	public:
	static bool Run(uint16_t port, uint32_t maxThreadCount, RSA* rsa = nullptr); // rsa: private rsa
	static void Kill();
	static bool IsRunning();
	static void RegisterCallback(uint32_t type, TCPCallbackFunction cb);
	static bool SendPacket(Packet* packet, int sock, bool crypt = false);
	static void CloseConnection(int sock);
	
	private:
	static int sockfd;
	static RSA* rsa;
	static ThreadPool* pool;
	static bool running;
	
	static int Encrypt(int flen, const uint8_t* from, uint8_t* to);
	static int Decrypt(int flen, const uint8_t* from, uint8_t* to);
};

#endif /* defined(__Smarter_Lock__TCPServer__) */
