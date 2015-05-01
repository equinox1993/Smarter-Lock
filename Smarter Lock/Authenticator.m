//
//  Authorizer.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "Authenticator.h"
@import LocalAuthentication;

@implementation Authenticator

static LAContext* context = nil;

+(void)authenticate: (void (^)(bool))cb {
    if (!context) {
        context = [[LAContext alloc] init];
    }
    
    if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:nil]) {
        [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"Authenticate to log into Smarter Lock" reply:^(BOOL success, NSError* err) {
            cb(success);
            
        }];
    } else {
        NSLog(@"No Bio");
		cb(true);
	}
}
@end
