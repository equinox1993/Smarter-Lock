//
//  ZxingBridgeTest.m
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/23.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

//#include <string>

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ZxingBridge.h"

@interface ZxingBridgeTest : XCTestCase

@end

@implementation ZxingBridgeTest

- (void)setUp {
    [super setUp];
	
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testImageDecode {
	cv::Mat mat = cv::imread("/Users/Edwin/Desktop/qr.jpg", CV_LOAD_IMAGE_COLOR);
	XCTAssert(mat.data);
	
	printf("%s\n", ZxingBridge::decode(mat).c_str());
}

//- (void)testExample {
//    // This is an example of a functional test case.
//    XCTAssert(YES, @"Pass");
//}

//- (void)testPerformanceExample {
//    // This is an example of a performance test case.
//    [self measureBlock:^{
//        // Put the code you want to measure the time of here.
//    }];
//}

@end
