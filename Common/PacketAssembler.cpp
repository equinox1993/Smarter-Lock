//
//  PacketAssembler.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "PacketAssembler.h"
#include <cstring>

#include "CommandPacket.h"
#include "PasscodePacket.h"
#include "VideoFramePacket.h"

uint32_t pad4(uint32_t in) {
	uint32_t roundDown = in / 4 * 4;
	
	if (in % 4 == 0)
		return roundDown;
	else return roundDown + 1;
}


int32_t PacketAssembler::GetLength(const uint8_t* input, size_t maxLen) {
	if (maxLen < PACKET_HEAD_LENGTH)
		return 0;
	
	if (maxLen >= 4) {
		uint32_t nsign;
		memcpy(&nsign, input+PACKET_OFFSET_SIGN, 4);
		
		if (nsign != PACKET_SIGNATURE)
			return -1;
	}
	
	uint32_t nplen;
	memcpy(&nplen, input+PACKET_OFFSET_PLEN, 4);
	
	uint32_t len = PACKET_HEAD_LENGTH + ntohl(nplen);
	
	if (len <= maxLen)
		return pad4(len);
	else
		return 0;
}

uint8_t* PacketAssembler::Assemble(const Packet* pl, size_t& totalLength) {
	uint32_t plen = pl->length();
	
    uint32_t nptype = htonl(pl->type());
	uint32_t npseq = htonl(pl->sequenceNumber);
	
	totalLength = pad4(PACKET_HEAD_LENGTH+plen);
	
	uint8_t* packet = new uint8_t[totalLength];
	memcpy(packet+PACKET_OFFSET_SIGN, &PACKET_SIGNATURE, 4);
	memcpy(packet+PACKET_OFFSET_TYPE, &nptype, 4);
	memcpy(packet+PACKET_OFFSET_SEQNUM, &npseq, 4);
	
//	if (plen > 0) {
	uint32_t nplen = htonl(plen);
	memcpy(packet+PACKET_OFFSET_PLEN, &nplen, 4);

	if (plen > 0) {
		uint8_t* pserialized = nullptr;

		pserialized = new uint8_t[plen];
		pl->serialize(pserialized);
		
		memcpy(packet+PACKET_OFFSET_PAYLOAD, pserialized, plen);
		
		delete[] pserialized;
	}
	
	return packet;
}

uint8_t* PacketAssembler::Assemble(const Packet* pl) {
	size_t tlen;
	
	return Assemble(pl, tlen);
}

// returns a viewing ptr of the payload
const uint8_t* GetPayloadData(const uint8_t* packetdata, int& plen) {
	uint32_t nplen;
	memcpy(&nplen, packetdata+PACKET_OFFSET_PLEN, 4);
	
	plen = ntohl(nplen);
	
	return packetdata + PACKET_OFFSET_PAYLOAD;
}

Packet* PacketAssembler::Disassemble(const uint8_t* input) {
	uint32_t npsign;
	uint32_t nptype;
	uint32_t npseq;
	memcpy(&npsign, input+PACKET_OFFSET_SIGN, 4);
	memcpy(&nptype, input+PACKET_OFFSET_TYPE, 4);
	memcpy(&npseq, input+PACKET_OFFSET_SEQNUM, 4);
	
	if (npsign != PACKET_SIGNATURE)
		return nullptr;
	
	uint32_t ptype = ntohl(nptype);
	uint32_t pseq = ntohl(npseq);
	
	int plen;
	const uint8_t* pldata = GetPayloadData(input, plen);

	switch (ptype) {
		case Type::PASSCODE:
            return PasscodePacket::ParsePacket(pldata, pseq);
		case Type::VIDEO_FRAME:
			return new VideoFramePacket(pldata, plen, pseq);
		default:
			return new CommandPacket(ptype, pseq);
	}
}