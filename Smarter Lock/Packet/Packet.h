//
//  Payload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

/*
    Numbers are all big-endian
 
    +----------+
    |   TYPE   |  -> type of payload (4 bytes)
    +----------+
    |   PLEN   |  -> len of payload (4 bytes)
    +----------+
    | Payload  |  -> payload (length of PLEN)
    +----------+
 
    Total packet length = 4 + 4 + PLEN
 
 */

#ifndef __Smarter_Lock__Packet__
#define __Smarter_Lock__Packet__

#include <stdint.h>

class Packet {
	public:
	
	virtual int serialize(uint8_t* output) const = 0; // serialized payload, output must be allocated
	virtual uint32_t length() const = 0;
	virtual uint32_t type() const = 0;
	virtual ~Packet() {};
};

enum Type {
	ACCEPT = 1,
	REJECT = 2,
	
	UNLOCK = 8,
	REQUEST_PASSCODE = 9,
	REQUEST_LOG = 10,
	REQUEST_MONITOR = 11,
	
	PASSCODE = 16
};
// TODO: add case to PacketAssembler::Disassemble when adding new payload type.

#endif /* defined(__Smarter_Lock__Packet__) */
