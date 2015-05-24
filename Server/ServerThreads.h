//
//  ServerThreads.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__ServerThreads__
#define __Smarter_Lock__ServerThreads__

#include <stdint.h>
#include <opencv2/core/core.hpp>

struct clientinfo {
	uint32_t seqno;
	int sockfd;
	uint64_t vidkey;
};

class ServerThreads {
public:
	static const char* rsaFile;
	static uint16_t port;
	static uint32_t numThreads;
	static const char* gpioUnlock;
	static void* startServer(void* sth);
	static void broadcastVideoFrame(cv::Mat frame);
	static void unlockDoor();
	static bool unlockWithPasscode(const char* psc);
	static uint32_t countMonitors();
	static void cleanup();
};

#endif /* defined(__Smarter_Lock__ServerThreads__) */
