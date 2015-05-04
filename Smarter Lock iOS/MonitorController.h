//
//  MonitorController.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/29.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Communicator.h"
//#import "UDPCommunicator.h"

@interface MonitorController : UIViewController {
	Communicator* comm;
//	UDPCommunicator* ucomm;
//	
//	struct sockaddr_in serveraddr;
//	socklen_t serveraddrlen;
	uint32_t monSeq;
	
	uint64_t videoKey;
}

@property (nonatomic) IBOutlet UIImageView* imageView;

@end
