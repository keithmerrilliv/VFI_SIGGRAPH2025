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


#if TARGET_OS_IOS

#import <UIKit/UIKit.h>

@class NSKJoystickView;

@protocol NSKJoystickViewDelegate

- (void)joystickView:(NSKJoystickView *)view didChange:(CGPoint)point;
- (void)joystickView:(NSKJoystickView *)view didPressed:(BOOL)isDown;

@end

IB_DESIGNABLE
@interface NSKJoystickView : UIView

@property (readonly, nonatomic) CGPoint velocity;
@property (readonly, nonatomic) CGFloat angularVelocity;

@property (weak, nonatomic) id<NSKJoystickViewDelegate> delegate;

@end

#endif // TARGET_OS_IOS
