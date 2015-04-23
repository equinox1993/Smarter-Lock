//
//  UnlockPayload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__IntPacket__
#define __Smarter_Lock__IntPacket__

#include "Packet.h"

/*
    PLEN = 4, payload = an integer
 */

class IntPacket : public Packet {
	public:
	
	uint32_t packetType, payload;
	
	IntPacket(uint32_t type, uint32_t payloadNum);
	IntPacket(uint32_t type, const uint8_t* payload);
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__IntPacket__) */
