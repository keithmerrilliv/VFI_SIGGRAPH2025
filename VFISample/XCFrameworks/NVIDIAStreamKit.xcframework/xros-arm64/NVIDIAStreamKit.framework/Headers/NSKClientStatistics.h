/*
 * Copyright (c) 2024 NVIDIA CORPORATION.  All Rights Reserved.
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

/// Abstract class representing a set of client produced statistics that are to be sent to the server and saved into the server's statistics stream.
@interface NSKClientStatistics : NSObject

/// The frame number to associate with the statistics. A frame number of 0 is acceptable for statistics that do not pertain to a specific frame.
@property uint32_t frameNumber;
/// The video stream index these statistics apply to. Defaults to 0.
@property uint16_t streamIndex;

@end

#pragma mark - Extended Stats

/// Represents a set of 8 int16 values to be sent to the server and written into the stream statistics ETL log.
@interface NSKExtendedClientStatistics : NSKClientStatistics

/// Set one of the 8 extended stats values.
/// - parameters:
///   - value A 16 bit value representing some client performed measurement
///   - index The position of the measurement in the array. Must be <= 7.
- (void)setValue:(int16_t)value atIndex:(NSUInteger)index;

@end

#pragma mark - Event String Stats

/// Free form string events to be saved with stats during streaming
@interface NSKEventClientStatistics : NSKClientStatistics

@property (copy) NSString *stringValue;

@end

#pragma mark - Generic Blob Stats

/// Generic blob data statistics. See GsBlobDefs.json in Stream SDK for format details.
@interface NSKGenericBlobClientStatistics : NSKClientStatistics

@property (copy) NSData *data;

@end

