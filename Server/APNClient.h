//
//  APNClient.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/25.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__APNClient__
#define __Smarter_Lock__APNClient__

#include <string>
#include <arpa/inet.h>
#include <openssl/rsa.h>

class APNClient {
public:
	static APNClient DefaultClient;

	APNClient();
	APNClient(const std::string& host, short port, RSA* rsa);
	~APNClient();
	
	bool connect();
	void close();
	bool notify(const std::string& token);

	std::string host;
	short port;
	RSA* rsa;
private:
	int _sockfd;
};

#endif /* defined(__Smarter_Lock__APNClient__) */
