//
//  PasscodePayload.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "PasscodePayload.h"

@implementation PasscodePayload
-(const char*)serialize {
    return nil;
}

-(u_int32_t)length {
    return 0;
}

-(u_int32_t)type {
    return 1;
}

-(id)init {
    return self;
}

@end
