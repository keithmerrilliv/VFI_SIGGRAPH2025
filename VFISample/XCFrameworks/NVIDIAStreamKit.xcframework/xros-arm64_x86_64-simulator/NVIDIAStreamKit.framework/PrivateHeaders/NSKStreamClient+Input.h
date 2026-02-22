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


#import <NVIDIAStreamKit/NSKStreamClient.h>

#import <NVIDIAStreamKit/NSKKeyCodeMap.h>

typedef NS_OPTIONS(NSUInteger, NSKKeyFlags) {
    NSKKeyFlagShift = 1 << 17,
    NSKKeyFlagControl = 1 << 18,
    NSKKeyFlagOption = 1 << 19,
    NSKKeyFlagCommand = 1 << 20
};

typedef NS_ENUM(NSUInteger, NSKStreamTouchEventPhase) {
    NSKStreamTouchEventPhaseDown = 0x01,
    NSKStreamTouchEventPhaseUp = 0x02,
    NSKStreamTouchEventPhaseMove = 0x04,
    NSKStreamTouchEventPhaseCancel = 0x08,
};

@interface NSKStreamTouchEvent : NSObject

@property (nonatomic) NSUInteger identifier;
@property (nonatomic) NSKStreamTouchEventPhase phase;
// normalized coordinates are in the range [0, 1], normalized to the streamed video bounds.
@property (nonatomic) CGFloat normalizedX;
@property (nonatomic) CGFloat normalizedY;
@property (nonatomic) CGFloat normalizedXRadius;
@property (nonatomic) CGFloat normalizedYRadius;

@property (nonatomic) NSTimeInterval timestamp;

@end

@protocol NSKStreamClientCursorDelegate <NSObject>
@required
- (void)streamClient:(NSKStreamClient *)client cursorChangedToBuiltinID:(NSUInteger)builtinCursorID;
- (void)streamClient:(NSKStreamClient *)client cursorChangedToBitmapID:(NSUInteger)bitmapCursorID;
- (void)streamClient:(NSKStreamClient *)client cursorChangedToBitmapID:(NSUInteger)bitmapCursorID hotSpot:(CGPoint)hotSpot imageData:(NSData *)imageData imageScale:(CGFloat)imageScale;
- (void)streamClient:(NSKStreamClient *)client cursorWarpedToNormalizedPoint:(CGPoint)np;

@end

@protocol NSKStreamClientHapticDelegate <NSObject>
@optional
- (void)streamClient:(NSKStreamClient *)client receivedHapticAtIndex:(GCControllerPlayerIndex)idx leftMotor:(float)leftMotor right:(float)rightMotor duration:(NSTimeInterval)duration;
- (void)streamClient:(NSKStreamClient *)client receivedOpaqueHapticMessage:(NSData *)hapticMessageData;
@end

@interface NSKStreamClient (Input)

@property (nonatomic) BOOL allowLocalMouse;
@property (weak) id<NSKStreamClientCursorDelegate> cursorDelegate;
@property (weak) id<NSKStreamClientHapticDelegate> hapticDelegate;

- (void)setActiveControllerCount:(NSUInteger)controllerCount;

- (void)sendTapEventAtPoint:(CGPoint)p inRect:(CGRect)r;
- (void)sendMouseAbsoluteMotionEvent:(CGPoint)p inRect:(CGRect)r;
- (void)sendMouseMotionEvent:(CGPoint)v;
- (void)sendMouseMotionEvent:(CGPoint)v accelerated:(BOOL)accelerated timestamp:(NSTimeInterval)ts;
- (void)sendMouseButtonEvent:(NSInteger)buttonIdx pressed:(BOOL)pressed;
- (void)sendMouseScrollEvent:(CGPoint)v;
- (void)sendPasteEventWithString:(NSString *)pasteboardContents;
- (void)sendKeyboardEventWithString:(NSString *)str;
- (void)sendKeyboardEventWithKeycode:(unsigned short)keyCode flags:(NSKKeyFlags)flags pressed:(BOOL)pressed;
- (void)sendKeyboardEventWithGCKeyCode:(GCKeyCode)keyCode flags:(NSKExtendedKeyFlags)flags pressed:(BOOL)pressed;

- (void)sendGamepadState:(GCExtendedGamepad *)gamepad;

- (void)sendTouchEvents:(NSArray<NSKStreamTouchEvent *> *)touches;

- (void)sendBulkPayload:(NSData *)data reliable:(BOOL)reliable;

@end
