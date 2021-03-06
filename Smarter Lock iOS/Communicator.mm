//
//  Communicator.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "Communicator.h"

#define MAX_LEN 8192
#define CONN_KILLER_THREAD_INTERVAL 10.0

@implementation Communicator

static NSString* DefaultHost = @"localhost";
static unsigned int DefaultPort = 2333;
static Communicator* DefaultComm = nil;
static NSTimeInterval DefaultTimeout = 20;

static RSA* rsa;

+(void)setDefaultHost:(NSString*)h {DefaultHost = h;}
+(void)setDefaultPort:(unsigned int)p {DefaultPort = p;}
+(NSString*)defaultHost {return DefaultHost;}
+(unsigned int)defaultPort {return DefaultPort;}

int Encrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_public_encrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}
int Decrypt(int flen, const uint8_t* from, uint8_t* to) {
	if (!rsa)
		return -1;
	return RSA_public_decrypt(flen, from, to, rsa, RSA_PKCS1_PADDING);
}


+(void)setRSA:(RSA *)r {
	rsa = r;
	
	PacketAssembler::SetEncryptor(Encrypt);
	PacketAssembler::SetDecryptor(Decrypt);
}

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
	targets = [[NSMutableDictionary alloc] init];
	selectors = [[NSMutableDictionary alloc] init];
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
	readbuf = nil;
	
    [ostream close];
    [istream close];
	
	[ostream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[istream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    ostream = nil;
    istream = nil;
	
	[targets removeAllObjects];
	[selectors removeAllObjects];
	
	connected = false;
}

-(void)dealloc {
	[self close];
}

//-(void)writeCString:(const u_int8_t *)str length: (size_t)len {
//    [self connect];
//    [ostream write:str maxLength:len];
//	[self touch];
////    [self close];
//}
//
//
-(void)writePacket:(const Packet*)pl {
	size_t totalLen;
	const uint8_t* packetBytes = PacketAssembler::Assemble(pl, totalLen, true);
	
	[ostream write:packetBytes maxLength:totalLen];
	
	delete[] packetBytes;
	[self touch];
}

-(void)consumeQueue {
	packetQueueMutex.lock();
	
	while ([ostream hasSpaceAvailable] && packetQueue.size() > 0) {
		Packet* first = *packetQueue.begin();
		[self writePacket: first];
		
		delete first;
		packetQueue.erase(packetQueue.begin());
	}
	packetQueueMutex.unlock();
	
	[self touch];
}

-(void)writePacket:(Packet*)pl target:(id)target withSelector:(SEL)sel {
	if (!pl->sequenceNumber)
		pl->sequenceNumber = curSeq;
	
	packetQueue.push_back(pl);
	
	if (target != nil) {
		[targets setObject:target forKey:[NSNumber numberWithInt:curSeq]];
		[selectors setObject:NSStringFromSelector(sel) forKey:[NSNumber numberWithInt:curSeq]];
	}
	
	curSeq++;
	
	[self connect];
	[self consumeQueue];
}

-(void)blockingWritePacket:(Packet&)pl target:(id)target withSelector:(SEL)sel {
	if (!pl.sequenceNumber)
		pl.sequenceNumber = curSeq;
	
	size_t totalLen;
	const uint8_t* packetBytes = PacketAssembler::Assemble(&pl, totalLen, true);
	
	[self connect];
	
	
	[ostream write:packetBytes maxLength:totalLen];
	[self touch];
	
//    [ostream write:packetBytes maxLength:totalLen];
	if (target != nil) {
		[targets setObject:target forKey:[NSNumber numberWithInt:curSeq]];
		[selectors setObject:NSStringFromSelector(sel) forKey:[NSNumber numberWithInt:curSeq]];
	}
	curSeq++;
//	[self touch];
	
	delete[] packetBytes;
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
			
			int32_t packetLen = PacketAssembler::GetLength((const uint8_t*)[readbuf bytes], [readbuf length]);
			
			if (packetLen > 0) { // a full packet
				Packet* p = PacketAssembler::Disassemble((const uint8_t*)[readbuf bytes]);
				uint32_t seq = p->sequenceNumber;
				id target = [targets objectForKey:[NSNumber numberWithInt:seq]];
				SEL selector = NSSelectorFromString([selectors objectForKey:[NSNumber numberWithInt:seq]]);
				if (target)
					[target performSelector: selector withObject: [NSValue valueWithPointer: p]];
				if (p!=nil)
					delete p;
				
//				[targets removeObjectForKey:[NSNumber numberWithInt:seq]];
//				[selectors removeObjectForKey:[NSNumber numberWithInt:seq]];
				
				[readbuf replaceBytesInRange:NSMakeRange(0, packetLen) withBytes:NULL length:0]; // remove from buffer
			} else if (packetLen < 0) { // error occurs
				[self close];
			}
			
		} break;
		
//		case NSStreamEventEndEncountered: {
//			
//		} break;
		
		case NSStreamEventErrorOccurred: {
			[self close];
		} break;
		
		case NSStreamEventHasSpaceAvailable: {
			if (aStream == ostream) {
				[self consumeQueue];
			}
		} break;
	}
	
	[self touch];
}

@end
