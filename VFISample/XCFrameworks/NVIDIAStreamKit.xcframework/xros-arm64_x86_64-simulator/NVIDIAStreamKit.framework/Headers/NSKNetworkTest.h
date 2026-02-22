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

NS_ASSUME_NONNULL_BEGIN

@class NSKStreamConfiguration;
@class NSKNetworkTestThreshold;
@class NSKNetworkTestResult;
@protocol NSKNetworkTestDelegate;

typedef NS_ENUM(NSInteger, NSKNetworkTestReachability) {
    NSKNetworkTestReachabilityNone, // No network connected
    NSKNetworkTestReachabilityViaWWAN,
    NSKNetworkTestReachabilityViaWiFi, // used if on WiFi but frequency cannot be determined
    NSKNetworkTestReachabilityViaWiFi24,
    NSKNetworkTestReachabilityViaWiFi50,
    NSKNetworkTestReachabilityViaEthernet
};

/// Performs tests to determine if the current network is adequate for streaming with a given configuration.
@interface NSKNetworkTest : NSObject

- (instancetype)initWithStreamingConfiguration:(NSKStreamConfiguration *)config;

@property (readonly) NSKStreamConfiguration *configuration;

/// Add additional streaming resolution and frame rates for testing, in addition to what is specified in ``configuration``.
- (void)addStreamingResolution:(CGSize)resolution frameRate:(NSUInteger)fps;

@property (weak, nullable) id<NSKNetworkTestDelegate> delegate;

/// Asynchronously run the network test.
/// - Returns an NSProgress object that can be used to monitor test progress or cancel the test
- (nullable NSProgress *)run;

@end

@protocol NSKNetworkTestDelegate <NSObject>
@required

- (void)networkTest:(NSKNetworkTest *)test didResolveReachability:(NSKNetworkTestReachability)reachability;
/// Sent when the test cannot be completed due to a network or server error.
- (void)networkTest:(NSKNetworkTest *)test didFailWithError:(NSError *)error;
/// Sent when the test is concluded successfully.
/// This message does not necessarily imply streaming will be possible, just that the test results are available.
- (void)networkTest:(NSKNetworkTest *)test didFinishWithResult:(NSKNetworkTestResult *)result streamingProfileThresholds:(NSArray<NSKNetworkTestThreshold *> *)thresholds;

@end

@interface NSKNetworkTestThreshold : NSObject

/// Streaming resolution targeted by the test.
@property (readonly) CGSize streamingResolution;

/// Streaming framerate targeted by the test.
@property (readonly) NSUInteger streamingFrameRate;

/// Minimum tolerable bandwidth required for streaming in megabits per second.
@property (readonly) double minimumTolerableBandwidthMbs;

/// Recommended bandwidth for streaming in megabits per second.
@property (readonly) double recommendedBandwidthMbs;

/// Maximum tolerable (UDP RTT) latency required for streaming in milliseconds.
@property (readonly) double maximumTolerableLatencyMs;

/// Recommended latency for streaming in milliseconds.
@property (readonly) double recommendedLatencyMs;

/// Maximum tolerable packet loss required for streaming in % (0-100)
@property (readonly) double maximumTolerablePacketLossPercentage;

/// Recommended packet loss for streaming in % (0-100)
@property (readonly) double recommendedPacketLossPercentage;

/// Maximum tolerable 99th percentile frame jitter required for streaming in milliseconds.
@property (readonly) double maximumTolerableFrameJitterMs;

/// Recommended 99th percentile frame jitter for streaming in ms
@property (readonly) double recommendedFrameJitterMs;

@end

@interface NSKNetworkTestResult : NSObject

/// Network test session ID.
@property (readonly, strong, nullable) NSString *sessionID;

/// Measured downlink bandwidth in megabytes per second.
/// - note: This measurement is capped to `maximumTestBandwidthMbps`.
///         Therefore if `downlinkBandwidthMbs == maximumTestBandwidthMbps`, it can be assumed that
///         `downlinkBandwidthMbs > maximumTestBandwidthMbps`.
@property (readonly) double downlinkBandwidthMbs;

/// Measured uplink bandwidth in megabytes per second.
@property (readonly) double uplinkBandwidthMbs;

/// Maximum Bandwidth in reported by the server in megabits per second.
@property (readonly) double maximumTestBandwidthMbps;

/// Measured latency in milliseconds (UDP RTT) without stream packets (unloaded latency)
@property (readonly) double latencyWithoutStreamMs;

/// Measured latency in milliseconds (UDP RTT) with stream packets (loaded latency)
@property (readonly) double latencyWithStreamMs;

/// Measured how many packets were lost/dropped in % (0-100)
@property (readonly) double packetLossPercentage;

/// The measured 99th percentile jitter with the concept of packets in milliseconds
@property (readonly) double frameJitterMs;

/// Measured Path Maximum Transmission Unit size in bytes
@property (readonly) NSUInteger measuredPathMtuBytes;

/// Measured Path Maximum Transmission Unit size in bytes excluding packet headers (UDPH and IPH)
@property (readonly) NSUInteger maximumPacketSizeBytes;

/// Determine whether these test results fall within the tolerable thresholds for a given streaming profile.
- (BOOL)isTolerableForThreshold:(NSKNetworkTestThreshold *)threshold;

/// Determine whether these test results fall within the recommended thresholds for a given streaming profile.
- (BOOL)isRecommendedForThreshold:(NSKNetworkTestThreshold *)threshold;

@end

NS_ASSUME_NONNULL_END
