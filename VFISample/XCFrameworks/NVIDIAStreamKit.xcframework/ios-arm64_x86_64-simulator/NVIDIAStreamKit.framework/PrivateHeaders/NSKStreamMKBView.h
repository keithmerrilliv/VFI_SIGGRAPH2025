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
#import <GameController/GameController.h>

#import <NVIDIAStreamKit/NSKKeyCodeMap.h>

@class NSKStreamVideoView;
@class NSKStreamMKBView;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NSKStreamCursorType) {
    NSKStreamCursorTypeSystem,
    NSKStreamCursorTypeBitmap,
};

@interface NSKStreamCursor : NSObject

@property (nonatomic) NSUInteger identifier;
@property (nonatomic, nullable) NSData *bitmapData;
@property (nonatomic) CGFloat bitmapScale;
@property (nonatomic) CGPoint hotSpot;
@property (nonatomic) NSKStreamCursorType cursorType;

@end

@protocol NSKStreamMKBViewDelegate <NSObject>
@required
- (void)mkbView:(NSKStreamMKBView *)view didUpdateCursorAbsolutePoint:(CGPoint)p inRect:(CGRect)boundingRect;
- (void)mkbView:(NSKStreamMKBView *)view didMoveCursorBy:(CGPoint)relativeMotion;
- (void)mkbView:(NSKStreamMKBView *)view didScrollMouseBy:(CGPoint)delta;
//! Mouse button up/down state changed.
//! @param view the view sending the event
//! @param downState YES if button down, NO if button up
//! @param buttonNumber 0 index button number (0 = left, 1 = right, 2 = mid, 3 = aux1, 4 = aux2, etc)
- (void)mkbView:(NSKStreamMKBView *)view didChangeMouseButtonState:(BOOL)downState button:(NSUInteger)buttonNumber;

- (void)mkbView:(NSKStreamMKBView *)view didChangeKeyboardState:(BOOL)downState keyCode:(GCKeyCode)keyCode modifierFlags:(NSKExtendedKeyFlags)flags;

@end

/*! A view that displays a mouse cursor (if not hidden) and sends mouse and keyboard events. */
@interface NSKStreamMKBView : UIView

+ (NSString *)keyboardLocale;

/*! The delegate receives keyboard and mouse events. */
@property (nonatomic, weak) id<NSKStreamMKBViewDelegate> delegate;

/*! Set the style of the cursor. Set to nil to hide and lock the cursor. */
@property (nonatomic, nullable) NSKStreamCursor *currentCursor;

/*! Warp the cursor to a point (in the coordinate system of self), clamping to the video rect */
- (void)warpCursorToPoint:(CGPoint)point;

/*! Warp the cursor to a normalized point (0 to 1 in the video rect) */
- (void)warpCursorToNormalizedPoint:(CGPoint)point;

/*! Video view, used to convert between view coordinates and video coordinates. */
@property (nonatomic, weak) NSKStreamVideoView *videoView;

/*! Send key-up messages to the delegate for all currently held keys / mouse buttons. */
- (void)releaseKeys;

@property (nonatomic) BOOL hasMouse;

@end

NS_ASSUME_NONNULL_END

#endif // TARGET_OS_IPHONE

