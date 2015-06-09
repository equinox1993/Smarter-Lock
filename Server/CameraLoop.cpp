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

#include <zxing/ReaderException.h>

#include "ZxingBridge.h"

cv::VideoCapture* cap = nullptr;

void openCamera(int dev, int width, int height) {
	if (cap)
		return;
	
	cap = new cv::VideoCapture();
	cap->open(dev);
	
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

void CameraLoop::loop(int device, int width, int height, bool gui, uint32_t wait) {
	decQrWait = 0;
	
	cv::Mat frame;
	
	while(1){
		if (ServerThreads::countMonitors() == 0 && decQrWait <= 0) {
			if (cap)
				closeCamera();
			
			sleep(1);
			continue;
		}
		
		if (!cap)
			openCamera(device, width, height);
		
		(*cap) >> frame;
		
		if(frame.empty()){
			std::cerr<<"frame is empty"<<std::endl;
			
			sleep(1);
			
			continue;
		}
		
		if (decQrWait > 0) {
			try {
				std::string code = ZxingBridge::decode(frame);
				std::cout<<"Decoded: "<<code<<std::endl;
				decQrWait = 0; // detected
				
				if (ServerThreads::unlockWithPasscode(code.c_str()))
					std::cout<<"Door unlocked\n";
				else
					std::cout<<"Wrong passcode\n";
			} catch (const ZxingBridge::ReaderException& e) {
				// no qr detected
				decQrWait--;
			}
		}

//		idk(frame);
		
		ServerThreads::broadcastVideoFrame(frame);
	
        if (gui)
            cv::imshow("", frame);
		
        cv::waitKey(wait);
	}
}


//void CameraLoop::loop(int device, int width, int height, bool gui, uint32_t wait) {
//	cv::Mat frame;
//	
//	openCamera(device, width, height);
//	
//	while(1){
//		
//		(*cap) >> frame;
//		
//		if(frame.empty()){
//			std::cerr<<"frame is empty"<<std::endl;
//			
//			sleep(1);
//			
//			continue;
//		}
//		
//		ServerThreads::broadcastVideoFrame(frame);
//	
//        if (gui)
//            cv::imshow("", frame);
//		
//        int key = cv::waitKey(wait) % 256;
//		if (key == ' ') {
//			try {
//				std::string code = ZxingBridge::decode(frame);
//				std::cout<<"Decoded: "<<code<<std::endl;
//			} catch (const ZxingBridge::ReaderException& e) {
//				std::cout<<"No code detected."<<std::endl;
//			}
//			
//		}
//	}
//}

bool CameraLoop::gui;
int CameraLoop::width;
int CameraLoop::height;
int CameraLoop::device;
uint32_t CameraLoop::wait;
int CameraLoop::decQrWait;

void* CameraLoop::startLoop(void* sth) {
	loop(device, width, height, gui, wait);
	return nullptr;
}