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

NS_ASSUME_NONNULL_BEGIN

/// Provides version number information about the NVIDIAStreamKit framework.
@interface NSKBuildInfo : NSObject

+ (instancetype)currentBuild;

/// The git commit hash for this release.
@property (readonly) NSString *gitSHA;
/// The perforce changelist number for the gamestream components in this release.
@property (readonly) NSString *perforceChange;
/// The perforce branch for the gamestream components in this release.
@property (readonly) NSString *perforceBranch;
/// The user-visible version of this release (e.g. 1.0)
@property (readonly) NSString *marketingVersion;
/// The auto-incremented build number of this release.
@property (readonly) NSString *buildNumber;
/// The build type of this release (intended audience).
///
/// Types include:
/// - local (local developer build)
/// - ibeta (internal testing build)
/// - ebeta (external testing build)
/// - release (public release)
@property (readonly) NSString *buildType;

- (NSString *)shortDescription;

@end

NS_ASSUME_NONNULL_END
