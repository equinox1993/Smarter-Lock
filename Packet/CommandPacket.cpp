//
//  UnlockPayload.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "CommandPacket.h"
#include <cstring>

int CommandPacket::serialize(uint8_t* output) const {
	return 0;
}

uint32_t CommandPacket::length() const {
    return 0;
}

uint32_t CommandPacket::type() const {
    return packetType;
}

CommandPacket::CommandPacket(uint32_t t, uint32_t seqNum) {
	packetType = t;
	sequenceNumber = seqNum;
}