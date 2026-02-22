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
#endif

#if TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#endif

@protocol NSKStreamLoadingViewControllerDelegate;

/// Custom NSKStreamLoadingViewControllers can implement just this protocol
/// if they want to subclass something other than NSKStreamLoadingViewController
@protocol NSKStreamLoadingViewController <NSObject>
@required
@property (nonatomic, weak) id<NSKStreamLoadingViewControllerDelegate> delegate;

//! Display the stream loading progress and queue position.
- (void)updateProgressEstimate:(NSTimeInterval)timeRemaining queuePosition:(NSInteger)queuePosition;

@end

#if TARGET_OS_IPHONE
/// A View Controller presented by ``NSKStreamViewController`` during streaming session connection and re-connection.
@interface NSKStreamLoadingViewController : UIViewController <NSKStreamLoadingViewController>
#else
@interface NSKStreamLoadingViewController : NSViewController <NSKStreamLoadingViewController>
#endif

/// Set by the ``NSKStreamViewController``. Use the delegate to inform the stream view controller of a user request
/// to cancel session setup.
@property (nonatomic, weak) id<NSKStreamLoadingViewControllerDelegate> delegate;

- (void)updateProgressEstimate:(NSTimeInterval)timeRemaining queuePosition:(NSInteger)queuePosition;

#if TARGET_OS_IPHONE
//! Set loading screen artwork image URLs.
- (void)setLoadingImageURLLandscape:(NSURL *)landscapeURL portrait:(NSURL *)portraitURL;
//! Set a rating image, title, and description (optional)
- (void)setLoadingRating:(NSURL *)ratingImageURL ratingTitle:(NSString *)ratingTitle ratingDescription:(NSString *)ratingDescription;

//! Set a series of tips (as localized strings) to be displayed to the user during loading.
//! Tips are randomly chosen and rotated as loading progresses.
@property (nonatomic, copy) NSArray<NSString *> *loadingTips;
#endif

#if TARGET_OS_OSX
//! Set loading screen artwork image URL.
- (void)setLoadingImageURL:(NSURL *)URL;
#endif

@end

@protocol NSKStreamLoadingViewControllerDelegate <NSObject>

/// The loading view controller should call this method to inform its corresponding ``NSKStreamViewController``
/// that the user wishes to cancel session (re)connection.
- (void)streamLoadingViewControllerDidCancel:(NSKStreamLoadingViewController *)viewController;

@end

