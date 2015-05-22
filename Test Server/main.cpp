//
//  main.cpp
//  Test Server
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <iostream>

#include <sys/signal.h>
#include <pthread.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <map>

#include "Helpers.h"
#include "ServerThreads.h"

void sigHandler(int signo) {
//	if (signo == SIGKILL || signo == SIGSTOP) {
	ServerThreads::cleanup();
//	}
}

// -- end actions


int main(int argc, const char * argv[]) {
	if (signal(SIGINT, sigHandler) == SIG_ERR)
		printf("Can't catch SIGINT\n");
//	
//	if (signal(SIGKILL, sigHandler) == SIG_ERR)
//		printf("Can't catch SIGKILL\n");
//	
//	if (signal(SIGSTOP, sigHandler) == SIG_ERR)
//		printf("Can't catch SIGSTOP\n");

	std::map<int, struct clientinfo> monitorMap;
	pthread_t serverThreadId;
	pthread_create(&serverThreadId, nullptr, ServerThreads::startServer, nullptr);
	
	cv::VideoCapture cap;
	
	cap.open(0);
	
	if (!cap.isOpened()) {
		std::cerr << "***Could not initialize capturing...***\n";
		std::cerr << "Current parameter's value: \n";
//		return -1;
	}
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 400);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
	
	cv::Mat frame;
	
	while(1){
		cap >> frame;
		if(frame.empty()){
			std::cerr<<"frame is empty"<<std::endl;
			
			sleep(1);
			
			continue;
		}

//		idk(frame);
		
		ServerThreads::broadcastVideoFrame(frame);
		
		#ifndef NO_GUI
		cv::imshow("", frame);
		#endif
		cv::waitKey(75);
		
	}
	

	
    return 1;
}
