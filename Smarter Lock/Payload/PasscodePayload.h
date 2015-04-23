//
//  PasscodePayload2.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__PasscodePayload__
#define __Smarter_Lock__PasscodePayload__

#include "Payload.h"
#include <string>

/* 
 16 bytes (character, [0-9A-Z]{16}) of passcode + expire datetime (64 bit unix timestamp)
 
 total length = 16 + 8 = 24
 */

class PasscodePayload : public Payload {
	public:
	
	std::string passcode; // to retain reference
	uint64_t date;
	
	PasscodePayload(const char* passcode, uint64_t date);
	PasscodePayload(const uint8_t* serial); // reads 24 bytes from serialized data
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__PasscodePayload__) */
