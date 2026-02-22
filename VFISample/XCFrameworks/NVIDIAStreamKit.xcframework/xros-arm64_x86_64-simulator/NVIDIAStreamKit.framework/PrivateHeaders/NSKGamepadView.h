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
#import <NVIDIAStreamKit/NSKJoystickView.h>
#import <NVIDIAStreamKit/NSKGamepadButton.h>

@class NSKVirtualGamepad;

typedef NS_ENUM(NSUInteger, NSKGamepadViewMode) {
    NSKGamepadViewModeComplete = 0, //!< Shows every control
    NSKGamepadViewModeStartBackL3R3 = 1, //!< Shows just the dismiss button, start, back, l3 and r3
    NSKGamepadViewModeStartBack = 2 //!< Shows just the dismiss button, start, and back
};

@interface NSKGamepadView : UIView <NSKJoystickViewDelegate>

@property (weak, nonatomic) IBOutlet NSKJoystickView *joystickLeft;
@property (weak, nonatomic) IBOutlet NSKJoystickView *joystickRight;

@property (nonatomic, weak) NSKVirtualGamepad *virtualGamepad;

@property (copy) dispatch_block_t dismissGamepadHandler;

@property (nonatomic) NSKGamepadViewMode viewMode;

@end

#endif // TARGET_OS_IOS
