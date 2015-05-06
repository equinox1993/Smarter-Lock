//
//  SecondViewController.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Communicator.h"

@interface SecondViewController : UIViewController {
	Communicator* comm;
}

@property (nonatomic) IBOutlet UILabel* expLabel;
@property (nonatomic) IBOutlet UIImageView* imageView;

-(IBAction)refreshQR:(id)sender;
-(IBAction)unlock:(id)sender;
-(IBAction)saveImage:(id)sender;

@end

