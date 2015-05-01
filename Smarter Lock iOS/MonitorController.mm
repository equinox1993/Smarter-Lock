//
//  MonitorController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/29.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "MonitorController.h"
#import "CommandPacket.h"

@implementation MonitorController

-(void)viewDidLoad {
	[super viewDidLoad];
	
	comm = [Communicator defaultCommunicator];
	
}

-(void)viewDidAppear:(BOOL)animated {
	CommandPacket* pk = new CommandPacket(Type::REQUEST_MONITOR);
	[comm writePacket:pk target:self withSelector:@selector(startMonitor:)];
	delete pk;
}

-(void)viewDidDisappear:(BOOL)animated {
	CommandPacket* pk = new CommandPacket(Type::STOP_MONITOR);
	[comm writePacket:pk target:self withSelector:@selector(stopMonitor:)];
	delete pk;
}

-(void)startMonitor:(NSValue*)pkptr {
	Packet* pk = (Packet*)[pkptr pointerValue];
	if (!pk || pk->type() != Type::ACCEPT) {
		UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle: @"Can't monitor the environment"
                                                          message: @"Start monitoring request not accepted"
                                                         delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [myAlert show];
		return;
	}
	// else NSLog(@"Started Monitoring");
	
	// start
}

-(void)stopMonitor:(NSValue*)pkptr {
//	NSLog(@"Monitor stopped");
}

@end
