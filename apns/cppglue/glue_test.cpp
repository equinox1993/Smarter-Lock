//
//  glue_test.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/24.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <cstring>
#include <string>
#include <iostream>

#include <stdio.h>
#include <assert.h>

#include "glue.h"
#include "../../Common/StringHex.inl"

void testLoadPrivateRSA() {
	printf("Start testLoadPrivateRSA\n");
	assert (setRSA("../../apnserver_private.pem", 0) == 1);
	printf("Finish testLoadPrivateRSA\n");
}

void testLoadPublicRSA() {
	printf("Start testLoadPublicRSA\n");
	assert (setRSA("../../../apnserver_public.pem", 1) == 1);
	printf("Finish testLoadPublicRSA\n");
}

void testMakePacket(char* buf) {
	testLoadPublicRSA();
	
	printf("Start testMakePacket\n");
	const char token[] = "f5ad0f90fd5a49f678a1fee89f0f87d31deeb22577759e8aeaf4a21d462d68af";
	
	int len = makePacket(token, buf);
	
	assert (len > 0);
	
	std::string hex = arrToHex((uint8_t*)buf, 128);
	
	std::cout<<hex<<std::endl;
	printf("Finish testMakePacket\n");
}

void testGetToken(const char* buf) {
	testLoadPrivateRSA();
	
	printf("Start testGetToken\n");
	
	char tok[65];
	
	assert (getToken(buf, tok) == 1);
	
	tok[64] = '\0';
	
	std::cout<<tok<<std::endl;
	
	assert (strcmp(tok, "f5ad0f90fd5a49f678a1fee89f0f87d31deeb22577759e8aeaf4a21d462d68af") == 0);
	
	printf("Finish testMakePacket\n");
}

int main(int argc, const char** argv) {
	char buf[256];
	testMakePacket(buf);
	testGetToken(buf);
	
	return 0;
}
