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

@interface NSKStreamControlAction : NSObject

//! image should be 24pts high. width should be between 24 and 40pts wide.
@property (nonatomic) UIImage *image;

//! if set, then we don't keep showing selected state after tracking ends. default is NO
@property(nonatomic,getter=isMomentary) BOOL momentary;

@property (nonatomic, weak) id target;
@property (nonatomic) SEL action;

@end

typedef NS_ENUM(NSInteger, NSKStreamControlViewMode) {
    NSKStreamControlViewModeCompact,
    NSKStreamControlViewModeCollapsed,
    NSKStreamControlViewModeExpanded
};

/*! The on screen control buttons / drawer for touch devices. */
@interface NSKStreamControlView : UIView

@property (nonatomic, copy) NSArray<NSKStreamControlAction *> *actions;

- (BOOL)isActionSelected:(NSKStreamControlAction *)action;
- (void)setSelected:(BOOL)selected forAction:(NSKStreamControlAction *)action;

- (void)flashHandle;

- (void)scheduleHide;
- (void)cancelScheduledHide;
- (void)deferScheduledHide;

@property (nonatomic) NSKStreamControlViewMode mode;

- (void)setMode:(NSKStreamControlViewMode)mode animated:(BOOL)animated completion:(void (^)(BOOL completed))completion;
- (void)setHidden:(BOOL)hidden animationDuration:(NSTimeInterval)interval completion:(void (^)(BOOL completed))completion;

@property (readonly) UIPanGestureRecognizer *handlePanGesture;
#if TARGET_OS_IOS
@property (readonly) UIScreenEdgePanGestureRecognizer *edgePanGesture;
#endif

@end

#endif

