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

struct NvstVideoDecodeUnit_t;
typedef struct NvstVideoDecodeUnit_t NvstVideoDecodeUnit;

//! Wraps a NvstVideoDecodeUnit to provide ObjC lifetime management capabilities (retain/release/dealloc, heap/leaks) for this objects.
@interface NSKVideoDecodeUnit : NSObject

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithVDU:(const NvstVideoDecodeUnit *)vdu NS_DESIGNATED_INITIALIZER;

@property (readonly) const NvstVideoDecodeUnit *vdu NS_RETURNS_INNER_POINTER;

@end

