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

#include "IntPacket.h"
#include "PasscodePacket.h"

uint8_t* PacketAssembler::Assemble(const Packet* pl, int& totalLength) {
	uint32_t plen = pl->length();
	
    uint32_t nptype = htonl(pl->type());
	
	totalLength = 4;
	
	uint8_t* packet = new uint8_t[totalLength];
	memcpy(packet, &nptype, 4);
	
	if (plen > 0) {
		totalLength += 4 + plen;
		uint32_t nplen = htonl(plen);
		memcpy(packet+4, &nplen, 4);

		uint8_t* pserialized = nullptr;

		pserialized = new uint8_t[plen];
		pl->serialize(pserialized);
		
		memcpy(packet+8, pserialized, plen);
		
		delete pserialized;
	}
	
	return packet;
}

uint8_t* PacketAssembler::Assemble(const Packet* pl) {
	int tlen;
	
	return Assemble(pl, tlen);
}

void GetPayloadData(const uint8_t* packetdata, const uint8_t*& pldata, int& plen) {
	uint32_t nplen;
	memcpy(&nplen, packetdata+4, 4);
	
	plen = ntohl(nplen);
	
	pldata = packetdata+8;
}

Packet* PacketAssembler::Disassemble(const uint8_t* input) {
	uint32_t nptype;
	memcpy(&nptype, input, 4);
	
	uint32_t ptype = ntohl(nptype);
	
	const uint8_t* pldata;
	int plen;
	GetPayloadData(input, pldata, plen);

	switch (ptype) {
		case Type::UNLOCK:
			return new IntPacket(Type::UNLOCK, pldata);
		case Type::PASSCODE:
			return new PasscodePacket(pldata);
		
		default:
			return nullptr;
	}
}