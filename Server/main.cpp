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
#define CAM_DEV						0
#define GPIO_DEFAULT_UNLOCK         "27"
#define GPIO_DEFAULT_ERROR			"17"
#define SAFETY_DEFAULT_RSA_FILE     "./private.pem"
#define SAFETY_DEFAULT_PASSWD		""
#define APN_DEFAULT_HOST			"localhost"
#define APN_DEFAULT_PORT			2303
#define APN_DEFAULT_RSA_FILE		"./apnserver_public.pem"
#define MISC_DEFAULT_QR_EXP			7200

#define KWARN  "\x1b[33m"
#define KERR   "\x1b[31m"
#define KREST  "\x1b[0m"

#include <iostream>

#include <sys/signal.h>
#include <pthread.h>

#include <cstdlib>
#include <cstring>

#include "Helpers.h"
#include "ServerThreads.h"
#include "CameraLoop.h"
#include "REPL.h"
#include "APNClient.h"
#include "BluetoothThread.h"

#include "../Common/RSAHelper.inl"

#include "../simpleini/SimpleIni.h"

const char HELP_STRING[] =
	"Usage: server [OPTION]...\n"
	"Run the Smarter Lock server\n"
	"\n"
	"Options:\n"
	"\t--help               Show this help\n"
	"\t--nogui              No OpenCV WebCam window\n"
	"\n"
	"Default:\n"
	"gui,\n"
	"port=" stringize(SERVER_DEFAULT_PORT) "\n"
	"#threads=" stringize(SERVER_DEFAULT_NUM_THREADS) "\n"
	"width=" stringize(CAM_DEFAULT_WIDTH) "\n"
	"height=" stringize(CAM_DEFAULT_HEIGHT) "\n"
	"unlock-pin=" GPIO_DEFAULT_UNLOCK "\n"
	"wait=" stringize(CAM_WAIT) "\n"
	"rsa=" SAFETY_DEFAULT_RSA_FILE "\n"
	"passwd=" SAFETY_DEFAULT_PASSWD "\n"
	"apn_host=" APN_DEFAULT_HOST "\n"
	"apn_port=" stringize(APN_DEFAULT_PORT) "\n"
	"apn_rsa=" APN_DEFAULT_RSA_FILE "\n"
	"qr_expire=" stringize(MISC_DEFAULT_QR_EXP) "\n"
	;

void cleanup() {
	printf("Doing cleanups...\n");
	ServerThreads::cleanup();
	printf("Done\n");
	exit(0);
}

void sigHandler(int signo) {
	printf("Received signal %d...\n", signo);
	cleanup();
}

int main(int argc, const char * argv[]) {
	CSimpleIniA ini;
	
	ini.SetUnicode();
	if (ini.LoadFile("config.ini") == SI_FILE) {
		std::cout<< KWARN "Warning:" <<KREST << " Cannot load config.ini. Default setting is used.\n\n";
	}

	bool gui = true;
	
	uint16_t port = atoi(ini.GetValue("server", "port", stringize(SERVER_DEFAULT_PORT)));
	uint32_t numThreads = atoi(ini.GetValue("server", "threads", stringize(SERVER_DEFAULT_NUM_THREADS)));
	
	int width = atoi(ini.GetValue("camera", "width", stringize(CAM_DEFAULT_WIDTH)));
	int height = atoi(ini.GetValue("camera", "height", stringize(CAM_DEFAULT_HEIGHT)));
	uint32_t wait = atoi(ini.GetValue("camera", "wait", stringize(CAM_WAIT)));
	int dev = atoi(ini.GetValue("camera", "device", stringize(CAM_DEV)));
	
	const char* unlockPin = ini.GetValue("gpio", "unlock", GPIO_DEFAULT_UNLOCK);
	const char* errorPin = ini.GetValue("gpio", "error", GPIO_DEFAULT_ERROR);
	
	const char* rsapath = ini.GetValue("safety", "pem", SAFETY_DEFAULT_RSA_FILE);
	const char* passwd = ini.GetValue("safety", "password", SAFETY_DEFAULT_PASSWD);
	
	uint32_t qrexp = atoi(ini.GetValue("misc", "qrexpire", stringize(MISC_DEFAULT_QR_EXP)));
	
	for (int i = 1; i < argc; i++) {
		const char* arg = argv[i];
		
		if (strcmp(arg, "--help") == 0) {
			printf("%s", HELP_STRING);
			exit(0);
		} else if (strcmp(arg, "--nogui") == 0) {
			gui = false;
		} else {
			std::cerr<<KERR <<"Error:" << KREST << "unknown argument \""<<arg<<'"'<<std::endl;
			exit(2);
		}
	}
	
	ServerThreads::qrexp = qrexp;
	ServerThreads::port = port;
	ServerThreads::numThreads = numThreads;
	ServerThreads::gpioUnlock = unlockPin;
	ServerThreads::gpioError = errorPin;
	ServerThreads::rsaFile = rsapath;

	if (signal(SIGINT, sigHandler) == SIG_ERR)
		std::cerr<<"Can't catch SIGINT\n";
	
	pthread_t serverThreadId;
	pthread_create(&serverThreadId, nullptr, ServerThreads::startServer, nullptr);
	
	const char* apnRsaFile = ini.GetValue("apn", "pem", APN_DEFAULT_RSA_FILE);
	
	APNClient::DefaultClient.host = ini.GetValue("apn", "host", APN_DEFAULT_HOST);
	APNClient::DefaultClient.port = atoi(ini.GetValue("apn", "port", stringize(APN_DEFAULT_PORT)));
	APNClient::DefaultClient.rsa = rsaFromFile(apnRsaFile, true);
	
	REPL::cleanupFn = cleanup;
	REPL::password = passwd;
	pthread_t replThreadId;
	pthread_create(&replThreadId, nullptr, REPL::startLoop, nullptr);
    
    pthread_t btThreadId;
    pthread_create(&btThreadId, nullptr, BluetoothThread::startLoop, nullptr);
	
	CameraLoop::loop(dev, width, height, gui, wait);
	
    return 1;
}
