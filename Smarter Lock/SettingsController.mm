//
//  SettingsController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/28.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "SettingsController.h"

@implementation SettingsController

- (void)viewDidLoad {
	comm = [Communicator defaultCommunicator];
	
	self.hostField.text = comm.host;
	self.portField.text = [NSString stringWithFormat: @"%d", comm.port];
}

-(IBAction)applyChanges:(id)sender {
	comm.host = self.hostField.text;
	comm.port = [self.portField.text intValue];
}

@end
