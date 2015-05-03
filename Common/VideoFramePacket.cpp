//
//  VideoFramePacket.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "VideoFramePacket.h"
#include <cstring>

uint32_t VideoFramePacket::length() const {
	return _dataLength;
}

uint32_t VideoFramePacket::type() const {
	return Type::VIDEO_FRAME;
}

const uint8_t* VideoFramePacket::data() const {
	return _data;
}

int VideoFramePacket::serialize(uint8_t* output) const {
	memcpy(output, _data, _dataLength);
	return _dataLength;
}

VideoFramePacket::VideoFramePacket(const uint8_t* d, uint32_t l, uint32_t seq) : _dataLength(l) {
	_data = new uint8_t[l];
	memcpy(_data, d, l);
	
	sequenceNumber = seq;
}

VideoFramePacket::~VideoFramePacket() {
	delete[] _data;
}