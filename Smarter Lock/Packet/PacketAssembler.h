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

#include "Packet.h"

class PacketAssembler {
	public:
	
	static int32_t GetLength(const uint8_t* input, uint32_t maxLen);  // returns length of the TOTAL packet. 0 if packet not fully received, -1 if error occurs
	
	 // !!! returned pointers need to be DELETED by yourself !!!
	static uint8_t* Assemble(const Packet* packet);
	static uint8_t* Assemble(const Packet* packet, int& totalLength);
	static Packet* Disassemble(const uint8_t* input);
};

#endif /* defined(__Smarter_Lock__PacketAssembler__) */
