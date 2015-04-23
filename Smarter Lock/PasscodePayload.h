//
//  PasscodePayload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Payload.h"

/*
 TYPE = 2
 16 bytes (character, [0-9A-Z]{16}) of passcode + expire datetime (64 bit unix timestamp)
 */

@interface PasscodePayload : NSObject <Payload>
-(id)initWithPasscode:(NSString*)passcode expire:(NSDate*)date;
@end
