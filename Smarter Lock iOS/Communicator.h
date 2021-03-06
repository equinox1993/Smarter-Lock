//
//  Communicator.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "PacketAssembler.h"

#include <openssl/rsa.h>

#include <vector>
#include <mutex>

@interface Communicator : NSObject<NSStreamDelegate> {
    @private
    NSInputStream* istream;
    NSOutputStream* ostream;
    
//    NSString* host;
//    unsigned int port;
	
	NSMutableData* readbuf;
	
	NSMutableDictionary* targets;
	NSMutableDictionary* selectors;
	
	uint32_t curSeq;
	
	NSDate* lastSent;
	
	NSTimer* connKiller;
	
	Boolean connected;
	
	std::vector<Packet*> packetQueue;
	std::mutex packetQueueMutex;
}

@property (retain) NSData* token;
@property (retain) id tag;
@property (retain) NSString* host;
@property int port;
@property NSTimeInterval timeout;

+(Communicator*)defaultCommunicator;

+(void)setDefaultHost:(NSString*)h;
+(void)setDefaultPort:(unsigned int)port;
+(NSString*)defaultHost;
+(unsigned int)defaultPort;

+(void)setRSA:(RSA*)rsa;

-(id)init;
-(id)initWithHost:(NSString*)host port:(unsigned int)port;
//-(void)writeString:(NSString*)str;
//-(void)writePacket:(Packet*)pl;
//-(void)writeCString:(const u_int8_t*)str length:(size_t)len;

-(void)consumeQueue;
-(void)blockingWritePacket:(Packet&)pl target:(id)target withSelector:(SEL)sel;
// Communicator WILL take the ownership of pl
-(void)writePacket:(Packet*)pl target:(id)target withSelector:(SEL)sel;
// callback:
//	receivePacket:(NSValue)packetPtr;
//	*** DON'T store the pointer. Copy it if you want to store the packet.

@end


