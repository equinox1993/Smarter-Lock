//
//  APNClient.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/25.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "APNClient.h"

#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include <functional>
#include <cstring>
#include <strings.h>

#include "../Common/RSAHelper.inl"
#include "../Common/Packets/SimplePacket.h"
#include "../Common/Packets/PacketAssembler.h"

#include "../Common/StringHex.inl"

#if defined(__APPLE__) || defined(__MACH__)
# ifndef MSG_NOSIGNAL
#   define MSG_NOSIGNAL 0
# endif
#endif

int encryptor(RSA* rsa, int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_public_encrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}

int decryptor(RSA* rsa, int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_public_decrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}

APNClient::APNClient() {}
APNClient::APNClient(const std::string& host, short port, RSA* rsa) : host(host), port(port), rsa(rsa) {}

APNClient::~APNClient() {
	if (_sockfd)
		close();
}

bool APNClient::connect() {
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	#ifdef SO_NOSIGPIPE
	int enb = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_NOSIGPIPE, &enb, sizeof(int)) < 0) {
    	perror("setsockopt(SO_NOSIGPIPE) failed");
		return false;
	}
	#endif
	
	int arg = fcntl(_sockfd, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(_sockfd, F_SETFL, arg);
	
	struct hostent *server = gethostbyname(host.c_str());
	
	if (!server) {
		fprintf(stderr, "APNClient Error: No such host.\n");
		return false;
	}
	
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	bcopy(server->h_addr_list[0], &address.sin_addr.s_addr, server->h_length);
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	
	int res = ::connect(_sockfd, (struct sockaddr *)&address, sizeof(address));
	
	if (res < 0) {
		
		if (errno == EINPROGRESS) {
			struct timeval tv;
			fd_set myset;
			tv.tv_sec = 15;
			tv.tv_usec = 0;
			FD_ZERO(&myset);
			FD_SET(_sockfd, &myset);
			
			if (select(_sockfd+1, NULL, &myset, NULL, &tv) > 0) {
				socklen_t lon = sizeof(int);
				int valopt;
				getsockopt(_sockfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
				if (valopt) {
					fprintf(stderr, "Error in connection() %d - %s\n", valopt, strerror(valopt));
					return false;
				}
			}
			else {
				fprintf(stderr, "Timeout or error()\n");
				return false;
			}
		}
		else {
			fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
			return false;
		}
	}

	return true;
}

void APNClient::close() {
	::close(_sockfd);
	
	_sockfd = 0;
}

bool APNClient::notify(const std::string& token) {
	uint8_t tokenBin[32];
	
	hexToArr(token, tokenBin);
	
	SimplePacket sp(tokenBin, 32, Type::DEVICE_TOKEN);
	
	size_t tl;
	auto enc = std::bind(encryptor, rsa, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	const uint8_t* pkbuf = PacketAssembler::Assemble(&sp, tl, enc, true);
	
	if (send(_sockfd, pkbuf, tl, MSG_NOSIGNAL) < 0) {
		perror("not sent");
		
		delete[] pkbuf;
		return false;
	}
	
	delete[] pkbuf;
	return true;
}

APNClient APNClient::DefaultClient;
