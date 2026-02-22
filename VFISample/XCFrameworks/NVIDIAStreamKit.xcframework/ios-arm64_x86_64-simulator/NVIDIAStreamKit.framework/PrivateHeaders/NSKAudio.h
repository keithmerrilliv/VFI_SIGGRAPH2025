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
#import <AVFoundation/AVFoundation.h>

struct NvstAudioFrame_t;

@protocol NSKAudioMicDelegate;

@interface NSKAudio : NSObject

- (void)reset;
- (void)playFrame:(const struct NvstAudioFrame_t *)buffer;

//! Must be called prior to playing first frame
- (void)startSession;

//! Call after the last frame has been played and after -reset has been called.
- (void)stopSession;

@property (weak) id<NSKAudioMicDelegate> micDelegate;

//! Size of a microphone packet in terms of audio frames sampled @ 48000Hz.
//! Defaults to 240 (5ms).
@property AVAudioFrameCount micFramesPerPacket;

@property (nonatomic, getter=isMicEnabled) BOOL micEnabled;

@end

@protocol NSKAudioMicDelegate <NSObject>

// Called on an arbitrary background queue.
- (void)audioDidFailToEnableMic:(NSKAudio *)audio;

//! Called on an arbitrary background queue.
//! Receiver must finish with buffer before returning.
- (void)audioDidProduceMicFrame:(AVAudioPCMBuffer *)buffer when:(AVAudioTime *)when;

@end

@interface AVAudioPCMBuffer (NSKAudio)

//! Breaks the receiver up into nvst sized audio chunks and calls handler for each chunk.
//! \param time the start time of this buffer
//! \param framesPerPacket the number of frames per packet to produce
//! \param handler called for each NvstAudioFrame. Return NO to break early.
- (void)nsk_processNvstAudioPacketsWithTime:(AVAudioTime *)time framesPerPacket:(AVAudioFrameCount)framesPerPacket handler:(BOOL (^)(const struct NvstAudioFrame_t *frame))handler;

@end
