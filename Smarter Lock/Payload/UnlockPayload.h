//
//  UnlockPayload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__UnlockPayload__
#define __Smarter_Lock__UnlockPayload__

#include "Payload.h"

/*
    PLEN = 0
 */

class UnlockPayload : public Payload {
	public:
	
	static Payload* DefaultPayload();
	
	virtual int serialize(uint8_t* output) const;
	virtual uint32_t length() const;
	virtual uint32_t type() const;
};

#endif /* defined(__Smarter_Lock__UnlockPayload__) */
