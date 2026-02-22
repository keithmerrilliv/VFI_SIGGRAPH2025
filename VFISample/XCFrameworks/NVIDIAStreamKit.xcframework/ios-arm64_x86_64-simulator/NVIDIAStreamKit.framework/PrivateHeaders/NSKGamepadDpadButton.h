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

#import <NVIDIAStreamKit/NSKGamepadButton.h>

typedef NS_ENUM(NSUInteger, DpadDirection) {
    DpadUp,
    DpadDown,
    DpadLeft,
    DpadRight,
};

IB_DESIGNABLE
@interface NSKGamepadDpadButton : NSKGamepadButton

@property (nonatomic, assign) IBInspectable NSUInteger direction;

@end

#endif // TARGET_OS_IOS
