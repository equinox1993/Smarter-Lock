//
//  ZxingBridge.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/23.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__ZxingBridge__
#define __Smarter_Lock__ZxingBridge__

#include <stdio.h>

#include <opencv2/core/core.hpp>

#include <zxing/qrcode/QRCodeReader.h>

namespace ZxingBridge {
	using namespace zxing;

	Ref<LuminanceSource> matToLuminance(const cv::Mat& mat);
	std::string decode(const cv::Mat& mat);
}

#endif /* defined(__Smarter_Lock__ZxingBridge__) */
