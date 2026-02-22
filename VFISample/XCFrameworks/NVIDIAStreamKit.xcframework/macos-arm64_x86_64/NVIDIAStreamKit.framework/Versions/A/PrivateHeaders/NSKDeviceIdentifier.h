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

#ifdef __cplusplus
extern "C" {
#endif

//! Returns an identifier that can uniquely identify the current device.
//! On iOS, this is [[UIDevice currentDevice] identifierForVendor]
//! On macOS, this is the device's serial number, or failing that its default WiFi MAC address
extern NSString *NSKDeviceIdentifier(void);

#ifdef __cplusplus
}
#endif
