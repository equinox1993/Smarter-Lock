//
//  Communicator.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "PacketAssembler.h"

@interface Communicator : NSObject<NSStreamDelegate> {
    @private
    NSInputStream* istream;
    NSOutputStream* ostream;
    
    NSString* host;
    int port;
}

+(id)defaultCommunicator;
-(id)initWithHost:(NSString*)host port:(int)port;
-(void)writeString:(NSString*)str;
-(void)writePayload:(Payload*)pl;
-(void)writeCString:(const u_int8_t*)str length:(unsigned int)len;
@end