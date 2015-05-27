//
//  PasscodePayload2.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__PasscodePacket__
#define __Smarter_Lock__PasscodePacket__

#include "Packet.h"
#include <string>

/* 
 16 bytes (character, [0-9A-Z]{16}) of passcode + expire datetime (64 bit unix timestamp)
 
 total length = 16 + 8 = 24
 */

class PasscodePacket : public Packet {
	public:
    
    static PasscodePacket* ParsePacket(const uint8_t* serial, uint32_t seqNum = 0); // reads 24 bytes from serialized data, returned object must be DELETED
	
	std::string passcode; // to retain reference
	uint64_t date;
	
	PasscodePacket(const char* passcode, time_t date, uint32_t seqNum = 0);
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__PasscodePacket__) */
