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

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

//#include <map>

#include "Packet.h"

typedef void (*TCPCallbackFunction)(Packet*, struct bufferevent*);

class TCPServer {
	public:
	static bool Run(uint16_t port);
	static bool IsRunning();
	static void RegisterCallback(uint32_t type, TCPCallbackFunction cb);
	static void SendPacket(Packet* packet, struct bufferevent* target);
//	static uint32_t threadCount();
	
//	virtual ~TCPServer();
	
	private:
//	static uint16_t _port;
	static bool running;
//	static std::vector<uint8_t> readbuf;
	
//	static std::map<uint32_t, TCPCallbackFunction> callbackMap;
//	static uint32_t _threadCount;
//	static uint32_t _maxThreadCount;
	
//	static pthread_t* _threadIds;
//	static pthread_mutex_t _globalLock;

	static void read_cb(struct bufferevent *bev, void *ctx);
	static void event_cb(struct bufferevent *bev, short events, void *ctx);
	static void accept_conn_cb(struct evconnlistener *listener,
    	evutil_socket_t fd, struct sockaddr *address, int socklen,
    	void *ctx);
};

#endif /* defined(__Smarter_Lock__TCPServer__) */
