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

@class NSKStreamVideoView;

@interface NSKStreamVideoScrollView : UIView

@property (readonly) NSKStreamVideoView *videoView;

@property (nonatomic, getter=isZoomEnabled) BOOL zoomEnabled;

//! Reset zoom scale to 1.0 and position video view centered aspect fit. Animatable.
- (void)resetScrollAndZoom;

@end

#endif // TARGET_OS_IPHONE
