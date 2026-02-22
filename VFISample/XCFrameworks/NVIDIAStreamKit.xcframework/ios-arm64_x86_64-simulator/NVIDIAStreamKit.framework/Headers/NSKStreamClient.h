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
#import <GameController/GameController.h>

#import <NVIDIAStreamKit/NSKStreamSessionPolicy.h>
#import <NVIDIAStreamKit/NSKClientStatistics.h>

@class NSKStreamConfiguration;
@class NSKStreamStatistics;
@protocol NSKStreamClientStateObserver;
@protocol NSKStreamClientPolicyDelegate;

/// Represents the set of states a streaming session can be in.
typedef NS_ENUM(NSInteger, NSKSessionState) {
    NSKSessionStateInitial = 0, ///< NSKStreamClient newly created, -connect has not yet been sent.
    NSKSessionStateJoining,     ///< Establishing connection to streaming session
    NSKSessionStateStreaming,   ///< Streaming in-progress
    NSKSessionStatePausing,     ///< pause has been sent to NSKStreamClient, waiting for server to acknowledge
    NSKSessionStatePaused,      ///< Streaming paused. It may be possible to reconnect quickly via -resume.
    NSKSessionStateStopped      ///< Streaming ended, either intentionally or due to an error.
};

/**
 A NSKStreamClient manages a single streaming session over its lifetime.

 - note: a NSKStreamClient, once stopped, cannot be re-used, instead
         create a new NSKStreamClient for each new streaming session.
*/
@interface NSKStreamClient : NSObject

/**
 Initialize a NSKStreamClient with the given configuration, in an initial state.

 The NSKStreamClient does not perform any networking until the ``connect`` message is sent.
*/
- (id)initWithConfiguration:(NSKStreamConfiguration *)configuration NS_DESIGNATED_INITIALIZER;
- (id)init NS_UNAVAILABLE;

/// Access the readonly configuration the stream client was initialized with.
@property (readonly) NSKStreamConfiguration *configuration;

/// Access the unique identifier for the current streaming session.
/// Only valid once the connection has been established.
@property (readonly) NSString *currentSessionId;

/// The current keyboard layout that keyboard events are sent with.
/// Defaults to en\_US (which is needed for the sendKeyboardEventWithString: method).
@property (copy) NSString *keyboardLayout;

/// Enable/disable microphone input, if possible. Default is off.
/// - remark: See ``NSKStreamClientStateObserver/streamClientDidFailToEnableMic:``
@property (getter=isMicEnabled) BOOL micEnabled;

/// The current connection state of the stream client.
@property (readonly) NSKSessionState currentState;

/// Register for extended details about the client's connection to the server.
/// - note: a weak reference is maintained to observer by NSKStreamClient.
- (void)addStateObserver:(id<NSKStreamClientStateObserver>)observer NS_SWIFT_NAME(addStateObserver(_:));

/// Remove a registered state observer.
/// No further messages will be sent to observer by the stream client after this method returns.
- (void)removeStateObserver:(id<NSKStreamClientStateObserver>)observer NS_SWIFT_NAME(removeStateObserver(_:));

/// A policy delegate can be registered with the stream client to handle situations
/// when an existing streaming session already exists for the user.
@property (weak) id<NSKStreamClientPolicyDelegate> policyDelegate;

/**
 Attempt to establish a streaming session from the initial state.

 Will result in either of the following messages being sent to registered state observers:

 - ``NSKStreamClientStateObserver/streamClientDidConnect:`` if a session is connected successfully.
 - ``NSKStreamClientStateObserver/streamClient:didFailWithError:`` if the session connection could not be established or fails later.

 - remark: The connect message only needs to be sent once in the lifetime of a NSKStreamClient object.
           Repeated connect messages will be ignored.
*/
- (void)connect;

/**
 Disconnect the streaming session (if active) or cancel a connection attempt if in progress.

 Will result in either of the following messages being sent to registered state observers:

 - ``NSKStreamClientStateObserver/streamClientDidDisconnect:`` if the session disconnects cleanly.
 - ``NSKStreamClientStateObserver/streamClient:didFailWithError:`` if the session disconnects with an error.

 - note: once disconnection is complete, the client will move its
         currentState to NSKSessionStateStopped, and the NSKStreamClient instance will
         no longer be usable for streaming.
*/
- (void)disconnect;

/**
 Pause an active streaming session.

 - parameter handler: a block to be called when pausing completes (whether successfully or not)
                      that can be used to end any background task.

 - note: A background task must be registered with the UIApplication instance until
         the handler is called, to give time to pause the session and close
         any active network connections.
*/
- (void)pauseWithCompletionHandler:(void (^)(NSError *))handler;

/**
 Resume a paused streaming session.

 Will result in the following messages being sent to registered state observers:

 - ``NSKStreamClientStateObserver/streamClientWillReconnect:`` upon beginning reconnection.
 - ``NSKStreamClientStateObserver/streamClientDidConnect:`` upon successful reconnection.
 - ``NSKStreamClientStateObserver/streamClient:didFailWithError:`` if reconnection fails.
*/
- (void)resume;

/**
 Queue the delivery of a custom message to the remote application whose delivery is not time critical.

 - parameters:
    - messagePayload: application specific data payload to send
    - messageType: application specific message type string
    - mimeType: mime type of payload (may be nil)
    - recipient: application specific recipient of the message

 Use ``NSKStreamClientStateObserver/streamClient:didReceiveCustomMessagePayload:messageType:mimeType:recipient:``
 to handle messages or replies sent from the remote side to the local side.

 - note: The `currentState` property must be `Streaming` in order to send a custom message, otherwise it will be silently dropped.
*/
- (void)sendCustomMessagePayload:(NSData *)messagePayload messageType:(NSString *)messageType mimeType:(NSString *)mimeType recipient:(NSString *)recipient;

