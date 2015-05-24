//
//  SettingsController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/28.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "SettingsController.h"
#import "SimplePacket.h"

@implementation SettingsController

- (void)viewDidLoad {
	userDefaults = [NSUserDefaults standardUserDefaults];
	comm = [Communicator defaultCommunicator];

	NSString* host = [userDefaults stringForKey: @"host"];
	int port = [userDefaults integerForKey: @"port"];
	bool verifyFp = [userDefaults boolForKey: @"auth"];
	
	if (host && port) {
		self.hostField.text = host;
		self.portField.text = [NSString stringWithFormat: @"%d", port];
		self.fpSwitch.on = verifyFp;
	}  else {
		self.hostField.text = comm.host;
		self.portField.text = [NSString stringWithFormat: @"%d", comm.port];
	}
}

-(IBAction)applyChanges:(id)sender {
	comm.host = self.hostField.text;
	comm.port = [self.portField.text intValue];
	
	[userDefaults setObject: comm.host forKey:@"host"];
	[userDefaults setInteger: comm.port forKey: @"port"];
	[userDefaults setBool: self.fpSwitch.on forKey: @"auth"];
	
	[userDefaults synchronize];
	
	NSData* data = comm.token;
	SimplePacket sp((const uint8_t*)[data bytes], [data length], Type::DEVICE_TOKEN);
	[[Communicator defaultCommunicator] writePacket: &sp target:nil withSelector:nil];
}

@end
