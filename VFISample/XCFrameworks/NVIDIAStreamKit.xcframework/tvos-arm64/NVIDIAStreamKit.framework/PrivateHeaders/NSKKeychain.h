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

@interface NSKKeychainItem : NSObject

@property (copy) NSString *server;      // required
@property (copy) NSString *account;     // required
@property (copy) NSString *password;    // required

@property (copy) NSData *applicationData; // optional, defaults to nil

@end

@interface NSKKeychain : NSObject

- (instancetype)initWithServicePrefix:(NSString *)service accessGroup:(NSString *)accessGroup; // Designated initializer

@property (readonly, copy) NSString *servicePrefix;
@property (readonly, copy) NSString *accessGroup;

- (NSArray<NSKKeychainItem *> *)allAccountsReturningError:(NSError *__autoreleasing *)error; // Returns KeychainItems with server and account populated, but password and applicationData will be nil. To fetch that info, use itemForAccount:server:error:.

- (BOOL)removeItemForAccount:(NSString *)account server:(NSString *)server error:(NSError *__autoreleasing *)error;

- (BOOL)storeItem:(NSKKeychainItem *)item error:(NSError *__autoreleasing *)error;

- (NSKKeychainItem *)itemForAccount:(NSString *)account server:(NSString *)server error:(NSError *__autoreleasing *)error;

@end

extern NSString *const NSKKeychainErrorDomain;
