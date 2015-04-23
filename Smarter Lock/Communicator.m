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

-(void)writePayload:(id<Payload>)pl {
    u_int32_t plen = CFSwapInt32HostToBig([pl length]);
    u_int32_t ptype = CFSwapInt32HostToBig([pl type]);
    const u_int8_t* pserialized = (const u_int8_t*)[pl serialize];
    
    NSMutableData* packet = [NSMutableData dataWithCapacity:4+4+plen];
    [packet appendData: [NSData dataWithBytes:&plen length:4]];
    [packet appendData: [NSData dataWithBytes:&ptype length:4]];
    [packet appendData: [NSData dataWithBytes: pserialized length:plen]];
    
    const u_int8_t* packetBytes = (const u_int8_t*)[packet bytes];
    [self writeCString: packetBytes length: 4+4+plen];
}

@end
