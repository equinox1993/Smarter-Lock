//
//  OctetPacket.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/3.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__SimplePacket__
#define __Smarter_Lock__SimplePacket__

#include "Packet.h"

class SimplePacket : public Packet {
	public:
	
	uint32_t packetType;
	uint32_t payloadLength;
	uint8_t* payload;
	
	SimplePacket(const uint8_t* payload, uint32_t length, uint32_t type, uint32_t seqNum = 0);
	~SimplePacket();
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__SimplePacket__) */
