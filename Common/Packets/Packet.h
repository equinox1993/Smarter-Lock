//
//  Payload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

/*
    Numbers are all big-endian
	
	Non-encrypted packet:
 
 	+----------+
	|   SIGN   |  -> PACKET_SIGNATURE for error detection (4 bytes)
    +----------+
    |   TYPE   |  -> type of payload (4 bytes)
	+----------+
	|  SEQNUM  |  -> sequence number (4 bytes)
    +----------+
    |   PLEN   |  -> len of payload (4 bytes)
	+----------+
    | Payload  |  -> payload (length of PLEN)
    +----------+
 l
 	Total head length = 4 * 4 = 16
    Total packet length = align4(Total head length + PLEN)
 
 	RSA-encrypted packet:
	
	+----------+
	|   SIGN   |  -> PACKET_SIGNATURE_ENCRYPTED (4 bytes)
	+----------+
	|   ELEN   |  -> len of encrypted packet (4 bytes)
	+----------+
	|   SIGN   |
	|   TYPE   |  -> (encrypted data) the non-encrypted packet
	|  SEQNUM  |
	|   ....   |
	+----------+
	
	packets can have their own sub-encryption (hybrid encryption).
	
	** ELEN < RSA_size(RSA) **
 */

#ifndef __Smarter_Lock__Packet__
#define __Smarter_Lock__Packet__

#ifndef htonll
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#endif

#ifndef ntohll
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

#include <stdint.h>
#include <arpa/inet.h>

#define PACKET_ENCRYPTED_HEAD_LENGTH 8
#define PACKET_OFFSET_EPAYLOAD 8
#define PACKET_OFFSET_ELEN 4

#define PACKET_HEAD_LENGTH 16
#define PACKET_OFFSET_SIGN 0
#define PACKET_OFFSET_TYPE 4
#define PACKET_OFFSET_SEQNUM 8
#define PACKET_OFFSET_PLEN 12
#define PACKET_OFFSET_PAYLOAD 16

static uint32_t PACKET_SIGNATURE = htonl(0xC5E48101); // CSE 481 L =_=, big endian
static uint32_t PACKET_SIGNATURE_ENCRYPTED = htonl(0xC5E48102);

class Packet {
	public:
	
	uint32_t sequenceNumber;
	
	virtual int serialize(uint8_t* output) const = 0; // serialized payload, output must be allocated
	virtual uint32_t length() const = 0;
	virtual uint32_t type() const = 0;
	virtual ~Packet() {};
};

enum Type {
	ACCEPT = 1,
	REJECT = 2,
	
	UNLOCK = 8,
	REQUEST_PASSCODE = 9,
	REQUEST_LOG = 10,
	REQUEST_MONITOR = 11,
	STOP_MONITOR = 12,
	RENEW_PASSCODE = 13,
	
	PASSCODE = 16,
	VIDEO_FRAME = 17,
	VIDEO_KEY = 18,
	
	DEVICE_TOKEN = 21
};
// TODO: add case to PacketAssembler::Disassemble when adding new payload type.

#endif /* defined(__Smarter_Lock__Packet__) */
