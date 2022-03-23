/*
 * Copyright (c) 2020 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and proprietary
 * rights in and to this software, related documentation and any modifications thereto.
 * Any use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from NVIDIA Corporation is strictly
 * prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
 * AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
 * SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
 * BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
 * INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES
 */

#pragma once

#include "nvoptix_types.h"
#include <stddef.h>

// defensive duplicate of OptixDeviceContextOptions because I have to modify it

typedef void (*OptixLogCallback_47)(unsigned int level, const char *tag, const char *message, void *cbdata);

typedef struct OptixDeviceContextOptions_47
{
    OptixLogCallback_47 logCallbackFunction;
    void *logCallbackData;
    int logCallbackLevel;
    int validationMode;
} OptixDeviceContextOptions_47;

// table as in public docs but stripped of most structures (pointers to which have been replaced with opaque `void*`)

typedef struct OptixFunctionTable_47
{
    const char *(*optixGetErrorName)(OptixResult result);
    const char *(*optixGetErrorString)(OptixResult result);
    OptixResult (*optixDeviceContextCreate)(CUcontext fromContext, const OptixDeviceContextOptions_47 *options, OptixDeviceContext *context);
    OptixResult (*optixDeviceContextDestroy)(OptixDeviceContext context);
    OptixResult (*optixDeviceContextGetProperty)(OptixDeviceContext context, int property, void *value, size_t sizeInBytes);
    OptixResult (*optixDeviceContextSetLogCallback)(OptixDeviceContext context, OptixLogCallback_47 callbackFunction, void *callbackData, unsigned int callbackLevel);
    OptixResult (*optixDeviceContextSetCacheEnabled)(OptixDeviceContext context, int enabled);
    OptixResult (*optixDeviceContextSetCacheLocation)(OptixDeviceContext context, const char *location);
    OptixResult (*optixDeviceContextSetCacheDatabaseSizes)(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark);
    OptixResult (*optixDeviceContextGetCacheEnabled)(OptixDeviceContext context, int *enabled);
    OptixResult (*optixDeviceContextGetCacheLocation)(OptixDeviceContext context, char *location, size_t locationSize);
    OptixResult (*optixDeviceContextGetCacheDatabaseSizes)(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark);
    OptixResult (*optixModuleCreateFromPTX)(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *PTX, size_t PTXsize, char *logString, size_t *logStringSize, OptixModule *module);
    OptixResult (*optixModuleDestroy)(OptixModule module);
    OptixResult (*optixBuiltinISModuleGet)(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule);
    OptixResult (*optixProgramGroupCreate)(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups);
    OptixResult (*optixProgramGroupDestroy)(OptixProgramGroup programGroup);
    OptixResult (*optixProgramGroupGetStackSize)(OptixProgramGroup programGroup, void *stackSizes);
    OptixResult (*optixPipelineCreate)(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline);
    OptixResult (*optixPipelineDestroy)(OptixPipeline pipeline);
    OptixResult (*optixPipelineSetStackSize)(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth);
    OptixResult (*optixAccelComputeMemoryUsage)(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes);
    OptixResult (*optixAccelBuild)(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties);
    OptixResult (*optixAccelGetRelocationInfo)(OptixDeviceContext context, OptixTraversableHandle handle, void *info);
    OptixResult (*optixAccelCheckRelocationCompatibility)(OptixDeviceContext context, const void *info, int *compatible);
    OptixResult (*optixAccelRelocate)(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr instanceTraversableHandles, size_t numInstanceTraversableHandles, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle);
    OptixResult (*optixAccelCompact)(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle);
    OptixResult (*optixConvertPointerToTraversableHandle)(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle);
    OptixResult (*optixSbtRecordPackHeader)(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer);
    OptixResult (*optixLaunch)(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth);
    OptixResult (*optixDenoiserCreate)(OptixDeviceContext context, int modelKind, const void *options, OptixDenoiser *returnHandle);
    OptixResult (*optixDenoiserDestroy)(OptixDenoiser handle);
    OptixResult (*optixDenoiserComputeMemoryResources)(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes);
    OptixResult (*optixDenoiserSetup)(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserInvoke)(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *guideLayer, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserComputeIntensity)(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserComputeAverageColor)(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserCreateWithUserModel)(OptixDeviceContext context, const void *data, size_t dataSizeInBytes, OptixDenoiser *returnHandle);
} OptixFunctionTable_47;
