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

@interface NSArray (NSKExtras)

- (NSArray *)nsk_arrayByMappingObjects:(id (^)(id obj))transformer;
- (id)nsk_firstObjectMatchingPredicate:(NSPredicate *)predicate;

@end

@interface NSAttributedString (NSKExtras)

- (void)nsk_appendAttributes:(NSDictionary *)attributes format:(NSString *)format, ... NS_FORMAT_FUNCTION(2, 3);

@end

@interface NSDate (NSKExtras)

+ (NSDate *)nsk_dateWithHTTPHeaderString:(NSString *)headerString;
- (NSString *)nsk_HTTPHeaderString;

- (BOOL)nsk_isInFuture;
- (BOOL)nsk_isInPast;

@end

@interface NSMutableURLRequest (NSKExtras)

- (void)nsk_setHTTPBasicAuthorizationWithUser:(NSString *)user andPassword:(NSString *)password;

- (void)nsk_setFormEncodedBodyWithQueryItems:(NSArray<NSURLQueryItem *> *)queryItems;

@end

@interface NSOutputStream (NSKExtras)

- (BOOL)nsk_write:(const uint8_t *)data length:(NSUInteger)length;

@end

@interface NSHTTPURLResponse (NSKExtras)

@property (readonly, getter=nsk_isErrorResponse) BOOL nsk_isErrorResponse;

@end

@interface NSError (NSKExtras)

+ (NSError *)nsk_cancelledError;

@property (readonly, getter=gfn_isCancelledError) BOOL nsk_cancelledError;

@end

@interface NSProgress (NSKExtras)

+ (NSProgress *)nsk_indeterminateProgress;

@end

@interface NSURL (NSKExtras)

//! Parse maybeURLString and return a NSURL if it is well-formed, or nil if it is not.
//! Contrast this method with +[NSURL URLWithString:] which throws an exception if the string is not well-formed.
//! \param maybeURLString an NSString which may be nil and may or may not contain a well-formed URL.
+ (NSURL *)nsk_parseURLWithString:(NSString *)maybeURLString;

+ (NSURL *)nsk_HTTPSURLFromString:(NSString *)urlString;

//! Creates and returns a URL corresponding to the given redirect keyword on www.nvidia.com.
//! \param keyword see https://wiki.nvidia.com/engwiki/index.php/Nvcpl/Documentation/MarketingUrls for supported keywords.
+ (NSURL *)nsk_NVURLOfRedirectKeyword:(NSString *)keyword;

@end

@interface NSFileManager (NSKExtras)

//! Create a compressed zip archive of a file or a directory.
//! \param path the source file or directory
//! \param toPath path to write the zip file at. If an item at toPath already exists, the zip operation will fail and an appropriate error will be returned (NSFileWriteFileExistsError in NSCocoaErrorDomain).
//! \param error out-param returning error object. may be NULL.
//! \returns YES if the operation succeeds, NO otherwise.
- (BOOL)nsk_zipItemsAtPath:(NSString *)path toPath:(NSString *)toPath error:(NSError *__autoreleasing *)error;

@end

@interface NSData (NSKExtras)

//! Compute SHA256 digest of receiver's data.
- (NSData *)nsk_sha256;

@end
