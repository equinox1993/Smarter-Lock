//
//  BluetoothThread.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/6/8.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__BluetoothThread__
#define __Smarter_Lock__BluetoothThread__

#include <stdio.h>

class BluetoothThread {
public:
    static void* startLoop(void* sth);
};

#endif /* defined(__Smarter_Lock__BluetoothThread__) */
