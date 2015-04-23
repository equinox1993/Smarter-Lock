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
 
 
 ------------------------------------------------
 
Type:
    1   - UNLOCK
    2   - PASSCODE
 */

#import <Foundation/Foundation.h>

@protocol Payload

-(const char*)serialize;
-(u_int32_t)length;
-(u_int32_t)type;
@end