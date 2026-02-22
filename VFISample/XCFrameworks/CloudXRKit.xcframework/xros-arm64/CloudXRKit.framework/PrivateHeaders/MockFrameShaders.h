// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#ifndef MockFrameShaders_h
#define MockFrameShaders_h

#include <simd/simd.h>

typedef enum {
    BufferIndexMeshPositions = 0,
    BufferIndexMeshNormals = 1,
    BufferIndexUniforms = 2,
    BufferIndexEyeIdx = 3
} BufferIndex;

typedef enum {
    VertexAttributePosition = 0,
    VertexAttributeNormal = 1,
} VertexAttribute;

typedef struct {
    matrix_float4x4 modelMatrix;
    matrix_float4x4 viewMatrix;
    matrix_float4x4 projectionMatrix;
} Uniforms;

typedef struct
{
    Uniforms uniforms[2];
} UniformsArray;

#endif /* MockFrameShaders_h */
