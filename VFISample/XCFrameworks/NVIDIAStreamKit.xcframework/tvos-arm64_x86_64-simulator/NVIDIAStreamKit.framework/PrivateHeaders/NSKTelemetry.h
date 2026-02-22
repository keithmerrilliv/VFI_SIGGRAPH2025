/*
 * Copyright (c) 2022-2024 NVIDIA CORPORATION.  All Rights Reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 *
 */

#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Return the Telemetry Client ID used for GxTelemetry events sent by NVIDIAStreamKit.
extern NSString *NSKTelemetryClientID(void);

#ifdef __cplusplus
}
#endif