/**
 Send client statistics for logging in the ETL on the server side. Three types of statistics are supported:
 - ``NSKExtendedClientStatistics``
        This mode allows the client to send 8 int16 values per frame per stream index.
 - ``NSKEventClientStatistics``
        This mode allows string-based event logs to be sent from the client.
 - ``NSKGenericBlobClientStatistics``
        This mode allows sending generic blob stats. See GsBlobDefs.json in Stream SDK for format details.
 
 - note: The `currentState` property must be `Streaming` in order to send client stats, otherwise it will be dropped.
*/
- (void)sendClientStatistics:(NSKClientStatistics *)clientStats NS_SWIFT_NAME(sendClientStatistics(clientStats:));


/**
 Access streaming session log data for telemetry or debugging purposes.

 This method may be accessed at the end of a streaming session (either on success or failure) and provides
 detailed logging information over the course of the session.

 Note that if ``NSKStreamConfiguration/hasTelemetryTechnicalConsent`` is set, log data is automatically uploaded
 to NVIDIA and associated with the session id after the session ends.

 - parameter completion: Called on a background queue when the log file is successfully written or an error occurs.
 */
- (void)saveLogDataWithCompletion:(void (^)(NSString *logPath, NSError *error))completion;

@end

/// Keys used the dictionary passed to ``NSKStreamClientStateObserver/streamClient:didConnectDownstreamWithInfo:``
typedef NSString * NSKDownstreamConnectionInfoKey NS_STRING_ENUM;

/// => NSNumber containing kCMVideoCodecType
extern NSKDownstreamConnectionInfoKey const NSKDownstreamConnectionVideoCodecKey;

/// => NSArray<NSValue *> with entries for each video stream, where each NSValue contains a CGSize representing the video resolution.
extern NSKDownstreamConnectionInfoKey const NSKDownstreamConnectionVideoResolutionsKey;

/// All messages to NSKStreamClientStateObserver are sent by NSKStreamClient on the main dispatch queue.
@protocol NSKStreamClientStateObserver <NSObject>
@optional

/// Sent when the client connects successfully to the streaming server, with both upstream and downstream ready.
- (void)streamClientDidConnect:(NSKStreamClient *)client;

/// Sent when client disconnects intentionally (whether initiated from the client or the server).
- (void)streamClientDidDisconnect:(NSKStreamClient *)client;

/// Sent when streaming server is ready to receive upstream data such as:
/// - input events
/// - feature control messages
/// - custom messages
/// - upstream audio (microphone)
- (void)streamClientDidConnectUpstream:(NSKStreamClient *)client;

- (void)streamClient:(NSKStreamClient *)client didConnectDownstreamWithInfo:(NSDictionary<NSKDownstreamConnectionInfoKey, id> *)connectionInfo;

/**
 Sent when client fails to connect or resume, or if it disconnects unexpectedly.
 - parameter error: Describes the nature of the failure. See `NSKError` for more details.
*/
- (void)streamClient:(NSKStreamClient *)client didFailWithError:(NSError *)error;

/**
 While client is in state NSKSessionStateJoining, it may send periodic updates with an estimate
 as to the time when the connection may be established.

 - parameters:
    - client: the connecting stream client
    - remaining: the approximate time in seconds from now when the connection may be established.
    - queuePosition: the number of users ahead of us in the queue to be allocated a game seat.
*/
- (void)streamClient:(NSKStreamClient *)client didUpdateConnectionTimeRemaining:(NSNumber *)remaining queuePosition:(NSNumber *)queuePosition;

/**
 While client is in state NSKSessionStateStreaming, it periodically provides information about the health
 of the streaming connection via this message.

 - parameter stats: detailed streaming health statistics
*/
- (void)streamClient:(NSKStreamClient *)client didUpdateStreamingStatistics:(NSKStreamStatistics *)stats;

/**
 Sent when the client will attempt to reconnect to a previously connected session,
 for example, due to a network interruption.
*/
- (void)streamClientWillReconnect:(NSKStreamClient *)client;

/**
 Sent when the client was unable to enable the uplink microphone channel, typically due to
 a lack of the application microphone use permission.
*/
- (void)streamClientDidFailToEnableMic:(NSKStreamClient *)client;

/**
 Sent when an application specific custom message is received from the server side.

 - parameters:
    - client: The associated stream client.
    - messagePayload: application specific opaque data sent from the remote side.
    - messageType: application specific type description of the data provided by the remote side.
    - mimeType: mime type description of the data provided by the remote side.
    - recipient: Subsystem expected to handle the message, provided by the remote side.
*/
- (void)streamClient:(NSKStreamClient *)streamClient didReceiveCustomMessagePayload:(NSData *)messagePayload messageType:(NSString *)messageType mimeType:(NSString *)mimeType recipient:(NSString *)recipient;

@end

/// Decides what to do when a user has an existing active streaming session and a new one is requested.
@protocol NSKStreamClientPolicyDelegate <NSObject>

/**
 A user can only have one active streaming session (across all clients) at a time.

 Implement this delegate method to choose how to proceed if an existing session already exists.

 See <doc:NSKStreamSessionPolicy> for more details.
*/
- (void)streamClient:(NSKStreamClient *)client decidePolicyForExistingSessionId:(NSString *)existingSessionId existingAppId:(NSUInteger)existingAppId decisionHandler:(void (^)(NSKStreamSessionPolicy policy))decisionHandler;

@end
