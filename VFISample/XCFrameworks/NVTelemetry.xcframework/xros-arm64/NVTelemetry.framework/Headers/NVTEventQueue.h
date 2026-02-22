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
#import <NVTelemetry/NVTEventTypes.h>

@class NVTEvent;

@interface NVTEventQueue : NSObject

+ (NVTEventQueue *)sharedQueue;

@property NSURLSession *URLSession;

- (void)    addEvent:(NSString *)eventName
             service:(NVTService)service
            clientId:(NSString *)clientId
          clientName:(NSString *)clientName
   definitionVersion:(NSString *)definitionVersion
       schemaVersion:(NSString *)schemaVersion
        gdprCategory:(NVTGDPRConsent)gdprCategory
          parameters:(NSData /* Property List serialized NSDictionary */ *)parameters;

- (void)sendPendingEvents;
- (void)sendPendingEventsWithCompletion:(void (^)(NSError *error))completion;

/// Delete the local events database and start fresh.
- (void)resetWithCompletion:(void (^)(void))completion;

@end
