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

@interface UIImage (NSKCoreAssets)

+ (UIImage *)nsk_honeycombImage;
+ (UIImage *)nsk_defaultGameArtBoxArtImage;
+ (UIImage *)nsk_defaultGameArtFeatureImage;
+ (UIImage *)nsk_defaultGameArtTVBannerImage;

@end

#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>

@interface NSImage (NSKCoreAssets)

+ (NSImage *)nsk_honeycombImage;
+ (NSImage *)nsk_defaultGameArtBoxArtImage;
+ (NSImage *)nsk_defaultGameArtFeatureImage;
+ (NSImage *)nsk_defaultGameArtTVBannerImage;

@end

#endif
