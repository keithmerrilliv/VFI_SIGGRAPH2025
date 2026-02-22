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


#ifndef NSKLogging_h
#define NSKLogging_h

#ifdef __cplusplus
extern "C" {
#endif

extern void NSKLog(NSString *format, ...) NS_FORMAT_FUNCTION(1,2);
extern void NSKLogv(NSString *format, va_list args) NS_FORMAT_FUNCTION(1,0);

extern void NSKLogForward(NSString *msg);

//! Roll the current log file.
extern void NSKLogRoll(void);

extern NSString *NSKLogThreadLabel(void);
extern NSString *NSKLogFormattedDate(void);
extern NSString *NSKLogFilename(const char *file);

/*! Save the log lines written since the launch of the process to path. */
extern BOOL NSKLogWriteToPath(NSString *path, NSError *__autoreleasing *outError);

/*! Save the log lines written during the previous run of the process to path. */
extern BOOL NSKLogWriteBackupToPath(NSString *path, NSError *__autoreleasing *outError);

#ifdef __cplusplus
}; // extern "C"
#endif

#if DEBUG
#define LOGD(format, ...) NSKLog(@"%@  DEBUG {%@} %s %@:%d " format, NSKLogFormattedDate(), NSKLogThreadLabel(), __PRETTY_FUNCTION__, NSKLogFilename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOGD_FORWARD(msg) NSKLogForward([NSString stringWithUTF8String:msg])
#else
#define LOGD(format, ...)
#define LOGD_FORWARD(msg)
#endif

#define LOGI(format, ...) NSKLog(@"%@   INFO {%@} %s %@:%d " format, NSKLogFormattedDate(), NSKLogThreadLabel(), __PRETTY_FUNCTION__, NSKLogFilename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOGW(format, ...) NSKLog(@"%@   WARN {%@} %s %@:%d " format, NSKLogFormattedDate(), NSKLogThreadLabel(), __PRETTY_FUNCTION__, NSKLogFilename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...) NSKLog(@"%@  ERROR {%@} %s %@:%d " format, NSKLogFormattedDate(), NSKLogThreadLabel(), __PRETTY_FUNCTION__, NSKLogFilename(__FILE__), __LINE__, ##__VA_ARGS__)

#define LOGI_FORWARD(msg) NSKLogForward([NSString stringWithUTF8String:msg])
#define LOGW_FORWARD(msg) NSKLogForward([NSString stringWithUTF8String:msg])
#define LOGE_FORWARD(msg) NSKLogForward([NSString stringWithUTF8String:msg])

#endif /* NSKLogging_h */
