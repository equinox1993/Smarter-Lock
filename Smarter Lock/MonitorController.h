//
//  MonitorController.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/29.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Communicator.h"

@interface MonitorController : UIViewController {
	Communicator* comm;
}

@property (nonatomic) IBOutlet UIImageView* imageView;

@end
