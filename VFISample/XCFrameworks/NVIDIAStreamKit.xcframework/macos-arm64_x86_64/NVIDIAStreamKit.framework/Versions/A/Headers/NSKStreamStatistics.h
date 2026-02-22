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

typedef NS_ENUM(NSInteger, NSKStreamQuality) {
    NSKStreamQualityUnknown,
    NSKStreamQualityPoor,       //!< represented by a red indicator
    NSKStreamQualityMarginal,   //!< represented by a yellow indicator
    NSKStreamQualityGood        //!< represented by a green indicator / no indicator
};

@interface NSKStreamStatistics : NSObject

/*! Codec of streaming video. */
@property (readonly) CMVideoCodecType streamingCodec;

/*! Transfer function for streaming video. Some type of `kCVImageBufferTransferFunction` */
@property (readonly) NSString *transferFunction;

/*! Color primaries for streaming video. Some type of `kCVImageBufferColorPrimaries` */
@property (readonly) NSString *colorPrimaries;

/*! Bit depth of streaming video */
@property (readonly) NSUInteger bitDepth;

/*! Current streaming resolution (may be less than requested resolution in configuration) */
@property (readonly) CGSize streamingResolution;

/*! Total frames processed in this session */
@property (readonly) NSUInteger frameCount;

#pragma mark - Summary Stats

/*! average streaming frame rate(fps) since last SummaryStats */
@property (readonly) uint32_t frameRate;

/*! number of frames considered in stats */
@property (readonly) uint32_t summaryFrames;

/*! number of frames dropped (from sequential count) */
@property (readonly) uint32_t droppedFrames;

/*! number of frames received but never completed to presentation */
@property (readonly) uint32_t orphanedFrames;

/*! number of frames with decoding errors */
@property (readonly) uint32_t decodeErrors;

/*! number of frames that decoded correctly but were not rendered */
@property (readonly) uint32_t skippedFrames;

/*! average receive time between video network frames */
@property (readonly) uint32_t averagePacketTimeUs;

/*! average difference time between desired frame rate and packet receipt time */
@property (readonly) uint32_t averagePacketJitterUs;

/*! number of times present was called */
@property (readonly) uint32_t presentFrames;

#pragma mark - Average Frame Stats

/*! relative time from receive to decode begin */
@property (readonly) uint32_t beginTimeUs;

/*! relative time from begin to decode complete */
@property (readonly) uint32_t decodeTimeUs;

/*! relative time between decode complete and render begin */
@property (readonly) uint32_t renderBeginTimeUs;

/*! relative time from render begin to render complete */
@property (readonly) uint32_t renderTimeUs;

/*! relative time from render complete to display complete */
@property (readonly) uint32_t presentTimeUs;

/*! total time for complete frames to render and display */
@property (readonly) uint32_t totalTimeUs;

#pragma mark - Quality of Service Stats

/*! The average video streaming rate from server to client in kilobits-per-second */
@property (readonly) uint32_t avgStreamingRate;

/*! Estimated bandwidth utilization from server to client as a percentage */
@property (readonly) uint32_t bandwidthUtilization;

/*! Estimated bandwidth available for streaming in kilobits-per-second. */
@property (readonly) uint32_t bandwidthAvailable;

/*! The aggregate number of lost video packets from server to client. */
@property (readonly) uint32_t packetLoss;

/*! The estimated round trip delay between server & client */
@property (readonly) uint32_t roundTripDelayMs;

/*! App FPS - the rate at which the remote application is emitting rendered frames. */
@property (readonly) uint32_t remoteAppFrameRate;

/*! Raw stream Quality metric. Ranges from (0, 1] where 0 = unknown, nextafter(0, 1) = terrible, 1 = great.
 *
 *  Applications can make their own determinations about when to show stream quality warnings,
 *  or they can use the buckets provided by NSKStreamQuality.
 *
 */
@property (readonly) double rawStreamQuality;

@property (readonly) NSKStreamQuality streamQuality;

#pragma mark - Input Stats

/*! count of keyboard events */
@property (readonly) uint32_t keyboardEvents;

/*! count of mouse events */
@property (readonly) uint32_t mouseEvents;

/*! count of gamepad events */
@property (readonly) uint32_t gamepadEvents;

#pragma mark - Per Stream Stats

/*! ``beginTimeUs`` for a single stream. */
- (uint32_t)beginTimeUsForStream:(NSUInteger)streamIndex;

/*! ``decodeTimeUs`` for a single stream. */
- (uint32_t)decodeTimeUsForStream:(NSUInteger)streamIndex;

/*! ``renderBeginTimeUs`` for a single stream. */
- (uint32_t)renderBeginTimeUsForStream:(NSUInteger)streamIndex;

/*! ``renderTimeUs`` for a single stream. */
- (uint32_t)renderTimeUsForStream:(NSUInteger)streamIndex;

/*! ``presentTimeUs`` for a single stream. */
- (uint32_t)presentTimeUsForStream:(NSUInteger)streamIndex;

/*! ``totalTimeUs`` for a single stream. */
- (uint32_t)totalTimeUsForStream:(NSUInteger)streamIndex;

/*! ``avgStreamingRate`` for a single stream. */
- (uint32_t)avgStreamingRateForStream:(NSUInteger)streamIndex;

/*! ``bandwidthUtilization`` for a single stream. */
- (uint32_t)bandwidthUtilizationForStream:(NSUInteger)streamIndex;

/*! ``bandwidthAvailable`` for a single stream. */
- (uint32_t)bandwidthAvailableForStream:(NSUInteger)streamIndex;

/*! ``packetLoss`` for a single stream. */
- (uint32_t)packetLossForStream:(NSUInteger)streamIndex;

/*! ``roundTripDelayMs`` for a single stream. */
- (uint32_t)roundTripDelayMsForStream:(NSUInteger)streamIndex;

@end
