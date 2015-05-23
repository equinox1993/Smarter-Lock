//
//  Helpers.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "Helpers.h"

#include <openssl/pem.h>
#include <cstring>


RSA* Helpers::rsaFromFile(const char* filename, bool pub) {
	FILE* fp = fopen(filename, "rb");
	
	if (!fp) {
		fprintf(stderr, "Unable to open RSA file.\n");
		return nullptr;
	}
	
	RSA* rsa = RSA_new(); //?!?!
	
	if (pub)
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	else
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	
	return rsa;
}

void Helpers::randSeq(char* out) {
    char set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	size_t setLen = strlen(set);
	
    for (int i = 0; i < 16; i++) {
        int rind = rand() % setLen;
        out[i] = set[rind];
    }
}

uint64_t Helpers::rand64() {
	return 
  		(((uint64_t) rand() <<  0) & 0x00000000FFFFFFFFull) |
  		(((uint64_t) rand() << 32) & 0xFFFFFFFF00000000ull);
}