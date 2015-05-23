//
//  CameraLoop.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "CameraLoop.h"

#include "ServerThreads.h"

#include <iostream>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

cv::VideoCapture* cap = nullptr;

void openCamera(int width, int height) {
	if (cap)
		return;
	
	cap = new cv::VideoCapture();
	cap->open(0);
	
	if (!cap->isOpened()) {
		std::cerr << "***Could not initialize capturing...***\n";
		std::cerr << "Current parameter's value: \n";
		
		return;
	}
	
	cap->set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap->set(CV_CAP_PROP_FRAME_HEIGHT, height);
}

void closeCamera() {
	cap->release();
	delete cap;
	cap = nullptr;
}

void CameraLoop::loop(int width, int height, bool gui, uint32_t wait) {

	cv::Mat frame;
	
	while(1){
		if (ServerThreads::countMonitors() == 0) {
			if (cap)
				closeCamera();
			
			sleep(1);
			continue;
		}
		
		if (!cap)
			openCamera(width, height);
		
		(*cap) >> frame;
		
		if(frame.empty()){
			std::cerr<<"frame is empty"<<std::endl;
			
			sleep(1);
			
			continue;
		}

//		idk(frame);
		
		ServerThreads::broadcastVideoFrame(frame);
	
        if (gui)
            cv::imshow("", frame);
		
        cv::waitKey(wait);
		
	}
}
