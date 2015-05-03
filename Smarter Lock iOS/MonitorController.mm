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
	if (!pk || (pk->type() != Type::VIDEO_FRAME && pk->type() != Type::ACCEPT)) {
		UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle: @"Can't monitor the environment"
                                                          message: @"Wrong packet responded"
                                                         delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [myAlert show];
		return;
	}
	
	if (pk->type() == Type::VIDEO_FRAME) {
		// start
		VideoFramePacket* vfp = (VideoFramePacket*)pk;
		NSData* imgData = [NSData dataWithBytes: vfp->data() length: vfp->length()];
		UIImage* img = [UIImage imageWithData: imgData];
		[self.imageView setImage: img];
	}
}

@end
