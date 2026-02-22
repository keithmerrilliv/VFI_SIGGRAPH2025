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

//! /return a user agent string for HTTP/S header.
extern NSString *NSKUserAgent(void);

//! /return the user agent string for the given platform
extern NSString *NSKUserAgentForPlatform(NSString *platformIdentifier);

@interface NSMutableURLRequest (NSKUserAgentExtra)

//! Add User-Agent with the current platform
- (void)nsk_addUserAgentHeader;

//! Add User-Agent with the specified platform.
//  Used as a workaround for overriding the platform for endpoints which are not updated to know about all platforms yet.
- (void)nsk_addUserAgentHeaderForPlatform:(NSString *)platformIdentifier;

@end
