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

@class NSKVideoFrame;

#if TARGET_OS_IPHONE
@interface NSKStreamVideoView : UIView
#else
@interface NSKStreamVideoView : NSView
#endif

//! Display frame. OK to call from any thread.
- (void)renderFrame:(NSKVideoFrame *)frame;

//! Flush and discard any pending frames. Remove any displayed image if removeImage is set to YES.
- (void)flushAndRemoveImage:(BOOL)removeImage;

//! Bounds of the displayed video in the coordinate space of the view
@property (readonly) CGRect videoRect;

//! Set to YES to enable luma averaging for the dark property. Default NO.
@property BOOL processesImage;

//! Set to YES to expect and decode YUVxA frames (YUV frames with alpha). Default NO.
@property (nonatomic) BOOL decodeYUVxA;

//! YES if the average color of the video image is dark, NO if it is light. KVO.
//  \note Will only be updated if processesImage is set to YES.
@property (nonatomic, readonly, getter=isDark) BOOL dark;

@end
