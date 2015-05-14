//
//  GPIO.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/13.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#ifndef __Smarter_Lock__GPIO__
#define __Smarter_Lock__GPIO__

#include <string>
#include <vector>

class GPIO {
  public:
	
	enum Direction {
		IN = 0,
		OUT = 1
	};
	
	bool setup(const char* pin, Direction);
	bool read(const char* pin, bool& tf);
	bool write(const char* pin, bool tf);
	void cleanup();
	
	GPIO();
	~GPIO();
	
  private:
	std::vector<std::string>* registered;
};

#endif /* defined(__Smarter_Lock__GPIO__) */
