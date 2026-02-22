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


#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

#if TARGET_OS_IPHONE
#define NSUIColor UIColor
#else
#define NSUIColor NSColor
#endif

@interface NSUIColor (NSK)

+ (NSUIColor *)nvidiaGreen;

+ (NSUIColor *)nsk_colorWithUInt8Red:(UInt8)red green:(UInt8)green blue:(UInt8)blue alpha:(UInt8)alpha;

#if TARGET_OS_IOS
+ (UIColor *)nsk_honeycombPattern;
#endif

@end

