//
//  StringHex.inl
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/24.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <sstream>

#ifndef __STRING_HEX_INL_
#define __STRING_HEX_INL_

inline std::string arrToHex(const uint8_t* arr, int len) {
	std::ostringstream ost;
	ost << std::hex;
	for (int i = 0; i < len; i++) {
		uint8_t cur = arr[i];
		
		if (cur < 16)
			ost << 0;
		
		ost << (int)cur;
	}
	
	return ost.str();
}

inline int hexToInt(char hex) {
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	else if (hex >= 'a' && hex <= 'f')
		return hex-'a'+10;
	else if (hex >= 'A' && hex <= 'F')
		return hex-'A'+10;
	else
		return 0;
}

inline void hexToArr(std::string hex, uint8_t* arr) {
	int j = 0;
	for (int i = 0; i < hex.length(); i+=2) {
		int b1 = hexToInt(hex[i]);
		int b2 = hexToInt(hex[i+1]);
		
		int full = b1*16+b2;
		
		arr[j] = (uint8_t)full;
		j++;
	}
}

#endif