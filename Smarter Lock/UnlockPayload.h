//
//  UnlockPayload.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/22.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Payload.h"

/*
    PLEN = 0
    TYPE = 1
 */

@interface UnlockPayload : NSObject <Payload>
+(id)defaultPayload;
@end
