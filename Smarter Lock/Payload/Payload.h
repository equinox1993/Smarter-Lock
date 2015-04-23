//
//  Payload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

/*
    Numbers are all big-endian
 
    +---------+
    |  PLEN   |  -> len of payload (4 bytes)
    +---------+
    |  TYPE   |  -> type of payload (4 bytes)
    +---------+
    | Payload |  -> payload (length of PLEN)
    +---------+
 
    Total packet length = 4 + 4 + PLEN
 
    Payload is just Payload
 
 */

#ifndef __Smarter_Lock__Payload__
#define __Smarter_Lock__Payload__

#include <stdint.h>

class Payload {
	public:
	
	virtual int serialize(uint8_t* output) const = 0; // output must be allocated
	virtual uint32_t length() const = 0;
	virtual uint32_t type() const = 0;
};

enum Type {
	UNLOCK = 1,
	PASSCODE = 2
};
// TODO: add case to PacketAssembler::ConstructPayload when adding new payload type.

#endif /* defined(__Smarter_Lock__Payload__) */
