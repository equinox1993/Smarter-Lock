//
//  RSAHelper.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/24.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <openssl/pem.h>

#ifndef _RSA_HELPER_INL_
#define _RSA_HELPER_INL_

inline RSA* rsaFromFile(const char* filename, bool pub) {
	FILE* fp = fopen(filename, "rb");
	
	if (!fp) {
		fprintf(stderr, "Unable to open RSA file %s.\n", filename);
		return nullptr;
	}
	
	RSA* rsa = RSA_new(); //?!?!
	
	if (pub)
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	else
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	
	return rsa;
}

#endif