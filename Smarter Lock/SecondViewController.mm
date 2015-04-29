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
#import "PasscodePacket.h"

@interface SecondViewController ()

@end

@implementation SecondViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
//	[self changeQR:@"Hello World 233333!"];
	comm = [Communicator defaultCommunicator];
    
    [self requestPasscode];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)changeQR:(NSString*)str {
	UIImage* qrImg = [UIImage mdQRCodeForString:str size:self.imageView.bounds.size.width];
    self.imageView.image = qrImg;
}

-(void)requestPasscode {
    CommandPacket* reqPacket = new CommandPacket(Type::REQUEST_PASSCODE);
    [comm writePacket:reqPacket target:self withSelector:@selector(receivePasscodePacket:)];
    delete reqPacket;
}

// responses
-(void)receivePasscodePacket:(NSValue*)qrPtr {
    Packet* res = (Packet*)[qrPtr pointerValue];
    
    if (![self packetTypeCheckActual:res->type() expect:Type::PASSCODE errTitle:@"Wrong return type" errMsg:@"Expected a passcode packet"])
        return;
    
    PasscodePacket* pcPacket = (PasscodePacket*)res;
    
    NSString* passcode = [NSString stringWithUTF8String: pcPacket->passcode.c_str()];
//    NSDate* expiration = [NSDate dateWithTimeIntervalSince1970:pcPacket->date];
    
    [self changeQR:passcode];
}

-(void)unlockReceivePacket:(NSValue*)responsePtr {
	Packet* response = (Packet*)[responsePtr pointerValue];
	
    if (![self packetTypeCheckActual:response->type() expect:Type::ACCEPT errTitle:@"Failed to unlock" errMsg:@"Request not accepted."])
        return;
	
	UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle:@"Successfully unlocked"
		message:@"The door is successfully unlocked."
		delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
	[myAlert show];
}

-(BOOL)packetTypeCheckActual:(uint32_t)actual expect:(uint32_t)exp errTitle: (NSString*)title errMsg:(NSString*)msg {
    if (actual != exp) {
        UIAlertView *myAlert = [[UIAlertView alloc] initWithTitle:title
                                                          message:msg
                                                         delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [myAlert show];
        return false;
    }
    return true;
}



// Actions
-(IBAction)refreshQR:(id)sender {
    [self requestPasscode];
}

-(IBAction)unlock:(id)sender {
    CommandPacket* unlockPacket = new CommandPacket(Type::UNLOCK);
    
    [comm writePacket:unlockPacket target:self withSelector:@selector(unlockReceivePacket:)];
    
    delete unlockPacket;
}

@end
