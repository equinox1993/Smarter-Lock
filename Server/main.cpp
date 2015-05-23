//
//  main.cpp
//  Test Server
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#define str_helper(x) #x
#define stringize(x) str_helper(x)

#define SERVER_DEFAULT_PORT         2333
#define SERVER_DEFAULT_NUM_THREADS  10
#define CAM_DEFAULT_WIDTH           400
#define CAM_DEFAULT_HEIGHT          300
#define CAM_WAIT					75
#define GPIO_DEFAULT_UNLOCK         "27"

#include <iostream>

#include <sys/signal.h>
#include <pthread.h>

#include <cstdlib>
#include <cstring>

#include "Helpers.h"
#include "ServerThreads.h"
#include "CameraLoop.h"

static const char HELP_STRING[] =
	"Usage: server [OPTION]...\n"
	"Run the Smarter Lock server\n"
	"\n"
	"Options:\n"
	"\t--help               Show this help\n"
	"\t--width=WIDTH        Set webcam width\n"
	"\t--height=HEIGHT      Set webcam height\n"
	"\t--wait=TIME			Wait TIME msec each cam loop\n"
	"\t--nogui              No OpenCV WebCam window\n"
	"\t--port=PORT          Set the PORT to listen\n"
	"\t--threads=NUM        Set NUM of threads\n"
	"\t--unlock-pin=PIN     Set unlock GPIO pin\n"
	"\n"
	"Default:\n"
	"gui,\n"
	"port=" stringize(SERVER_DEFAULT_PORT) "\n"
	"#threads=" stringize(SERVER_DEFAULT_NUM_THREADS) "\n"
	"width=" stringize(CAM_DEFAULT_WIDTH) "\n"
	"height=" stringize(CAM_DEFAULT_HEIGHT) "\n"
	"unlock-pin=" GPIO_DEFAULT_UNLOCK "\n"
	"wait=" stringize(CAM_WAIT) "\n"
	;

void sigHandler(int signo) {
	printf("Received signal... Doing cleanups...\n");
	ServerThreads::cleanup();
	printf("Done\n");
	exit(0);
}

int main(int argc, const char * argv[]) {

	// read args
	bool gui = true;
	uint16_t port = SERVER_DEFAULT_PORT;
	uint32_t numThreads = SERVER_DEFAULT_NUM_THREADS;
	
	int width = CAM_DEFAULT_WIDTH;
	int height = CAM_DEFAULT_HEIGHT;
	
	uint32_t wait = CAM_WAIT;
	
	const char gpioDefaultUnlock[] = GPIO_DEFAULT_UNLOCK;
	const char* unlockPin = gpioDefaultUnlock;
	
	for (int i = 1; i < argc; i++) {
		const char* arg = argv[i];
		
		if (strcmp(arg, "--help") == 0) {
			printf("%s", HELP_STRING);
			exit(0);
		} else if (strcmp(arg, "--nogui") == 0) {
			gui = false;
		} else if (strncmp(arg, "--port=", 7) == 0 && strlen(arg) > 7) {
			port = atoi(arg+7);
		} else if (strncmp(arg, "--threads=", 10) == 0 && strlen(arg) > 10) {
			numThreads = atoi(arg+10);
		} else if (strncmp(arg, "--width=", 8) == 0 && strlen(arg) > 8) {
			width = atoi(arg+8);
		} else if (strncmp(arg, "--height=", 9) == 0 && strlen(arg) > 9) {
			height = atoi(arg+9);
		} else if (strncmp(arg, "--unlock-pin=", 13) == 0 && strlen(arg) > 13) {
			unlockPin = arg+13;
		} else if (strncmp(arg, "--wait=", 7) == 0 && strlen(arg) > 7) {
			wait = atoi(arg+7);
		} else {
			printf("Error: unknown argument \"%s\".\n", arg);
			exit(2);
		}
	}
	
	ServerThreads::port = port;
	ServerThreads::numThreads = numThreads;
	ServerThreads::gpioUnlock = unlockPin;

	if (signal(SIGINT, sigHandler) == SIG_ERR)
		printf("Can't catch SIGINT\n");

	pthread_t serverThreadId;
	pthread_create(&serverThreadId, nullptr, ServerThreads::startServer, nullptr);
	
	CameraLoop::loop(width, height, gui, wait);
	
    return 1;
}
