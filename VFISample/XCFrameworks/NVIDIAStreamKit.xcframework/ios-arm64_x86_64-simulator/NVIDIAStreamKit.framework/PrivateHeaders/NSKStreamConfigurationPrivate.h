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

@interface NSKStreamConfiguration (Private)

/**
 The overridden device identifier used to negotiate a streaming session.

 Leave this un-set to use the default device identifier. Setting a
 identifier here is only for NVIDIA internal applications.
 */
@property (nullable, copy) NSString *deviceHashId;

/**
 The set of all server zones (zone name => zone URL).

 Only available if the NSKStreamConfiguration was constructed with:
 ``NSKStreamConfiguration/loadDefaultConfigurationForStarfleetAccount:completion:``
 or
 ``NSKStreamConfiguration/loadDefaultConfigurationForStarfleetAccount:completion:``
 */
@property (nullable, readonly) NSDictionary<NSString *, NSURL *> *allZones;

@end
