//
//  Communicator.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "Communicator.h"

@implementation Communicator
static Communicator* DefaultComm = nil;

+(id)defaultCommunicator {
    if (DefaultComm == nil)
        DefaultComm = [[Communicator alloc] initWithHost:@"localhost" port:2333];
    
    return DefaultComm;
}

-(id)initWithHost:(NSString*)h port:(int)p {
    host = h;
    port = p;
    
//    [ostream write:(const uint8_t *)[@"Hello World 233!\n" UTF8String] maxLength:128];
//    [ostream close];
    
    return self;
}

-(void)connect {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)(host), port, &readStream, &writeStream);
    
    NSInputStream *inputStream = (__bridge_transfer NSInputStream *)readStream;
    NSOutputStream *outputStream = (__bridge_transfer NSOutputStream *)writeStream;
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [inputStream open];
    [outputStream open];
    
    istream = inputStream;
    ostream = outputStream;
}

-(void)close {
    [ostream close];
    [istream close];
    
    ostream = nil;
    istream = nil;
}

-(void)writeCString:(const u_int8_t *)str length: (unsigned int)len {
    [self connect];
    [ostream write:str maxLength:len];
    [self close];
}

-(void)writeString:(NSString*)str {
    [self writeCString:(const u_int8_t*)[str UTF8String] length:[str length]];
}

-(void)writePayload:(Payload*)pl {
	int totalLen;
	const uint8_t* packetBytes = PacketAssembler::Assemble(pl, totalLen);
    [self writeCString: packetBytes length: totalLen];
	delete packetBytes;
}

@end
