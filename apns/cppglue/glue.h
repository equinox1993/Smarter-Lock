//
//  glue.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/24.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__glue__
#define __Smarter_Lock__glue__

extern "C" {

int setRSA(const char* filename, int ispub);

// input = packet data, output = 64 byte empty space
// return success = 1, failed = 0
int getToken(const char* input, char* output); // not null-terminated
int getLength(const char* input, int maxLen);

// input = 64 byte token hex, output = DEVICE_TOKEN packet (length <= 256), return length
int makePacket(const char* input, char* output);
}

#endif /* defined(__Smarter_Lock__glue__) */
