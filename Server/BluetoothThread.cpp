//
//  BluetoothThread.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/6/8.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "BluetoothThread.h"
#include "ServerThreads.h"

#include <string>
#include <iostream>
#include <stdio.h>

#include <cstdlib>

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
    bool unlocked = false;
    
    while (true) {
        auto result = exec(COMMAND MAC);
        
        if (result.length() < 31) {
            sleep(1);
            continue;
        }
            
        
        const char* tpl_str = result.c_str()+30;
        
        long tpl = strtol(tpl_str, nullptr, 10);
        
        if (!unlocked && tpl <=0) {
            ServerThreads::unlockDoor();
            unlocked = true;
        } else if (unlocked && tpl > 0) {
            unlocked = false;
        }
        
        sleep(1);
    }
    
    return nullptr;
}