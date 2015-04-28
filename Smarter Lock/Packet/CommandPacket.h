//
//  UnlockPayload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__CommandPacket__
#define __Smarter_Lock__CommandPacket__

#include "Packet.h"

/*
    Command packet -> no payload / plen = 0
 */

class CommandPacket : public Packet {
	public:
	
	uint32_t packetType;
	
	CommandPacket(uint32_t type, uint32_t seqNum = 0);
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__CommandPacket__) */
