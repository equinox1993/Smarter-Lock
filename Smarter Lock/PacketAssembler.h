//
//  PacketAssembler.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//
// Assembles packets from payloads / disassembles packets into payloads

#ifndef __Smarter_Lock__PacketAssembler__
#define __Smarter_Lock__PacketAssembler__

#include "Payload.h"

class PacketAssembler {
	public:
	
	 // !!! return pointers need to be DELETED by yourself !!!
	
	static uint8_t* Assemble(const Payload* payload);
	static uint8_t* Assemble(const Payload* payload, int& totalLength);
	static Payload* Disassemble(const uint8_t* input);
	static Payload* ConstructPayload(const uint8_t* pldata, uint32_t plen, uint32_t ptype);
	
};

#endif /* defined(__Smarter_Lock__PacketAssembler__) */
