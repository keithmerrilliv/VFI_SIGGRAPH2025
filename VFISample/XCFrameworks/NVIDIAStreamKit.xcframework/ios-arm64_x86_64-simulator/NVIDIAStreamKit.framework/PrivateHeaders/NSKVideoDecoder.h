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

@class NSKVideoDecodeUnit;
@class NSKVideoDecoder;

typedef NS_ENUM(NSInteger, NSKVideoDecodeResult) {
    NSKVideoDecodeResultSuccess, // frame fully decoded successfully
    NSKVideoDecodeResultHeaderAcknowledged, // frame was a header frame (no video data available)
    NSKVideoDecodeResultEndOfStreamAcknowledged, // frame was an end of stream (no video data available)
    NSKVideoDecodeResultSkipped, // frame decoded, but no output is available (used to catch up when running behind)
    NSKVideoDecodeResultQueueFull, // frame cannot be decoded because the queue is full.
    NSKVideoDecodeResultFailed // frame cannot be decoded (because it is corrupt or because of system errors)
};


@protocol NSKVideoDecoderDelegate <NSObject>

- (NSDictionary *)videoDecoder:(NSKVideoDecoder *)decoder extraImageAttributesForFormat:(CMVideoFormatDescriptionRef)format;

@end

@interface NSKVideoDecoder : NSObject

- (instancetype)initWithCodec:(CMVideoCodecType)codec;

@property (weak) id<NSKVideoDecoderDelegate> delegate;

@property (readonly) CMVideoCodecType codec;
@property (readonly) NSString *transferFunction;
@property (readonly) NSString *colorPrimaries;
@property (readonly) NSUInteger bitDepth;
@property NSUInteger streamIndex;

//! Asynchronously decode the provided frame, and produce a CMSampleBufferRef
//! \param vdu the encoded video frame to decode
//! \param startHandler called when this frame reaches the front of the queue and decoding actually begins.
//! \param completion called when this frame is decoded (successfully or not)
- (void)decodeVDU:(NSKVideoDecodeUnit *)vdu startHandler:(dispatch_block_t)startHandler completion:(void (^)(NSKVideoDecodeResult result, CMSampleBufferRef sb))completion;

//! Block caller until all pending VDUs are completed.
- (void)flush;

+ (BOOL)isHEVCSupported;

@end
