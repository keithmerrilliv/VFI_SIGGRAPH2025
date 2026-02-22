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


#import <GameController/GameController.h>

@class NSKVirtualGamepad;

#if TARGET_OS_IOS
@class NSKGamepadView;
#endif

@interface NSKVirtualController : GCController

#if TARGET_OS_IOS
@property (nonatomic, readonly) NSKGamepadView *gamepadView;
#endif

//! The virtualGamepad property provides access to additional capabilities of the virtual controller beyond the extended gamepad profile. However, extendedGamepad is also available.
@property (nonatomic, readonly) NSKVirtualGamepad *virtualGamepad;

@end

@interface NSKVirtualGamepad : GCExtendedGamepad

@property (nonatomic, readonly) GCControllerButtonInput *backButton;
@property (nonatomic, readonly) GCControllerButtonInput *startButton;

//! 3D touch on left thumbstick
@property (nonatomic, readonly) GCControllerButtonInput *leftThumbstickButton;

//! 3D touch on right thumbstick
@property (nonatomic, readonly) GCControllerButtonInput *rightThumbstickButton;

@end

