//
//  UDPCommunicator.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__UDPCommunicator__
#define __Smarter_Lock__UDPCommunicator__

#include "Packets/Packet.h"
#include <sys/socket.h>

class UDPCommunicator {
	public:
	static struct sockaddr_in resolveAddress(const char* hostname, bool& err, short port = 0);
	
	UDPCommunicator(bool& err, short myport = 0);
	~UDPCommunicator();
	
	bool sendString(const struct sockaddr* destaddr, socklen_t destaddrlen, const char* data, int len); // returns success or not
	void receiveString(struct sockaddr* destaddr, socklen_t& destaddrlen, char* output, int bufsize, ssize_t& recvsize);
	
	bool sendPacket(const struct sockaddr* destaddr, socklen_t destaddrlen, const Packet& packet);
	Packet* receivePacket(struct sockaddr* destaddr, socklen_t& destaddrlen); // malloced out
	
	
	struct sockaddr_in *myaddr;
	
	private:
	int fd;
};

#endif /* defined(__Smarter_Lock__UDPCommunicator__) */
