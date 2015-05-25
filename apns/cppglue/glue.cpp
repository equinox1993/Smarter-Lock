//
//  glue.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/24.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <cstring>
#include <string>

#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "glue.h"

#include "../../Common/Packets/PacketAssembler.h"
#include "../../Common/Packets/SimplePacket.h"

#include "../../Common/StringHex.inl"
#include "../../Common/RSAHelper.inl"

#define TOKEN_LEN 32
#define TOKEN_LEN_HEX 64

RSA* rsa = nullptr;
int ispub;

int Encrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	
	if (ispub)
		return RSA_public_encrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
	
	return RSA_private_encrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}
int Decrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	
	if (ispub)
		return RSA_public_decrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
	
	return RSA_private_decrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}

int setRSA(const char* filename, int pub) {
	ispub = pub;
	
	rsa = rsaFromFile(filename, pub);
	
	if (!rsa)
		return 0;
	
	PacketAssembler::SetEncryptor(Encrypt);
	PacketAssembler::SetDecryptor(Decrypt);
	
	return 1;
}

int getToken(const char* input, char* output) {
	Packet* p = PacketAssembler::Disassemble((const uint8_t*)input, true);
	
	if (!p)
		return 0;
	
	if (p->type() != Type::DEVICE_TOKEN) {
		delete p;
		return 0;
	}
	
	SimplePacket* sp = (SimplePacket*)p;
	
	if (sp->length() != TOKEN_LEN) {
		delete p;
		return 0;
	}
	
	std::string str = arrToHex(sp->payload, sp->length());
	
	memcpy(output, str.c_str(), TOKEN_LEN_HEX);
	
	return 1;
}

int getLength(const char* input, int maxLen) {
	return PacketAssembler::GetLength((const uint8_t*)input, maxLen);
}

int makePacket(const char* input, char* output) {
	uint8_t tokenBin[TOKEN_LEN];
	hexToArr(input, tokenBin);
	SimplePacket sp(tokenBin, TOKEN_LEN, Type::DEVICE_TOKEN);
	
	size_t totalLen;
	const uint8_t* ass = PacketAssembler::Assemble(&sp, totalLen, true);
	
	memcpy(output, ass, totalLen);
	
	delete [] ass;
	
	return totalLen;
}