// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#ifndef Serialization_h
#define Serialization_h

#import <CloudXRKit/CloudXRCommon.h>
#import <Foundation/Foundation.h>
#import <simd/matrix_types.h>

//#import "frameTransfer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cxrFrameMetadata {
    uint32_t frameNumber;
    uint32_t streamIndex;
    uint64_t poseTimestamp;
    uint64_t poseID;
    simd_float4x4 poseMatrix;
    uint16_t unwarped_width;
    uint16_t unwarped_height;
    uint16_t fov_center_horz;
    uint16_t fov_center_vert;
    uint16_t fov_radius_horz;
    uint16_t fov_radius_vert;
    uint16_t center_scale_horz;
    uint16_t center_scale_vert;
} cxrFrameMetadata;

typedef struct cxrClientTimingFeedback {
    uint32_t frameAckNum;
    uint64_t sendTimestampMicros;
    float avgQueueTimeMillis;
    float avgDeliveryTimeMillis;
} cxrClientTimingFeedback;


NSData *serializeDeviceDescVersion41(cxrDeviceDesc deviceDesc);

NSData *serializeVRTrackingStateVersion41(cxrVRTrackingState trackingState, cxrClientTimingFeedback timingFeedback);

NSData *serializeBlobAsClientDataBlobVersion41(NSData *data, UInt32 dataId);

NSData *serializeBlobAsClientInputEventVersion41(NSData *data, UInt32 dataId);

cxrFrameMetadata deserializeFrameMetadataVersion41(NSData *data);

#ifdef __cplusplus
}
#endif

#endif /* Serialization_h */
