//
//  Communicator.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "Communicator.h"

#define MAX_LEN 1024
#define CONN_KILLER_THREAD_INTERVAL 10.0

@implementation Communicator

static NSString* DefaultHost = @"localhost";
static unsigned int DefaultPort = 2333;
static Communicator* DefaultComm = nil;
static NSTimeInterval DefaultTimeout = 60;

+(void)setDefaultHost:(NSString*)h {DefaultHost = h;}
+(void)setDefaultPort:(unsigned int)p {DefaultPort = p;}
+(NSString*)defaultHost {return DefaultHost;}
+(unsigned int)defaultPort {return DefaultPort;}

+(Communicator*)defaultCommunicator {
	if (DefaultComm)
		return DefaultComm;
	else {
		DefaultComm = [[Communicator alloc] init];
		return DefaultComm;
	}
}

-(id)init {
	return [self initWithHost:DefaultHost port:DefaultPort];
}

-(id)initWithHost:(NSString*)h port:(unsigned int)p {
    self.host = h;
    self.port = p;
	
	readbuf = nil;
	
	self.timeout = DefaultTimeout;
	curSeq = 1;
	delegates = [[NSMutableDictionary alloc] init];
	userInfos = [[NSMutableDictionary alloc] init];
	connected = false;
	
	[self touch];
	
	connKiller = [NSTimer scheduledTimerWithTimeInterval:CONN_KILLER_THREAD_INTERVAL target:self selector:@selector(connKillTimerThread:) userInfo:nil repeats:YES];
	
	[connKiller fire];
	
//    [ostream write:(const uint8_t *)[@"Hello World 233!\n" UTF8String] maxLength:128];
//    [ostream close];
    
    return self;
}

-(void)touch {
	lastSent = [NSDate date];
}

-(void)connect {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)(self.host), self.port, &readStream, &writeStream);
    
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
	
	connected = true;
}

-(void)close {
    [ostream close];
    [istream close];
	
	[ostream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[istream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    ostream = nil;
    istream = nil;
	
	[delegates removeAllObjects];
	[userInfos removeAllObjects];
	
	connected = false;
}

-(void)dealloc {
	[self close];
}

-(void)writeCString:(const u_int8_t *)str length: (unsigned int)len {
    [self connect];
    [ostream write:str maxLength:len];
	[self touch];
//    [self close];
}

//-(void)writeString:(NSString*)str {
//    [self writeCString:(const u_int8_t*)[str UTF8String] length:[str length]];
//}

-(void)writePacket:(Packet*)pl {
	int totalLen;
	const uint8_t* packetBytes = PacketAssembler::Assemble(pl, totalLen);
    [self writeCString: packetBytes length: totalLen];
	delete packetBytes;
}

-(void)writePacket:(Packet*)pl delegate:(id<CommunicatorDelegate>)del userInfo:(id)info {
	pl->sequenceNumber = curSeq;
	
	int totalLen;
	const uint8_t* packetBytes = PacketAssembler::Assemble(pl, totalLen);
	
	[self connect];
	
    [ostream write:packetBytes maxLength:totalLen];
	[delegates setObject:del forKey:[NSNumber numberWithInt:curSeq]];
	[userInfos setObject:info forKey:[NSNumber numberWithInt:curSeq]];
	curSeq++;
	[self touch];
	
	delete packetBytes;
}

-(void)connKillTimerThread:(id)info {
//	NSLog(@"Connection Killer Thread Runned.");
	
	NSDate* current = [NSDate date];
	NSTimeInterval elapse = [current timeIntervalSinceDate:lastSent];
	if (connected && elapse > self.timeout) {
		[self close];
		NSLog(@"Connection Killed.");
	}
}

-(void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode {
	// TODO: check no race condition
	switch (eventCode) {
		case NSStreamEventHasBytesAvailable: {
			if (!readbuf) {
				readbuf = [[NSMutableData alloc] initWithCapacity: MAX_LEN];
			}
			
			uint8_t buf[MAX_LEN];
			int len = 0;
            len = (unsigned int)[(NSInputStream *)aStream read:buf maxLength: MAX_LEN];
			
			if (len > 0) {
				[readbuf appendBytes:buf length:len];
			}
			
			uint32_t packetLen = PacketAssembler::GetLength((const uint8_t*)[readbuf bytes], [readbuf length]);
			
			if (packetLen > 0) { // a full packet
				Packet* p = PacketAssembler::Disassemble((const uint8_t*)[readbuf bytes]);
				uint32_t seq = p->sequenceNumber;
				id<CommunicatorDelegate> delegate = [delegates objectForKey:[NSNumber numberWithInt:seq]];
				id info = [userInfos objectForKey:[NSNumber numberWithInt:seq]];
				if (delegate)
					[delegate communicator:self receivedPacket:p userInfo:info];
				if (p!=nil)
					delete p;
				
				[readbuf replaceBytesInRange:NSMakeRange(0, packetLen) withBytes:NULL length:0]; // remove from buffer
			}
			
		} break;
		
//		case NSStreamEventEndEncountered: {
//			
//		} break;
		
		case NSStreamEventErrorOccurred: {
			[self close];
			
			readbuf = nil;
		} break;
	}
	
	[self touch];
}

@end
