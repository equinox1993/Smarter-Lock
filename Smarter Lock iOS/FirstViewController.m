//
//  FirstViewController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "FirstViewController.h"
#import "Authenticator.h"

@interface FirstViewController ()

@end

@implementation FirstViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSUserDefaults* ud = [NSUserDefaults standardUserDefaults];
	
	if ([ud boolForKey: @"auth"]) {
		[Authenticator authenticate:^(bool success) {
			if (success) {
				NSLog(@"Success");
			} else {
				UIAlertView* uiv = [[UIAlertView alloc] initWithTitle:@"Failed to authenticate." message:@"Failed to pass authentication" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
				[uiv show];
				exit(1);
			}
		}];
	}
	
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
