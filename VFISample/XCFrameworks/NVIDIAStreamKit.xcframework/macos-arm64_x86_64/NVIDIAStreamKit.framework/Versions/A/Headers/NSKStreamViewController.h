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
#import <GameController/GameController.h>

#import <NVIDIAStreamKit/NSKStreamSessionPolicy.h>
#import <NVIDIAStreamKit/NSKStreamLoadingViewController.h>

@class NSKStreamClient;
@class NSKStreamConfiguration;

/**
 NSKStreamViewController is a UIViewController subclass that displays streaming video,
 forwards user input to the remote system, and can present a standard user interface
 for controlling the streaming session.
 */
@interface NSKStreamViewController : GCEventViewController

/**
 Initialize the stream view controller with a streaming configuration object.

 The ``streamClient`` property will be initialized from the configuration.
 This initializer is recommended when using the integrated or custom loading view controller.
*/
- (instancetype)initWithConfiguration:(NSKStreamConfiguration *)configuration;

/**
 Initialize the stream view controller with an existing stream client

 The view controller will be configured to match the current state of the streamClient.
 This initializer is recommended when session loading progress is shown prior to presenting the stream view controller.
*/
- (instancetype)initWithStreamClient:(NSKStreamClient *)streamClient;

/// The streaming configuration this instance was initialized with.
@property (readonly) NSKStreamConfiguration *configuration;

/**
 The underlying stream client controlling the streaming session.

 NSKStreamViewController users should add a state observer to the stream client to learn when the streaming session
 starts or stops in order to dismiss the view controller.
*/
@property (readonly) NSKStreamClient *streamClient;

#if TARGET_OS_IPHONE
/// Add subviews to this view to add content above the streamed video, but below any stream controls
@property (nonatomic, readonly) UIView *overlayContentView;
#else
@property (nonatomic, readonly) NSView *overlayContentView;
#endif

#if TARGET_OS_IPHONE
/**
 Add subviews to this view to add content beneath the streamed video.

 These views will only be visible if the video is streamed with per-pixel-alpha.
 See ``NSKStreamConfiguration/enableStreamAlphaChannel``
*/
@property (nonatomic, readonly) UIView *underlayContentView;
#else
@property (nonatomic, readonly) NSView *underlayContentView;
#endif

#if TARGET_OS_IPHONE
/**
 Set/access the loading view controller. Defaults to an instance of NSKStreamLoadingViewController.

 You can replace the default loading screen with any view controller that conforms to the
 ``NSKStreamLoadingViewController-c.protocol`` protocol if you want to provide a custom loading experience.

 The loading view controller is automatically presented by the stream view controller as needed.
*/
@property (nonatomic, strong) UIViewController<NSKStreamLoadingViewController> *loadingViewController;
#else
@property (nonatomic, strong) NSViewController<NSKStreamLoadingViewController> *loadingViewController;
#endif

@end
