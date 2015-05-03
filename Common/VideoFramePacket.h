//
//  VideoFramePacket.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__VideoFramePacket__
#define __Smarter_Lock__VideoFramePacket__

#include "Packet.h"

/*
	payload = data
*/

class VideoFramePacket : public Packet {
	public:
	
	virtual uint32_t length() const;
	virtual uint32_t type() const ;
	
	virtual const uint8_t* data() const;
	
	virtual int serialize(uint8_t* output) const;
	
	VideoFramePacket(const uint8_t* data = nullptr, uint32_t dataLength = 0, uint32_t seq = 0);
	
	virtual ~VideoFramePacket();
	
	private:
	
	uint32_t _dataLength;
	uint8_t* _data;
};

#endif /* defined(__Smarter_Lock__VideoFramePacket__) */
