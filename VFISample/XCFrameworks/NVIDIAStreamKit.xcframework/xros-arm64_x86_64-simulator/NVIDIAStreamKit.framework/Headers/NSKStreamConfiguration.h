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
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

#if TARGET_OS_IPHONE
@class UIScreen;
#endif

#if TARGET_OS_OSX
@class NSScreen;
#endif

@protocol NSKAuthMethod;
@class NSKAuthMethodStarfleet;
@class NSKAuthMethodGuest;

/// Customizes the remote user interface according to an input device profile.
/// See ``NSKStreamConfiguration/inputLaunchMode``
typedef NS_ENUM(NSInteger, NSKStreamInputLaunchMode) {
    NSKStreamInputLaunchModeDefault = 0,
    /// The primary input device will be a gamepad (or virtual gamepad).
    NSKStreamInputLaunchModeGamepad,
    /// The primary input device will be touch. Touch events will not be synthesized to mouse events, but instead will be sent unmodified to the server.
    NSKStreamInputLaunchModeTouch,
    /// The primary input device will be keyboard and mouse (or virtual keyboard and touches interpreted as mouse).
    ///
    /// Applications wishing to use keyboard and mouse while streaming on iPadOS must set the Info.plist key `UIApplicationSupportsIndirectInputEvents` to true.
    NSKStreamInputLaunchModeKeyboardAndMouse = NSKStreamInputLaunchModeDefault
};

@class NSKStreamConfiguration;

typedef void (^NSKStreamConfigurationLoadCompletion)(NSKStreamConfiguration * _Nullable config, NSError * _Nullable error);

/** Represents the settings for a streaming session. */
@interface NSKStreamConfiguration : NSObject

/**
 Asynchronously load the recommended streaming configuration for the provided user account.

 This method makes network requests to query service information.

 On success, this method will provide a `NSKStreamConfiguration` object to the completion handler
 with the following properties already configured:

 - ``serverURL``: Set to the URL of the recommended streaming zone for the user and location.
 - ``authMethod``: Set to the value of the `starfleet` parameter.
 - ``inputLaunchMode``: Set according to the device type.
 - ``hdr``: Set according to the user's subscription level and system and screen capabilities.
 - ``preferredStreamingSize``: Set according to the user's subscription level and system and screen capabilities.
 - ``maxFPS``: Set according to the user's subscription level and system and screen capabilities.

 - parameters:
    - starfleet: The starfleet object for the logged-in user (required)
    - screen: The screen that the stream is intended for (set to nil to use the main screen).
    - completion: Called asynchronously on a background queue with the recommended configuration or an NSError object.

 - returns: An indeterminate NSProgress object that can be used to cancel the operation.
 */
+ (NSProgress *)loadDefaultConfigurationForStarfleetAccount:(nonnull NSKAuthMethodStarfleet *)starfleet
                                                 completion:(nonnull NSKStreamConfigurationLoadCompletion)completion;

/**
 Asynchronously load the recommended streaming configuration for the provided Guest Mode session.

 This method makes network requests to query service information.

 On success, this method will provide a `NSKStreamConfiguration` object to the completion handler
 with the following properties already configured:

 - ``serverURL``: Set to the URL of the recommended streaming zone for the user and location.
 - ``authMethod``: Set to the value of the `guestMode` parameter.
 - ``inputLaunchMode``: Set according to the device type.
 - ``hdr``: Set according to the service level and system and screen capabilities.
 - ``preferredStreamingSize``: Set according to the service available resolutions and system and screen capabilities.
 - ``maxFPS``: Set according to the service level and system and scren capabilities.
 - ``appId``: Set according to the cms identifier of the guest mode session.

 - parameters:
    - guestMode: The guest mode authentication object session (required)
    - completion: Called asynchronously on a background queue with the recommended configuration or an NSError object.

 - returns: An indeterminate NSProgress object that can be used to cancel the operation.
 */
+ (NSProgress *)loadDefaultConfigurationForGuestMode:(nonnull NSKAuthMethodGuest *)guestMode
                                          completion:(nonnull NSKStreamConfigurationLoadCompletion)completion;

/** Initialize the stream configuration with the recommended streaming settings for the device and current network configuration. */
- (instancetype)init;

#if TARGET_OS_IPHONE && !TARGET_OS_VISION
/** Configure with recommended stream settings for a given screen. */
- (void)configureWithScreen:(UIScreen *)screen;
#endif

#if TARGET_OS_OSX
- (void)configureWithScreen:(NSScreen *)screen;
#endif

