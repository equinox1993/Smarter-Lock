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

#include "CommandPacket.h"
#include "PasscodePacket.h"

uint32_t PacketAssembler::GetLength(const uint8_t* input, uint32_t maxLen) {
	if (maxLen < 12)
		return 0;
	
	uint32_t nplen;
	memcpy(&nplen, input+8, 4);
	
	uint32_t len = 12 + ntohl(nplen);
	
	if (len <= maxLen)
		return len;
	else
		return 0;
}

uint8_t* PacketAssembler::Assemble(const Packet* pl, int& totalLength) {
	uint32_t plen = pl->length();
	
    uint32_t nptype = htonl(pl->type());
	uint32_t npseq = htonl(pl->sequenceNumber);
	
	totalLength = 12+plen;
	
	uint8_t* packet = new uint8_t[totalLength];
	memcpy(packet, &nptype, 4);
	memcpy(packet+4, &npseq, 4);
	
//	if (plen > 0) {
	uint32_t nplen = htonl(plen);
	memcpy(packet+8, &nplen, 4);

	if (plen > 0) {
		uint8_t* pserialized = nullptr;

		pserialized = new uint8_t[plen];
		pl->serialize(pserialized);
		
		memcpy(packet+12, pserialized, plen);
		
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
	memcpy(&nplen, packetdata+8, 4);
	
	plen = ntohl(nplen);
	
	pldata = packetdata+12;
}

Packet* PacketAssembler::Disassemble(const uint8_t* input) {
	uint32_t nptype;
	uint32_t npseq;
	memcpy(&nptype, input, 4);
	memcpy(&npseq, input+4, 4);
	
	uint32_t ptype = ntohl(nptype);
	uint32_t pseq = ntohl(npseq);
	
	const uint8_t* pldata;
	int plen;
	GetPayloadData(input, pldata, plen);

	switch (ptype) {
		case Type::PASSCODE:
			return new PasscodePacket(pldata, pseq);
		default:
			return new CommandPacket(ptype, pseq);
	}
}