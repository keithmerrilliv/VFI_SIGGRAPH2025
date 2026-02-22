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

@interface NSUserDefaults (NSKDefaults)

//! Returns the object for key as set by [NSKDefaults registerDefaults], or nil if key was not put in NSRegistrationDomain.
- (id)nsk_registrationDomainObjectForKey:(NSString *)key;

@end

@interface NSKDefaults : NSUserDefaults

+ (void)registerDefaults;
+ (void)registerStagingDefaults;

@end

extern NSString *const NSKStarfleetHostKey;
extern NSString *const NSKStarfleetClientIDKey;
extern NSString *const NSKStarfleetRedirectionSchemeKey;
extern NSString *const NSKJarvisHostKey;
extern NSString *const NSKServerInfoHostKey;
extern NSString *const NSKLoginProviderKey;
extern NSString *const NSKLayoutHostKey;
extern NSString *const NSKAppsHostKey;
extern NSString *const NSKTelemetryHostKey;
extern NSString *const NSKTelemetrySessionLogUploadURLKey;
extern NSString *const NSKFeedbackHostKey;
extern NSString *const NSKRemoteConfigHostKey;
extern NSString *const NSKServiceURLsHostKey;
extern NSString *const NSKAccountLinkingServiceHostKey;

extern NSString *const NSKOverrideZoneHostKey;
extern NSString *const NSKInternalOverrideZoneHostKey; // => Internal-only setting. Takes precedence over NSKOverrideZoneHostKey and need not be in the server info's list of regions.
extern NSString *const NSKStreamingQualityKey; // => Integer NSKSettingsQuality

extern NSString *const NSKStreamWWANKey; // => BOOL

extern NSString *const NSKControlsMenuDelayKey;

extern NSString *const NSKFirstLaunchKey; // => BOOL (defaults to YES)

extern NSString *const NSKNetworkTestFingerprintKey;
extern NSString *const NSKNetworkTestZoneKey;
extern NSString *const NSKNetworkTestZoneAddressKey;
extern NSString *const NSKNetworkTestZoneNameKey;

extern NSString *const NSKStreamCaptureBitstreamKey; // => BOOL. Internal-only setting.
extern NSString *const NSKStreamCaptureAudioKey; // => BOOL. Internal-only setting.
extern NSString *const NSKStreamClientConfigKey; // => NSString. Internal-only setting.

extern NSString *const NSKContentRestrictionRatingKey;  // => NSNumber
extern NSString *const NSKContentRestrictionRatingTitleKey; // => NSString
extern NSString *const NSKContentRestrictionShowRestrictedGameKey; // => BOOL (defaults to NO)
extern NSString *const NSKContentRestrictionPinCodeKey;

extern NSString *const NSKRtspDebugTelemetryEnabledKey; // => BOOL (defaults to NO)
