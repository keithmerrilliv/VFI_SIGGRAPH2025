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

/// Policies for handling multiple active streaming sessions for a single user account.
typedef NS_ENUM(NSInteger, NSKStreamSessionPolicy)
{
    //! Cancel the new stream, leaving the previous session as the active one (cancel connection).
    NSKStreamSessionPolicyCancel,
    //! Resume the existing session here.
    NSKStreamSessionPolicyResume,
    //! Stop the existing session and replace it with a new one.
    NSKStreamSessionPolicyReplace
};
