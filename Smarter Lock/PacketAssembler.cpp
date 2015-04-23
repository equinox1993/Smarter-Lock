//
//  PacketAssembler.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "PacketAssembler.h"
#include <arpa/inet.h>
#include <cstring>

#include "UnlockPayload.h"
#include "PasscodePayload.h"

uint8_t* PacketAssembler::Assemble(const Payload* pl, int& totalLength) {
	uint32_t plen = pl->length();
	
	uint32_t nplen = htonl(plen);
    uint32_t nptype = htonl(pl->type());
	
	totalLength = 4+4+plen;
	
	uint8_t* packet = new uint8_t[totalLength];
	memcpy(packet, &nplen, 4);
	memcpy(packet+4, &nptype, 4);
	
	if (plen > 0) {
		uint8_t* pserialized = nullptr;

		pserialized = new uint8_t[plen];
		pl->serialize(pserialized);
		
		memcpy(packet+8, pserialized, plen);
		
		delete pserialized;
	}
	
	return packet;
}

uint8_t* PacketAssembler::Assemble(const Payload* pl) {
	int tlen;
	
	return Assemble(pl, tlen);
}

Payload* PacketAssembler::ConstructPayload(const uint8_t* pldata, uint32_t plen, uint32_t ptype) {
	switch (ptype) {
		case Type::UNLOCK:
			return UnlockPayload::DefaultPayload();
		case Type::PASSCODE:
			return new PasscodePayload(pldata);
		
		default:
			return nullptr;
	}
}

Payload* PacketAssembler::Disassemble(const uint8_t* input) {
	uint32_t nplen;
	uint32_t nptype;
	
	memcpy(&nplen, input, 4);
	memcpy(&nptype, input+4, 4);
	
	uint32_t plen = ntohl(nplen);
	uint32_t ptype = ntohl(nptype);
	
	uint8_t* pldata = nullptr;
	if (plen > 0) {
		pldata = new uint8_t[plen];
		memcpy(pldata, input+8, plen);
	}
	
	Payload* pl = ConstructPayload(pldata, plen, ptype);
	
	if (pldata != nullptr) {
		delete pldata;
	}
	
	return pl;
}