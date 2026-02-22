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

@class NSKJSONSchemaValidator;

@interface NSKJSON : NSObject

//! Parse JSON out of an NSURLSessionDataTask completion handler.
//! @param data The data to parse. If it cannot be parsed as JSON, error will be set to point to an error object.
//! @param response NSURLResponse to be examined for a success code. If it is non-success, error will be set to point to an error object.
//! @param validator If data is decoded to JSON successfully, validate the structure of the JSON object. If validation fails, error will be set to point to an error object.
//! @param error An in-out pointer to an error object. If *error is non-nil, the method will return nil and leave error set to its previous value. If *error is nil, but any other error occurs, *error will be set to an error object.
//! @return A plist style object deserialized by NSJSONSerialization, iff *error is non-nil.
+ (id)JSONWithData:(NSData *)data response:(NSURLResponse *)response schemaValidator:(NSKJSONSchemaValidator *)validator error:(NSError **)error;


@end

typedef NS_ENUM(NSUInteger, NSKJSONNullHandling) {
    //! Map JSON null to template object rather than NSNull (i.e. empty string, array, dictionary, 0)
    NSKJSONNullToEmpty,
    //! Map JSON null to nil rather than NSNull (for objects in template)
    NSKJSONNullToNil,
    //! Return error when encountering JSON null when another type was expected
    NSKJSONNullToError
};

@interface NSKJSONSchemaValidator : NSObject

//! @param templateObj an object describing the expected structure of the json.
//! @returns a schema validator using the provided template.
+ (NSKJSONSchemaValidator *)validatorWithTemplate:(id)templateObj;

//! Validates obj, setting error if obj is unexpected.
//! @returns either obj itself, nil, or a replacement object.
- (id)validateJSONObject:(id)obj error:(NSError **)error;

//! Defaults to NSKJSONNullToEmpty
@property NSKJSONNullHandling nullHandling;

@end

@interface NSKJSONOptional : NSObject

//! If a JSON dictionary field is optional
//! (meaning it may be omitted entirely, both key and value)
//! it may be wrapped in a NSKJSONOptional. When parsing, the field must either be absent
//! or if it exists it must conform to templateObj.
//! @param templateObj the expected the value to conform to.
//! @returns an optional object.
+ (instancetype)optionalWithTemplate:(id)templateObj;

@property (readonly) id templateObj;

@end

