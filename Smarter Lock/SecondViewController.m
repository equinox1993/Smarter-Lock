//
//  SecondViewController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "SecondViewController.h"
#import "QR/UIImage+MDQRCode.h"
#import "Communicator.h"
#import "UnlockPayload.h"

@interface SecondViewController ()

@end

@implementation SecondViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
	[self changeQR:@"Hello World 233333!"];
    
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
    [[Communicator defaultCommunicator] writePayload: [UnlockPayload defaultPayload]];
}

@end
