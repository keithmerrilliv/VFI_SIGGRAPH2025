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

#define MAP_ENTRY(code, name, message) \
    static const uint32_t _##name = code; \
    static const char _##name##_STR[] = #name; \
    static const char _##name##_MSG[] = message;
#include <NVIDIAStreamKit/NSKErrorDefines.h>
#undef MAP_ENTRY

#define NSKError(name) ([NSError errorWithDomain:NSKErrorDomain code:_##name userInfo:@{NSLocalizedFailureReasonErrorKey:[NSString stringWithFormat:@"%s: %s", _##name##_STR, _##name##_MSG]}])

/// NSError domain for errors originating from the NVIDIAStreamKit framework.
///
/// Note that not all errors surfaced by the NVIDIAStreamKit framework are in the NSKErrorDomain, because errors
/// can also be caused by lower level components as well (e.g. Foundation)
extern NSString *const NSKErrorDomain;

/// NSError conveniences that categorize common NSK errors.
@interface NSError (NSKError)

/// YES if this error is caused by an authentication issue. (Recommend re-authenticating the user).
@property (readonly) BOOL nsk_isAuthError;
/// YES if the streaming session disconnected due to a server configured time limit.
@property (readonly) BOOL nsk_isStreamTimeoutError;
/// YES if the streaming session could not be started because it was configured to not stream over a cellular WWAN
/// connection, yet a cellular WWAN network connection was all that was available.
@property (readonly) BOOL nsk_isCellularDataNotAvailableError;
/// YES if the session disconnected due to no user input over a server configured duration.
/// This error is a subset of ``nsk_isStreamTimeoutError``
@property (readonly) BOOL nsk_isAFKError;
/// YES if the session disconnected due to an unknown server side error (from the client perspective).
@property (readonly) BOOL nsk_isUnspecifiedServerError;
/// YES if the session disconnected due to a network error.
@property (readonly) BOOL nsk_isNetworkError;

@end
