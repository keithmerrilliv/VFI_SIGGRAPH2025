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

#import <NVIDIAStreamKit/NSKAuthMethod.h>

extern NSString *const NSKAuthMethodJarvisErrorDomain;

typedef NS_ENUM(NSInteger, NSKAuthMethodJarvisErrorCode) {
    NSKAuthMethodJarvisErrorCodeCredentials = 1, // credentials are invalid (must log in again)
    NSKAuthMethodJarvisErrorCodeServer = 2, // error communicating with server (unexpected/no response)
};

typedef NS_ENUM(NSInteger, NSKAuthMethodJarvisDeviceType) {
    NSKAuthMethodJarvisDeviceTypeUnknown = 0,
    NSKAuthMethodJarvisDeviceTypeShield = 1,
    NSKAuthMethodJarvisDeviceTypeMacOSX = 2,
    NSKAuthMethodJarvisDeviceTypeWindows = 3,
    NSKAuthMethodJarvisDeviceTypeLinux = 4,
    NSKAuthMethodJarvisDeviceTypeChromium = 5,
    NSKAuthMethodJarvisDeviceTypeGFEWindows = 6,
    NSKAuthMethodJarvisDeviceTypeiOS = 7,
    NSKAuthMethodJarvisDeviceTypetvOS = 8,
};

extern NSString *const NSKAuthMethodJarvisCurrentAccountDidChangeNotification; // posted on main queue when +currentAccount changes

@interface NSKAuthMethodJarvis : NSObject <NSKAuthMethod>

//! Current account stored the keychain/user defaults. Will be nil if not logged in.
//! \note Having a currentAccount doesn't mean it is will be valid because it can be invalidated remotely at any time.
+ (NSKAuthMethodJarvis *)currentAccount;

//! Asynchronously authenticate with the provided OAuth code.
//! If authentication succeeds, the new jarvis object will be stored in the keychain and returned via the completion block.
//! If authentication fails, an error will be returned via the completion block.
//! @param code OAuth token as returned to OAuth resource redirect (see +parseOAuthRedirectURL:code:)
//! @param completion block to be called on an arbitrary background queue.
+ (void)authenticateWithOAuthCode:(NSString *)code completion:(void (^)(NSKAuthMethodJarvis *jarvis, NSError *error))completion;

//! URL to load in a Web View to begin Jarvis OAuth login process.
//! @param preferredAuthenticators an ordered list of authenticators. Those not specified will be hidden behind a "Show more" link.
+ (NSURL *)beginURLForOAuthWithPreferredAuthenticators:(NSArray<NSString *> *)preferredAuthenticators;

//! Parse an OAuth redirect URL to extract the OAuth login code.
//! @param URL Redirect URL
//! @param outCode Extracted code. Will be set to nil if URL cannot be parsed.
//! @return YES if URL is an OAuth redirect URL with a code, NO otherwise.
+ (BOOL)parseOAuthRedirectURL:(NSURL *)URL code:(NSString *__autoreleasing*)outCode;

//! Perform off-device authentication for platforms lacking a web browser (tvOS).
//! @param codeHandler Delivers the URL for the user to navigate to with their computer / phone and the code to enter there.
//! @param completion After the user has logged in off-device completion will be called with the logged in jarvis instance, which will be stored in the keychain. If an error occurs, it will be provided here.
//! @return An indeterminate NSProgress. This object may be used to cancel the operation.
//! @note completion will be called with an error and codeHandler may never be called if there is an error fetching the initial code.
+ (NSProgress *)performOffDeviceOAuthWithCodeHandler:(void (^)(NSURL *offdeviceURL, NSString *code))codeHandler completion:(void (^)(NSKAuthMethodJarvis *jarvis, NSError *error))completion;

//! Asynchronously fetch a JSON formatted Jarvis token.
//! @param handler Completion block. Will be called on an arbitrary background queue.
- (void)accessTokenWithCompletionHandler:(void (^)(NSString *accessToken, NSError *error))handler;

//! Log out of Jarvis and remove any associated item from the Keychain.
//! @param handler block to be called when logout is completed. Will be called on an arbitrary background queue.
- (void)logoutWithCompletionHandler:(void (^)(NSError *error))handler;

//! YES if this instance represents the currently logged in jarvis account, NO otherwise.
@property (readonly, getter=isCurrent) BOOL current;

//! Store this jarvis in the keychain and update +currentAccount to point to this instance.
- (void)becomeCurrent;

//! If !emailVerified, resend a verification email to the account email.
//! param handler block to be called when operation completes. Will be called on an arbitrary background queue.
- (void)resendEmailVerificationWithCompletionHandler:(void (^)(NSError *error))handler;

- (void)updateUserInfoWithCompletionHandler:(void (^)(NSError *error))handler;

//! Register device token of push notification to Jarvis shared store
- (void)registerDeviceToken:(NSData *)token withCompletionHandler:(void (^)(NSError *error))handler;

@property (nonatomic, readonly) NSString *userId;
@property (nonatomic, readonly) NSString *email;
@property (nonatomic, readonly, getter=isEmailVerified) BOOL emailVerified;
@property (nonatomic, readonly) NSString *displayName;
@property (nonatomic, readonly) NSArray<NSString *> *loginHintValues;

@property (nonatomic, readonly, getter=isDeviceTokenRegistered) BOOL deviceTokenRegistered;

@end

@interface NSMutableURLRequest (NSKAuthMethodJarvisExtensions)

- (void)nsk_addJarvisAuthorizationHeaderWithAccessToken:(NSString *)jarvisAuthToken;

@end

@interface NSKAuthMethodJarvis (HTTPResponse)

//! If response is an HTTP 401 error, returns an appropriate NSKAuthMethodJarvisErrorDomain error. Otherwise returns nil
+ (NSError *)authErrorWithResponse:(NSURLResponse *)response;

@end

@interface NSKAuthMethodJarvis (NSKTesting)

//! NSUserDefaults implementation for testing.
//! Set JarvisEmail and JarvisPassword in defaults (either via command line or other methods).
+ (NSKAuthMethodJarvis *)authMethodFromUserDefaults;

//! Asynchronously authenticate with the provided Nvidia IDM credentials.
//! If authentication succeeds, the new jarvis object will be stored in the keychain and returned via the completion block.
//! If authentication fails, an error will be returned via the completion block.
//! @param email Nvidia IDM account email
//! @param password Nvidia IDM account password
//! @param completion block to be called on an arbitrary background queue.
//! @note This method is for internal testing only! End-user authentication should be done through one of the OAuth methods.
+ (void)authenticateWithEmail:(NSString *)email password:(NSString *)password completion:(void (^)(NSKAuthMethodJarvis *jarvis, NSError *error))completion;

@end
