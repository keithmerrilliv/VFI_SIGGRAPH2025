// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#ifndef FrameKernels_h
#define FrameKernels_h

// Sigh... needed for Swift to understand anything more then just float.
#ifndef __METAL_VERSION__
#include <simd/simd.h>
#define float4 simd_float4
#endif

// Buffer indices used by computeFrame().
typedef enum {
    ComputeFrameBuffer_DisplacementParams = 0,
    ComputeFrameBuffer_FoveationParams = 1,
    ComputeFrameBuffer_FoveationFlag = 2,
} ComputeFrameBuffer;

// Texture indices used by computeFrame().
typedef enum {
    ComputeFrameTexture_InY = 0,
    ComputeFrameTexture_InCbCr = 1,
    ComputeFrameTexture_InAlpha = 2,
    ComputeFrameTexture_InDepth = 3,
    ComputeFrameTexture_OutRGBA = 4,
    ComputeFrameTexture_OutDepth = 5,
    ComputeFrameTexture_OutDisplacement = 6,
} ComputeFrameTexture;

// Texture indices used by computeDefoveatedVert().
typedef enum {
    ComputeVert_gridResolution = 0,
    ComputeVert_planeSizeInMeters = 1,
    ComputeVert_desc2D = 2,
    ComputeVert_isFoveatedAsUInt = 3,
    ComputeVert_textureCoordinates = 4,
    ComputeVert_positions = 5,
} ComputeVert;

struct XRPiecewiseQuadraticWarpDesc
{
    // left parabola: al * x^2 + bl * x + cl
    float al, bl, cl;
    // middle linear piece: am * x + bm.  am should give 1:1 pixel mapping between warped size and full size.
    float am, bm;
    // right parabola: al * x^2 + bl * x + cl
    float ar, br, cr;

    // points where left and right switch over from quadratic to linear (the x value where we switch)
    float switchLeft, switchRight;
    // same, in inverted space (the f(x) value where we switch)
    float invSwitchLeft, invSwitchRight;
};

struct XRPiecewiseQuadraticWarpDesc2D
{
    struct XRPiecewiseQuadraticWarpDesc xWarp;
    struct XRPiecewiseQuadraticWarpDesc yWarp;
};


// Buffer indices used by computeDisplacement().
typedef enum {
    ComputeDisplacementBuffer_DisplacementParams = 0
} ComputeDisplacementBuffer;

// Texture indices used by computeDisplacement().
typedef enum {
    ComputeDisplacementTexture_InDepth = 0,
    ComputeDisplacementTexture_OutDisplacement = 1,
} ComputeDisplacementTexture;

typedef struct DisplacementParams
{
    float   planeDistance;          // Distance from camera to projection plane
    float   near;                   // Near and far plane information
    float   far;
    float   epsilonDepthBias;
    
    float   depthMaxValue;
    int32_t infFar;
    int32_t reversed;
    int32_t premultiplyByAlpha;

    int32_t swapDepthAndAlpha;
    int32_t vizValues;              // 0 = Normal/Disabled, 1 = Alpha, 2 = Depth, 3 = RGB (alpha 1.0)
    int32_t swapUpperleft;
    int32_t quantizeDepth;
} DisplacementParams;

typedef struct EffectParams
{
    /*
     * "Matrix" (2x4) to go from world position to noise UV.
     *   Can also contain scale.
     *   Forth component is multiplied with 1 for offset.
     */
    float4 worldPosNoiseToU;
    float4 worldPosNoiseToV;

    // Sweep normal, or direction, last component unused.
    float4 sweepNormal;
    // Sweep starting point, last component unused.
    float4 sweepPoint;

    // Sweep sphere center point.
    float4 spherePoint;

    // The color of the effect highlight, last component unused.
    float4 effectHighlightColor;

    // The color of the effect that fades to pixel color, last component unused.
    float4 effectFadeColor;

    // At which time value should the global alpha start increasing.
    float globalAlphaStart;

    // At which time value should the global alpha stop increasing.
    float globalAlphaStop;

    // Length of sweep.
    float sweepLength;

    // The length of the color effect (in normalized cordinates).
    float sweepEffectLength;

    // The length of the sphere effect.
    float sphereLength;

    // The length of the color effect (in normalized cordinates).
    float sphereEffectLength;

    // The highlight effect cutoff value.
    float effectHighlightCutoffValue;

    // Value that controls the animation, in the [0..1] to the shader.
    float time;

    // Which effect to use.
    int32_t effectSelector;

} EffectParams;

#endif /* FrameKernels_h */
