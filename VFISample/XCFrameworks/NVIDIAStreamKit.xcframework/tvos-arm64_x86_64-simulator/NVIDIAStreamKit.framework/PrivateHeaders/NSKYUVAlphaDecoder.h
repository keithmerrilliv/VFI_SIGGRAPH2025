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
#import <CoreVideo/CoreVideo.h>

@interface NSKYUVAlphaDecoder : NSObject

/*!
    Decodes textures encoded with the YUVxA scheme to RGBA.
    @param yuvxaBuffer A buffer with format kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
    @return A buffer with format kCVPixelFormatType_420YpCbCr8VideoRange_8A_TriPlanar
 */
- (CVImageBufferRef)decodeYUVxABuffer:(CVImageBufferRef)yuvxaBuffer CF_RETURNS_RETAINED;

@end
