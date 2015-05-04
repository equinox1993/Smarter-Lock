//
//  FirstViewController.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "FirstViewController.h"
#import "Authenticator.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>

RSA* rsaFromFile(const char* filename, bool pub) {
	FILE* fp = fopen(filename, "rb");
	
	if (!fp) {
		fprintf(stderr, "Unable to open RSA file.");
		return nullptr;
	}
	
	RSA* rsa = RSA_new(); //?!?!
	
	if (pub)
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	else
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	
	return rsa;
}


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
	
	NSString* docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	NSString* rsaPath = [docPath stringByAppendingPathComponent: @"public.pem"];
	
	if (![[NSFileManager defaultManager] fileExistsAtPath: rsaPath]) {
		UIAlertView* uiv = [[UIAlertView alloc] initWithTitle:@"Failed to load certificate" message:@"Please copy the public.pem file into the document folder for smarter lock." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
				[uiv show];
//				exit(1);
	}
	
	RSA* rsa = rsaFromFile([rsaPath UTF8String], true);
	
	[Communicator setRSA: rsa];
	
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
