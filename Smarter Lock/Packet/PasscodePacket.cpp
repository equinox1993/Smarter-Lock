//
//  PasscodePayload.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "PasscodePacket.h"
#include <arpa/inet.h>
#include <cstring>

int PasscodePacket::serialize(uint8_t* output) const {
    uint64_t ndate = htonll(date);
	
	memcpy(output, passcode.c_str(), 16);
	memcpy(output+16, &ndate, 8);
	
	return 24;
}

uint32_t PasscodePacket::length() const {
    return 24;
}

uint32_t PasscodePacket::type() const {
    return Type::PASSCODE;
}

PasscodePacket::PasscodePacket(const char* psc, uint64_t dt) {
	passcode = psc;
	date = dt;
}

PasscodePacket::PasscodePacket(const uint8_t* serial) {
	char tpsc[17];
	uint64_t ndate;
	
	memcpy(tpsc, serial, 16);
	tpsc[16] = 0;
	
	memcpy(&ndate, serial+16, 8);
	
	passcode = tpsc;
	date = ntohll(ndate);
}