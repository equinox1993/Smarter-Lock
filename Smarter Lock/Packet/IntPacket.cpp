//
//  UnlockPayload.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "IntPacket.h"
#include <cstring>
#include <arpa/inet.h>

int IntPacket::serialize(uint8_t* output) const {
	uint32_t npl = htonl(payload);
	memcpy(output, &npl, 4);
	return 4;
}

uint32_t IntPacket::length() const {
    return 4;
}

uint32_t IntPacket::type() const {
    return packetType;
}

IntPacket::IntPacket(uint32_t t, uint32_t pl) {
	packetType = t;
	payload = pl;
}

IntPacket::IntPacket(uint32_t t, const uint8_t* pl) {
	packetType = t;
	uint32_t npl;
	memcpy(&npl, pl, 4);
	payload = ntohl(npl);
}