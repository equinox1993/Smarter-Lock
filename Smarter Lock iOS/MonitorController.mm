//
//  MonitorController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/29.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "MonitorController.h"
#import "CommandPacket.h"
#import "VideoFramePacket.h"
#import "SimplePacket.h"

//#include <cstring>

@implementation MonitorController

-(void)viewDidLoad {
	[super viewDidLoad];
	
	comm = [Communicator defaultCommunicator];
//	ucomm = new UDPCommunicator(err);
}

-(void)dealloc {
//	delete ucomm;
}

-(void)viewDidAppear:(BOOL)animated {
	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
	
	CommandPacket pk = CommandPacket(Type::REQUEST_MONITOR);
	[comm writePacket:&pk target:self withSelector:@selector(getImage:)];
	
	self.imageView.contentMode = UIViewContentModeScaleAspectFit;
	
	monSeq = pk.sequenceNumber;
}

-(void)viewDidDisappear:(BOOL)animated {
	CommandPacket pk = CommandPacket(Type::STOP_MONITOR, monSeq);
	[comm writePacket:&pk target:self withSelector:@selector(getImage:)];
}

-(void)getImage:(NSValue*)pkptr {
	Packet* pk = (Packet*)[pkptr pointerValue];
	
	if (!pk)
		return;
	
	switch (pk->type()) {
  		case Type::VIDEO_KEY: {
			SimplePacket* sp = (SimplePacket*)pk;
			memcpy(&videoKey, sp->payload, 8);
			break;
		}
		case Type::VIDEO_FRAME: {
			VideoFramePacket* vfp = (VideoFramePacket*)pk;
			vfp->crypt(videoKey);
			NSData* imgData = [NSData dataWithBytes: vfp->data() length: vfp->length()];
			UIImage* img = [UIImage imageWithData: imgData];
			[self.imageView setImage: img];
			break;
		}
		case Type::ACCEPT: {
			break;
		}
  		default: {
			UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle: @"Can't monitor the environment"
                                                          message: @"Wrong packet responded"
                                                         delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        	[myAlert show];
			break;
		}
	}
}

@end
