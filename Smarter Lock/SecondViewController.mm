//
//  SecondViewController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "SecondViewController.h"
#import "QR/UIImage+MDQRCode.h"
#import "CommandPacket.h"

@interface SecondViewController ()

@end

@implementation SecondViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
	[self changeQR:@"Hello World 233333!"];
	
	comm = [Communicator defaultCommunicator];
    
}

- (void)changeQR:(NSString*)str {
	UIImage* qrImg = [UIImage mdQRCodeForString:str size:self.imageView.bounds.size.width];
    self.imageView.image = qrImg;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

// Actions
-(IBAction)unlock:(id)sender {
	CommandPacket* unlockPacket = new CommandPacket(Type::UNLOCK);
	
	[comm writePacket:unlockPacket target:self withSelector:@selector(unlockReceivePacket:)];
	
	delete unlockPacket;
}

-(void)unlockReceivePacket:(NSValue*)responsePtr {
	Packet* response = (Packet*)[responsePtr pointerValue];
	if (response == nil) {
		UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle:@"Failed to unlock"
     	message:@"No response from the server."
          delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
		[myAlert show];
		return;
	}
	
	if (response->type() != Type::ACCEPT) {
		UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle:@"Failed to unlock"
     	message:@"Request not accepted."
          delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
		[myAlert show];
		return;
	}
	
	UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle:@"Successfully unlocked"
		message:@"The door is successfully unlocked."
		delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
	[myAlert show];
}

@end
