//
//  AppDelegate.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/4/16.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#import "AppDelegate.h"
#import "Communicator.h"
#import "Authenticator.h"

#include "SimplePacket.h"

#include "../Common/RSAHelper.inl"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
	NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
	Communicator* comm = [Communicator defaultCommunicator];

	NSString* host = [userDefaults stringForKey: @"host"];
	int port = [userDefaults integerForKey: @"port"];
	bool verifyFp = [userDefaults boolForKey: @"auth"];
	
	if (host && port) {
		comm.host = host;
		comm.port = port;
	}
	
	[[UIApplication sharedApplication] registerUserNotificationSettings: [UIUserNotificationSettings settingsForTypes: (UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert) categories:nil]];
	
	
	if (verifyFp) {
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
	
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings
{
    //register to receive notifications
    [application registerForRemoteNotifications];
}

- (void)application:(UIApplication *)application handleActionWithIdentifier:(NSString *)identifier forRemoteNotification:(NSDictionary *)userInfo completionHandler:(void(^)())completionHandler
{
    //handle the actions
    if ([identifier isEqualToString:@"declineAction"]){
    }
    else if ([identifier isEqualToString:@"answerAction"]){
    }
}

- (void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
	NSLog(@"Token: %@", deviceToken);
	[Communicator defaultCommunicator].token = deviceToken;
	SimplePacket sp((const uint8_t*)[deviceToken bytes], [deviceToken length], Type::DEVICE_TOKEN);
	[[Communicator defaultCommunicator] writePacket: &sp target:nil withSelector:nil];
}

- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
	NSLog(@"Failed to get token, error: %@", error);
}

-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
	NSLog(@"Received notification.");
	UITabBarController *tabBarController = (UITabBarController*)self.window.rootViewController;
	
	tabBarController.selectedIndex = 2;
}

@end
