//
//  Authorizer.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/30.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Authenticator : NSObject {

}

+(void)authenticate: (void (^)(bool))cb;

@end
