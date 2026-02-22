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
typedef void (^NSKImageCacheHandler)(UIImage *image, NSError *error);
#else
#import <Cocoa/Cocoa.h>
typedef void (^NSKImageCacheHandler)(NSImage *image, NSError *error);
#endif


@interface NSKImageCache : NSObject

+ (instancetype)sharedCache;

//! Asynchronously load image located at URL
//! Returns a progress object which can be used to cancel the request prior to its completion.
//! \param size the image size to request. Width or height can be 0, which means the image is unconstrained in that dimension. Specifying CGSizeZero will return the natural size of the image.
//! \param handler will be invoked on the main queue.
//! \note Make sure that you are consistent in your choice of size and scale between prefetching and loading the corresponding image. If you are not, the preload will be wasted. If you can't be sure what size you'll need, it is better to specify CGSizeZero for both the preload and the load.
- (NSProgress *)imageForURL:(NSURL *)URL size:(CGSize)size scale:(CGFloat)scale completionHandler:(NSKImageCacheHandler)handler;

- (void)prefetchImageForURL:(NSURL *)URL size:(CGSize)size scale:(CGFloat)scale;

@end
