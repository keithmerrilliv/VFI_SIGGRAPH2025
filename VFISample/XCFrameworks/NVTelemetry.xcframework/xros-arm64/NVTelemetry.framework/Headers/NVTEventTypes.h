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


#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, NVTGDPRConsent) {
    NVTGDPRConsentNone          = 0,
    NVTGDPRConsentFunctional    = (1 << 0),
    NVTGDPRConsentTechnical     = (1 << 1),
    NVTGDPRConsentBehavioral    = (1 << 2),
    NVTGDPRConsentRequired      = (1 << 3),

    /// If set, any of the above consents are provided to telemetry server as :Temp
    NVTGDPRConsentTemporary     = (1 << 15),
};

typedef NS_ENUM(NSUInteger, NVTService) {
    NVTServiceNone = 0,
    NVTServiceTelemetry,
    NVTServiceFeedback
};