/** The URL of the streaming server. */
@property (nullable, copy) NSURL *serverURL;

/** Credentials for the streaming server. */
@property (nullable, strong) id<NSKAuthMethod> authMethod;

/** The remote application ID to launch on the streaming seat. */
@property NSUInteger appId;

/** The NVIDIA client application identifier.

 - note: this is assigned by NVIDIA and is typically not the application bundle identifier.
 */
@property (copy) NSString *clientIdentifier;

/** The NVIDIA client version number.

 - note: this is assigned by NVIDIA for the protocol version, and is not the application version.
 */
@property (copy) NSString *clientVersion;

/**
 If the user has linked the remote application's platform login with their NVIDIA account, setting this option
 can bypass the requirement to re-enter credentials in the stream during remote application launch.

 Defaults to YES, but launch time will be slightly shorter if set to NO and the account is known to not be linked.
 */
@property BOOL useLinkedPlatformAccount;

/**
 The user-visible name of the application to launch.

 This property is not required, and is used only for logging and display purposes.
*/
@property (nullable, copy) NSString *appName;

/**
 The input mode to prefer at remote application launch time.

 This setting affects the appearance and behavior of the remote application
 (e.g. larger buttons with additional spacing in touch mode, or focus driven navigation in gamepad mode).

 It does not, however, preclude the use of other input types (e.g. using a physical gamepad is still
 possible in touch or keyboard and mouse modes).
*/
@property NSKStreamInputLaunchMode inputLaunchMode;

/**
 Request a stream with High Dynamic Range color.

 Defaults to YES on systems capable of HDR streaming, NO otherwise.

 - note: Not all appIds and user subscription levels are provisioned for HDR streaming.\
         If the app or user's subscripton level is insufficient, streaming will fall back to SDR.
 */
@property BOOL hdr;

/**
 Configures the session with an XR profile if YES.

 Defaults to NO.
 */
@property BOOL xrProfile;

/**
 Set the preferred resolution of the video stream.

 Arbitrary resolutions are not available, instead the server will choose the closest supported resolution.
*/
@property CGSize preferredStreamingSize;

/**
 Limit the maximum streaming bitrate in kilobits per second.

 Default is nil, which means means choose a reasonable bitrate for the resolution and frame rate chosen.
*/
@property (nullable) NSNumber *maxBitrateKbps;

/**
 Set the maximum streaming framerate.

 - For lowest streaming latency, set this to the refresh rate of the display the stream will be presented on.
 - For best picture quality with low bitrates (e.g. on mobile networks), set this to 30fps.

 The default is the refresh rate of the screen.

 - note: Not all appIds and user subscription levels support all streaming framerates. The server
         will choose the closest rate available.
*/
@property (nullable) NSNumber *maxFPS;

/**
 Set the number of video streams requested by the client.

 The default is 1.
*/
@property (nullable) NSNumber *streamCount;

/**
 Set to YES to allow streaming over cellular networks.

 When set to NO, attempts to connect a streaming session over the cellular WWAN interface will fail with an error.

 Defaults to the value read from NSUserDefaults `NSKStreamWWANKey`.
*/
@property BOOL allowsCellularAccess;

/**
 Set to YES to request a stream with a per-pixel alpha channel. Defaults to NO.

 See ``NSKStreamViewController/underlayContentView`` to provide a view for compositing.
 */
@property BOOL enableStreamAlphaChannel;

/**
 Set to YES to enable automatic client telemetry and log uploads to NVIDIA. Defaults to NO.

 This property must only be set to YES if the user has granted technical consent for technical telemetry data to be
 uploaded to NVIDIA.

 Even if user technical consent is provided, it still may be useful to leave this property set to NO if you
 want to handle telemetry or log uploads via another method.

 - seealso: ``NSKAuthMethodStarfleet/hasTelemetryTechnicalConsent``
 */
@property BOOL hasTelemetryTechnicalConsent;

/**
 Set key/value pairs of data to be included with the session setup request.

 Typically these are left unset, but can be used to configure launch parameters or environment
 for the remote application.
 */
@property (nullable) NSDictionary<NSString *, NSString *> *launchMetadata;

/**
 Set HTTP header key/value pairs of data to be included with the session setup request.

 Typically these are left unset, but can be used to control special client properties or launch parameters.

 Note that these are distinct from launchMetadata, which is included in the body of the session setup request.
 */
@property (nullable) NSDictionary<NSString *, NSString *> *sessionSetupHTTPHeaders;

@end

NS_ASSUME_NONNULL_END
