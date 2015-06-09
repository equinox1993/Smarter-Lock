//
//  BluetoothThread.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/6/8.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "BluetoothThread.h"

#include <string>
#include <iostream>
#include <stdio.h>

#include <unistd.h>

#define MAC     "80:BE:05:3E:FA:64"
#define COMMAND "hcitool tpl " // TODO

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void* BluetoothThread::startLoop(void* sth) {
    while (true) {
        auto result = exec(COMMAND MAC);
        
        std::cout << result;
        
        sleep(1);
    }
    
    return nullptr;
}