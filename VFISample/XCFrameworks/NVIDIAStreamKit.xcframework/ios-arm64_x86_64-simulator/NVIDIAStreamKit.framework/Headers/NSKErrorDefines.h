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


#ifndef NSKErrorDefines_h
#define NSKErrorDefines_h

// See https://confluence.nvidia.com/display/GNCE/New+Schema+Format

MAP_ENTRY(0x80F40000, NSKErrorGeneric, "Generic Error.")
MAP_ENTRY(0x80F40002, NSKErrorInvalidParameters, "Invalid parameters.")

MAP_ENTRY(0xC0F41000, NSKErrorVTDecoderDecode, "Decoding failed.")

MAP_ENTRY(0xC0F41101, NSKErrorClientInitialize, "Stream Client initialization failed.")
MAP_ENTRY(0xC0F41201, NSKErrorNetworkTest, "Network test error")
MAP_ENTRY(0xC0F41301, NSKErrorLatencyTest, "Latency test error")

MAP_ENTRY(0xC0F41321, NSKErrorPauseFailed, "Pause Failed")

MAP_ENTRY(0xC0F41401, NSKErrorConfigurationFailed, "Configuration error")

MAP_ENTRY(0xC0F42000, NSKErrorBifrost, "Bifrost Error");
MAP_ENTRY(0xC0F42FFF, NSKErrorBifrostMax, "Bifrost Error (Max)");

#endif /* NSKErrorDefines_h */
