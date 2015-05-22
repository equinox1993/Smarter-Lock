//
//  Helpers.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__Helpers__
#define __Smarter_Lock__Helpers__

#include <stdint.h>
#include <openssl/rsa.h>

class Helpers {
public:
	static RSA* rsaFromFile(const char* filename, bool pub);
	static void randSeq(char* out);
	static uint64_t rand64();
};

#endif /* defined(__Smarter_Lock__Helpers__) */
