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

+(void)authenticate {
    if (!context) {
        context = [[LAContext alloc] init];
    }
    
    if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:nil]) {
        NSLog(@"Has Bio");
        
        [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"Authentication for server login" reply:^(BOOL success, NSError* err) {
            if (success) {
                NSLog(@"Success");
            } else {
                NSLog(@"Failed");
            }
            
        }];
    } else
        NSLog(@"No Bio");
}
@end
