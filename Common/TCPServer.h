//
//  TCPServer.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/2.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

// dep: libevent

#ifndef __Smarter_Lock__TCPServer__
#define __Smarter_Lock__TCPServer__

#include <pthread.h>
#include <stdint.h>

#include "ThreadPool.h"
#include "Packet.h"


class CommunicationTask : public ThreadPool::Task {
	public:
	
	CommunicationTask(ThreadPool::thread_task_fn fn, int sockfd, int addr, int port)
		: sockfd_(sockfd), addr_(addr), port_(port), ThreadPool::Task(fn) {}
	
	int sockfd_, addr_, port_;
};


typedef void (*TCPCallbackFunction)(Packet*, CommunicationTask*);  // don't store Communication Task ref

class TCPServer {
	public:
	static bool Run(uint16_t port, uint32_t maxThreadCount);
	static bool IsRunning();
	static void RegisterCallback(uint32_t type, TCPCallbackFunction cb);
	static bool SendPacket(Packet* packet, int sock);
	static void CloseConnection(int sock);
	
	private:
	static ThreadPool* pool;
	static bool running;
};

#endif /* defined(__Smarter_Lock__TCPServer__) */
