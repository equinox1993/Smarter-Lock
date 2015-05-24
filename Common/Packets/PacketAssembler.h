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

#define CRYPTO_OUTPUT_BUFFER_LEN 1024

// param: input length, input, output
// return: #bytes in output, -1 = fail
typedef int (*CryptoFn)(int, const uint8_t*, uint8_t*);

class PacketAssembler {
	public:
	
	static int32_t GetLength(const uint8_t* input, size_t maxLen);  // returns length of the TOTAL packet. 0 if packet not fully received, -1 if error occurs
	
	 // !!! returned pointers need to be DELETED by yourself !!!
	static uint8_t* Assemble(const Packet* packet, bool encrypt = false);
	static uint8_t* Assemble(const Packet* packet, size_t& totalLength, bool encrypt = false);
	static Packet* Disassemble(const uint8_t* input, bool needEncryption = false);
	
	static void SetEncryptor(CryptoFn enc);
	static void SetDecryptor(CryptoFn dec);
};

#endif /* defined(__Smarter_Lock__PacketAssembler__) */
