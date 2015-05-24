//
//  VideoFramePacket.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "VideoFramePacket.h"
#include <cstring>

uint32_t pad8(uint32_t in) {
	uint32_t roundDown = in / 8 * 8;
	
	if (in % 8 == 0)
		return roundDown;
	else return roundDown + 8;
}

uint32_t VideoFramePacket::length() const {
	return _dataLength;
}

uint32_t VideoFramePacket::type() const {
	return Type::VIDEO_FRAME;
}

const uint8_t* VideoFramePacket::data() const {
	return _data;
}

void VideoFramePacket::crypt(uint64_t key) {
	uint64_t* datacast = (uint64_t*)_data;
	int octCount = _dataLength / 8;
	
	for (int i = 0; i < octCount; i++) {
		datacast[i] = datacast[i] ^ (key ^ i);
	}
}

int VideoFramePacket::serialize(uint8_t* output) const {
	memcpy(output, _data, _dataLength);
	return _dataLength;
}

VideoFramePacket::VideoFramePacket(const uint8_t* d, uint32_t l, uint32_t seq) : _dataLength(pad8(l)) {
	_data = new uint8_t[pad8(l)];
	memcpy(_data, d, l);
	
	sequenceNumber = seq;
}

VideoFramePacket::~VideoFramePacket() {
	delete[] _data;
}