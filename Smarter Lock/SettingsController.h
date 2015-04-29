//
//  SettingsController.h
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/28.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Communicator.h"

@interface SettingsController : UIViewController {
	Communicator* comm;
}

@property (nonatomic) IBOutlet UITextField *hostField, *portField;

-(IBAction)applyChanges:(id)sender;

@end
