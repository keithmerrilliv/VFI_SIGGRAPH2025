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
#import <CoreMedia/CoreMedia.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT const CFStringRef kCMSampleAttachmentKey_NvstGenericMetadata;

@interface NSKVideoFrame : NSObject

- (instancetype)initWithDecodedFrame:(CMSampleBufferRef)decodedFrame frameNumber:(NSUInteger)frameNumber streamIndex:(NSUInteger)streamIndex didRender:(dispatch_block_t)didRender;

@property (readonly) NSUInteger frameNumber;
@property (readonly) NSUInteger streamIndex;
@property (readonly) CMSampleBufferRef decodedFrame;
@property (readonly) CGSize frameSize;
@property (readonly, strong, nullable) NSData *nvstGenericMetadata;

@property (readonly, copy) dispatch_block_t didRender;

@end

NS_ASSUME_NONNULL_END
