//
//  REPL.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/23.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__REPL__
#define __Smarter_Lock__REPL__

#include <string>

typedef void (*CleanupFunc)();

class REPL {
public:
	static void* startLoop(void* sth);
	static void loop();
	
	static CleanupFunc cleanupFn;
	static const char* password;
};

#endif /* defined(__Smarter_Lock__REPL__) */
