//
//  TestGPIO.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/13.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <unistd.h>
#include "../Common/GPIO.h"

int main(int argc, const char * argv[]) {
	GPIO io;
	
//	io.setup("23", GPIO::Direction::IN);
	io.setup("27", GPIO::Direction::OUT);
	
	io.write("27", true);

	sleep(2);
	
//	bool p23tf;
//	io.read("23", p23tf);
//	
//	printf("P23: %d\n", p23tf);
//	
	io.cleanup();
	
	return 0;
}