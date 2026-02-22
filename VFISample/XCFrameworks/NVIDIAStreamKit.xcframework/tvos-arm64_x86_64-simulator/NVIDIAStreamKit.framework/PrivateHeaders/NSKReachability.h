/*
 * Copyright (c) 2022 NVIDIA CORPORATION.  All Rights Reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 *
 */


#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, NSKReachabilityType) {
	NSKReachabilityTypeNone = 0,
	NSKReachabilityTypeWiFi,
	NSKReachabilityTypeWWAN,
    NSKReachabilityTypeEthernet
};

extern NSString *const NSKReachabilityChangedNotification;

@interface NSKReachability : NSObject

+ (instancetype)sharedReachability;

@property (readonly) NSKReachabilityType reachabilityType;

//! Returns YES if network is reachable, but only via an IPV6 route (no IPV4 connectivity).
@property (readonly) BOOL routeIsIPV6Only;

- (void)beginGeneratingNotifications;

//! Returns YES if this device has a cellular radio.
//  \note that this does not necessarily mean that it is enabled or in-use, simply whether or not it is present at all.
+ (BOOL)deviceHasCellularInterface;

@end

