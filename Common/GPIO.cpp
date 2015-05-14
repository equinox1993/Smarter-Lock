//
//  GPIO.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/13.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "GPIO.h"

#include <stdio.h>
#include <cstring>

static char GPIO_EXPORT_PATH[] = "/sys/class/gpio/export";
static char GPIO_UNEXPORT_PATH[] = "/sys/class/gpio/unexport";
static char GPIO_PATH[] = "/sys/class/gpio/";

static char DIRECTION_IN[] = "in";
static char DIRECTION_OUT[] = "out";

static char GPIO_TRUE[] = "1";
static char GPIO_FALSE[] = "0";

GPIO::GPIO() {
	registered = new std::vector<std::string>();
}

bool GPIO::setup(const char* pin, GPIO::Direction direction) {
	char* dir;
	
	switch (direction) {
		case GPIO::Direction::IN:
			dir = DIRECTION_IN;
			break;
		case GPIO::Direction::OUT:
			dir = DIRECTION_OUT;
			break;
		default:
			dir = nullptr;
	}
	
	FILE* efd = fopen(GPIO_EXPORT_PATH, "w");
	if (!efd) {
		perror("Setup EFD");
		return false;
	}
	fwrite(pin, sizeof(char), strlen(pin), efd);
	fclose(efd);
	
	const char* pinDir = (std::string(GPIO_PATH)+"gpio"+pin+"/direction").c_str();
	FILE* dfd = fopen(pinDir, "w");
	if (!dfd) {
		perror("Setup DFD");
		return false;
	}
	fwrite(dir, sizeof(char), strlen(dir), dfd);
	fclose(dfd);
	
	registered->push_back(pin);
	
	return true;
}

bool GPIO::write(const char* pin, bool tf) {
	const char* valDir = (std::string(GPIO_PATH)+"gpio"+pin+"/value").c_str();
	FILE* fd = fopen(valDir, "w");
	
	if (!fd) {
		perror((std::string("Can't open ")+valDir).c_str());
		return false;
	}
	
	char* tfStr = tf?GPIO_TRUE:GPIO_FALSE;
	
	fwrite(tfStr, sizeof(char), strlen(tfStr), fd);
	fclose(fd);
	
	return true;
}

bool GPIO::read(const char* pin, bool& tf) {
	const char* valDir = (std::string(GPIO_PATH)+"gpio"+pin+"/value").c_str();
	FILE* fd = fopen(valDir, "r");
	
	if (!fd) {
		perror((std::string("Can't open ")+valDir).c_str());
		return false;
	}
	
	char out[16];
	fread(out, sizeof(char), 16, fd);
	fclose(fd);
	
	if (strcmp(out, GPIO_TRUE) == 0)
		tf = true;
	else if (strcmp(out, GPIO_FALSE) == 0)
		tf = false;
	else
		return false;
	
	return true;
}

void GPIO::cleanup() {
	for (auto it = registered->begin(); it != registered->end(); it++) {
		const char* pin = (*it).c_str();
		
		FILE* fd = fopen(GPIO_UNEXPORT_PATH, "w");
		
		if (!fd) {
			perror("Can't open unexport");
		}
		fwrite(pin, sizeof(char), strlen(pin), fd);
		fclose(fd);
	}
	
	registered->clear();
}

GPIO::~GPIO() {
	cleanup();
	delete registered;
}