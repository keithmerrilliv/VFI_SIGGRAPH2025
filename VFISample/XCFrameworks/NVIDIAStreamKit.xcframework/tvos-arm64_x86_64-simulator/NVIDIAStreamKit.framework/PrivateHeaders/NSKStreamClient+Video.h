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

@class NSKVideoFrame;

/// Messages to the video delegate are sent on a serial background queue by NSKStreamClient.
@protocol NSKStreamClientVideoDelegate <NSObject>

- (void)streamClient:(NSKStreamClient *)client didReceiveVideoFrame:(NSKVideoFrame *)videoFrame;
- (void)streamClientFlushVideoForEndOfStream:(NSKStreamClient *)client andRemoveImage:(BOOL)removeImage;

@optional
/// @return A dictionary containing CVPixelBuffer attribute keys and values (e.g. kCVPixelBufferPixelFormatTypeKey)
///         These attributes will be passed to VTDecompressionSessionCreate and will affect the format and usage
///         of the decoded frames.
- (NSDictionary *)streamClient:(NSKStreamClient *)client extraVideoFrameImageAttributesForFormat:(CMVideoFormatDescriptionRef)format streamIndex:(NSUInteger)streamIndex;

@end

@interface NSKStreamClient (Video)

@property (weak) id<NSKStreamClientVideoDelegate> videoDelegate;

@end
