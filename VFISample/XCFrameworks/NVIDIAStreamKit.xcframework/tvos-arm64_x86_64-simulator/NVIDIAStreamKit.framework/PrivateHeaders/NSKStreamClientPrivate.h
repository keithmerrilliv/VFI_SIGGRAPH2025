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

@class UIActivityViewController;

@interface NSKStreamClient (Private)

#if TARGET_OS_IOS
//! Return a UIActivityViewController by which any captured bitstreams can be shared.
@property (readonly) UIActivityViewController *capturedBitstreamActivityViewController;
#endif

@property (readonly) NSString *zoneName;
@property (readonly) NSString *instanceName;

@property (readonly) NSString *subSessionId;

@end
