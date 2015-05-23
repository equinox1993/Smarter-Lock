//
//  CameraLoop.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__CameraLoop__
#define __Smarter_Lock__CameraLoop__

#include <stdint.h>

class CameraLoop {
public:
	static void loop(int width, int height, bool gui, uint32_t wait);
};

#endif /* defined(__Smarter_Lock__CameraLoop__) */
