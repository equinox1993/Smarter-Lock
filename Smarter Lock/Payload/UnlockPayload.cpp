//
//  UnlockPayload.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "UnlockPayload.h"

static UnlockPayload* DefaultUnlockPayload = nullptr;

int UnlockPayload::serialize(uint8_t* output) const {
	return 0;
}

uint32_t UnlockPayload::length() const {
    return 0;
}

uint32_t UnlockPayload::type() const {
    return Type::UNLOCK;
}

Payload* UnlockPayload::DefaultPayload() {
    if (!DefaultUnlockPayload)
        DefaultUnlockPayload = new UnlockPayload();
    
    return DefaultUnlockPayload;
}
