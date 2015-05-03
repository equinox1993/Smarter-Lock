//
//  TCPServer.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/2.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <map>

#include <stdio.h>
#include <errno.h>

#include <arpa/inet.h>
//#include <sys/socket.h>
#include <stdlib.h>
#include <cstring>

#include "TCPServer.h"

#include "PacketAssembler.h"

//#define MAX_CHUNKS_COUNT 10

std::map<uint32_t, TCPCallbackFunction> callbackMap = std::map<uint32_t, TCPCallbackFunction>();

// ---- callbacks ----
 void TCPServer::read_cb(struct bufferevent *bev, void *ctx)
{
	// ASSUME: input short as only one segment
	struct evbuffer *input = bufferevent_get_input(bev);
	
	size_t buflen = evbuffer_get_length(input);
	
	uint8_t* bufview = new uint8_t[buflen];
	
	evbuffer_copyout(input, bufview, buflen);
	
//	readbuf.assign(chunkcp, chunkcp+buflen);
	
//	uint8_t *bufview = &readbuf.front();
	
	int32_t testlen = PacketAssembler::GetLength(bufview, buflen);
	
	if (testlen > 0) { // full packet
		Packet* packet = PacketAssembler::Disassemble(bufview);
		
		uint32_t type = packet->type();
		if (callbackMap.count(type) != 0) {
			 TCPCallbackFunction cb = callbackMap[type];
			
			 cb(packet, bev);
		}
		
		delete packet;
	} else if (testlen < 0) { // broken packet
		// ignore
	}
	
	
}

void TCPServer::event_cb(struct bufferevent *bev, short events, void *ctx)
{
        if (events & BEV_EVENT_ERROR) {
                perror("Error from bufferevent");
				bufferevent_free(bev);
		}
        if (events & BEV_EVENT_EOF) {
                bufferevent_free(bev);
        }
}

void TCPServer::accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen,
    void *ctx)
{
        /* We got a new connection! Set up a bufferevent for it. */
        struct event_base *base = evconnlistener_get_base(listener);
        struct bufferevent *bev = bufferevent_socket_new(
                base, fd, BEV_OPT_CLOSE_ON_FREE);

        bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);

        bufferevent_enable(bev, EV_READ|EV_WRITE);
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the listener. "
                "Shutting down.\n", err, evutil_socket_error_to_string(err));

        event_base_loopexit(base, NULL);
}

// ---- class defs ----


//TCPServer::TCPServer() : _port(pt), _maxThreadCount(mtc), _threadIds(nullptr) {}
//
//uint32_t TCPServer::threadCount() {
//	return isRunning();
//}

bool TCPServer::running = false;
//struct std::vector<uint8_t> TCPServer::readbuf = std::vector<uint8_t>();

void TCPServer::SendPacket(Packet* packet, struct bufferevent* target) {
	size_t tl;
	const uint8_t* pkbuf = PacketAssembler::Assemble(packet, tl);
	
	struct evbuffer* outbuf = bufferevent_get_output(target);
	evbuffer_add(outbuf, pkbuf, tl);
	
	delete[] pkbuf;
}

bool TCPServer::IsRunning() {
	return running;
}

void TCPServer::RegisterCallback(uint32_t type, TCPCallbackFunction cb) {
	callbackMap[type] = cb;
}

bool TCPServer::Run(uint16_t port) {
	if (IsRunning()) {
		fprintf(stderr, "Server already running");
		return false;
	}
	
//	_maxThreadCount = mtc;
	
	
//	_globalLock = PTHREAD_MUTEX_INITIALIZER;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Failed to create socket");
		return false;
	}
	
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	struct event_base* base = event_base_new();
	
	if (!base) {
		fprintf(stderr, "Couldn't open event base");
		return false;
	}
	
	struct evconnlistener* listener;
	
	listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
			(struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	if (!listener) {
		perror("Could'nt create listener");
		return false;
	}
	
	evconnlistener_set_error_cb(listener, accept_error_cb);
	
	event_base_dispatch(base);
	printf("Server running...");
	
	running = true;
	return true;

//
//	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//		perror("Failed to bind");
//		return false;
//	}
//	
//	if (listen(sockfd, 32) < 0) {
//		perror("Failed to listen");
//		return false;
//	}
//		
//	_threadIds = new pthread_t[_maxThreadCount];
//	bzero(_threadIds, _maxThreadCount * sizeof(pthread_t));
//	
//	
//	_threadCount = 0;
//	
//	while (1) {
//		pthread_mutex_lock(&_globalLock);
//		
//		if (_threadCount > _maxThreadCount) {
//			pthread_mutex_unlock(&_globalLock);
//			continue;
//		}
//		
//		pthread_mutex_unlock(&_globalLock);
//		
//		struct sockaddr_in* cli_addr = new struct sockaddr_in;
//		bzero(cli_addr, sizeof(struct sockaddr_in));
//		socklen_t clilen = sizeof(struct sockaddr_in);
//		accept(sockfd, (struct sockaddr*)cli_addr, &clilen);
//	}
}

//TCPServer::~TCPServer() {
//	if (!_threadIds)
//		return;
//	
//	for (int i = 0; i < _maxThreadCount; i++) {
//		if (_threadIds[i])
//			pthread_join(_threadIds[i], nullptr);
//	}
//	
//	delete _threadIds;
//}