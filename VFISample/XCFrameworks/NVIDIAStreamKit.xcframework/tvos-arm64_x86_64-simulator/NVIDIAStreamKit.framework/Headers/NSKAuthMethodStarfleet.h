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
#import <AuthenticationServices/AuthenticationServices.h>

#import <NVIDIAStreamKit/NSKAuthMethod.h>

@class NSKAuthMethodStarfleet;

/// Presents an off-device login screen to the user.
@protocol NSKStarfleetOffDeviceURLDelegate <NSObject>
@required

//! Called by Starfleet off-device login upon resolving the URL that the user is to be directed to load in a web browser, and a code to enter on the page.
//!
//! The delegate is expected to present this information to the user (e.g. via a QR code as well as a written out URL) so that they can complete login on a separate device with a web browser (e.g. Phone, Tablet, or Computer).
//!
//! - parameter loginURL: the full URL including the code as a query parameter, can be used in a QR code.
//! - parameter loginShortURL: the short login URL in an easy to type form, presented as text to the user.
//! - parameter loginCode: the code the user will be prompted to enter into their web browser
//!
//! - returns: YES if the off-device login details were successfully shown to the user. Returning NO cancels the off-device login process.
- (BOOL)starfleetLoginResolvedOffDeviceURL:(NSURL *)loginURL
                                  shortURL:(NSURL *)loginShortURL
                                      code:(NSString *)loginCode;

@end

/// Configuration for a Starfleet login session.
@interface NSKStarfleetLoginRequest : NSObject

//! Hostname of Starfleet auth server.
@property (copy) NSString *host;

//! Client ID for the application requesting Starfleet login.
//! Note that there are separate clientIDs for platforms that have web browsers capable of on device login (iOS, iPadOS, macOS) vs platforms that use off device login (tvOS), even if it's nominally the same "application."
@property (copy) NSString *clientID;

//! Custom URL handler scheme used for Starfleet redirection.
//! The Starfleet application corresponding to the clientID must be configured to contain "\<url-scheme\>:" as one of its redirect targets.
//! This property is required for platforms that have web browsers capable of on device login (iOS, iPadOS, macOS). It is not used on tvOS.
@property (copy) NSString *redirectionURLScheme;

//! Optional previous Starfleet object. Can be used to prime a re-login request.
@property (strong) NSKAuthMethodStarfleet *previousStarfleetSession;

//! Presentation context for the ASWebAuthenticationSession.
//! This property is required for platforms that have web browsers capable of on device login (iOS, iPadOS, macOS), it is not used on tvOS.
@property (weak) id<ASWebAuthenticationPresentationContextProviding> presentationContextProvider API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(tvos, watchos);

//! Delegate capable of presenting off device login URL and code to the user. Only used on tvOS for the off-device login flow.
@property (weak) id<NSKStarfleetOffDeviceURLDelegate> offDeviceURLDelegate;

@end

//! Represents a user login session with NVIDIA's Starfleet authentication service.
//!
//! See the [Starfleet documentation](https://gitlab-master.nvidia.com/kaizen/auth/starfleet/sf-docs) for more info.
@interface NSKAuthMethodStarfleet : NSObject <NSKAuthMethod, NSSecureCoding>

//! Current account stored the keychain/user defaults. Will be nil if not logged in.
//!
//! Note that having a currentAccount doesn't mean it will be valid because it can be invalidated remotely at any time.
+ (NSKAuthMethodStarfleet *)currentAccount;

//! Perform login using the Starfleet configuration in request.
//! - parameter request: used to configure the Starfleet login process.
//! - parameter completionHandler: called asynchronously on the main thread when the login process completes, fails, or is cancelled.
//! - returns: An indeterminate NSProgress object that may be used the cancel the login operation.
+ (NSProgress *)performLoginWithRequest:(NSKStarfleetLoginRequest *)request
                      completionHandler:(void (^)(NSKAuthMethodStarfleet *starfleet, NSError *error))completionHandler;

//! Asynchronously fetch a JWT.
//! - parameter handler: Completion block. Will be called on an arbitrary background queue.
- (void)accessTokenWithCompletionHandler:(void (^)(NSString *accessToken, NSError *error))handler;

//! Log out of Starfleet and remove any associated item from the Keychain/user defaults.
//! - parameter handler: block to be called when logout is completed. Will be called on an arbitrary background queue.
- (void)logoutWithCompletionHandler:(void (^)(NSError *error))handler;

//! YES if this instance represents the currently logged in Starfleet account, NO otherwise.
@property (readonly, getter=isCurrent) BOOL current;

//! Store this account in the keychain or user defaults and update +currentAccount to point to this instance.
- (void)becomeCurrent;

#pragma mark - User Info
//! The Starfleet user identifier.
@property (readonly) NSString *userId;
//! The Starfleet preferred username.
@property (readonly) NSString *preferredUsername;
//! The time when user info was last fetched by the client for this account (not the time when it was last updated on the server).
@property (readonly) NSDate *lastUpdatedUserInfo;

//! The Starfleet identity provider identifier.
@property (readonly) NSString *idpID;
//! The Starfleet identity provider name.
@property (readonly) NSString *idpName;

//! - returns: YES if the user account has enabled technical consent for telemetry data uploads.
@property (readonly) BOOL hasTelemetryTechnicalConsent;

//! - returns: YES if the user account has enabled behavioral consent for telemetry data uploads.
@property (readonly) BOOL hasTelemetryBehavioralConsent;

//! Asynchronously update the account's user info
//! - parameter completion: called on an arbitrary background queue when the operation concludes, either successfully or with an error.
- (void)updateUserInfoWithCompletion:(void (^)(NSError *error))completion;

@end

/// NSError domain for errors from the Starfleet subsystem.
extern NSString *const NSKAuthMethodStarfleetErrorDomain;

// NSError codes for errors from the Starfleet subsystem.
typedef NS_ENUM(NSInteger, NSKAuthMethodStarfleetErrorCode) {
    //! error with ASWebAuthenticationSession
    NSKAuthMethodStarfleetErrorCodeAuthSession = 1,
    //! invalid NSKStarfleetLoginRequest configuration
    NSKAuthMethodStarfleetErrorCodeRequest,
    //! error communicating with server (unexpected/no response)
    NSKAuthMethodStarfleetErrorCodeServer,
    //! Login expired, user must perform log in process again
    NSKAuthMethodStarfleetErrorCodeTokenExpired
};

/// Posted on main queue when ``NSKAuthMethodStarfleet/currentAccount`` changes.
extern NSString *const NSKAuthMethodStarfleetCurrentAccountDidChangeNotification;
