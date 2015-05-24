//
//  OctetPacket.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/3.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "SimplePacket.h"
#include <cstring>

int SimplePacket::serialize(uint8_t* output) const {
	memcpy(output, payload, payloadLength);
	return payloadLength;
}

uint32_t SimplePacket::length() const {
    return payloadLength;
}

uint32_t SimplePacket::type() const {
    return packetType;
}

SimplePacket::SimplePacket(const uint8_t* pl, uint32_t l, uint32_t t, uint32_t seqNum)
	: payloadLength(l), packetType(t){
		sequenceNumber = seqNum;
		payload = new uint8_t[l];
		memcpy(payload, pl, l);
}

SimplePacket::~SimplePacket() {
	delete [] payload;
}