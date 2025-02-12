/* 
* SPDX-FileCopyrightText: Copyright (c) 2019 - 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved. 
* SPDX-License-Identifier: LicenseRef-NvidiaProprietary 
* 
* NVIDIA CORPORATION, its affiliates and licensors retain all intellectual 
* property and proprietary rights in and to this material, related 
* documentation and any modifications thereto. Any use, reproduction, 
* disclosure or distribution of this material and related documentation 
* without an express license agreement from NVIDIA CORPORATION or 
* its affiliates is strictly prohibited. 
*/ 

#pragma once

#include "nvoptix.h"
#include <stddef.h>

// defensive duplicate of OptixDeviceContextOptions because I have to modify it

typedef struct OptixDeviceContextOptions_105
{
    OptixLogCallback logCallbackFunction;
    void *logCallbackData;
    int logCallbackLevel;
    int validationMode;
} OptixDeviceContextOptions_105;

// table as in public docs but stripped of most structures (pointers to which have been replaced with opaque `void*`)

typedef struct OptixFunctionTable_105
{
    const char *(*optixGetErrorName)(OptixResult result);
    const char *(*optixGetErrorString)(OptixResult result);
    OptixResult (*optixDeviceContextCreate)(CUcontext fromContext, const OptixDeviceContextOptions_105 *options, OptixDeviceContext *context);
    OptixResult (*optixDeviceContextDestroy)(OptixDeviceContext context);
    OptixResult (*optixDeviceContextGetProperty)(OptixDeviceContext context, int property, void *value, size_t sizeInBytes);
    OptixResult (*optixDeviceContextSetLogCallback)(OptixDeviceContext context, OptixLogCallback callbackFunction, void *callbackData, unsigned int callbackLevel);
    OptixResult (*optixDeviceContextSetCacheEnabled)(OptixDeviceContext context, int enabled);
    OptixResult (*optixDeviceContextSetCacheLocation)(OptixDeviceContext context, const char *location);
    OptixResult (*optixDeviceContextSetCacheDatabaseSizes)(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark);
    OptixResult (*optixDeviceContextGetCacheEnabled)(OptixDeviceContext context, int *enabled);
    OptixResult (*optixDeviceContextGetCacheLocation)(OptixDeviceContext context, char *location, size_t locationSize);
    OptixResult (*optixDeviceContextGetCacheDatabaseSizes)(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark);
    OptixResult (*optixModuleCreate)(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module);
    OptixResult (*optixModuleCreateWithTasks)(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module, OptixTask *firstTask);
    OptixResult (*optixModuleGetCompilationState)(OptixModule module, int *state);
    OptixResult (*optixModuleDestroy)(OptixModule module);
    OptixResult (*optixBuiltinISModuleGet)(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule);
    OptixResult (*optixTaskExecute)(OptixTask task, OptixTask *additionalTasks, unsigned int maxNumAdditionalTasks, unsigned int *numAdditionalTasksCreated);
    OptixResult (*optixProgramGroupCreate)(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups);
    OptixResult (*optixProgramGroupDestroy)(OptixProgramGroup programGroup);
    OptixResult (*optixProgramGroupGetStackSize)(OptixProgramGroup programGroup, void *stackSizes, OptixPipeline pipeline);
    OptixResult (*optixPipelineCreate)(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline);
    OptixResult (*optixPipelineDestroy)(OptixPipeline pipeline);
    OptixResult (*optixPipelineSetStackSize)(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth);
    OptixResult (*optixAccelComputeMemoryUsage)(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes);
    OptixResult (*optixAccelBuild)(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties);
    OptixResult (*optixAccelGetRelocationInfo)(OptixDeviceContext context, OptixTraversableHandle handle, void *info);
    OptixResult (*optixCheckRelocationCompatibility)(OptixDeviceContext context, const void *info, int *compatible);
    OptixResult (*optixAccelRelocate)(OptixDeviceContext context, CUstream stream, const void *info, const void *relocateInputs, size_t numRelocateInputs, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle);
    OptixResult (*optixAccelCompact)(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle);
    OptixResult (*optixAccelEmitProperty)(OptixDeviceContext context, CUstream stream, OptixTraversableHandle handle, const void *emittedProperty);
    OptixResult (*optixConvertPointerToTraversableHandle)(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle);
    OptixResult (*optixOpacityMicromapArrayComputeMemoryUsage)(OptixDeviceContext context, const void *buildInput, void *bufferSizes);
    OptixResult (*optixOpacityMicromapArrayBuild)(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers);
    OptixResult (*optixOpacityMicromapArrayGetRelocationInfo)(OptixDeviceContext context, CUdeviceptr opacityMicromapArray, void *info);
    OptixResult (*optixOpacityMicromapArrayRelocate)(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr targetOpacityMicromapArray, size_t targetOpacityMicromapArraySizeInBytes);
    OptixResult (*optixDisplacementMicromapArrayComputeMemoryUsage)(OptixDeviceContext context, const void *buildInput, void *bufferSizes);
    OptixResult (*optixDisplacementMicromapArrayBuild)(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers);
    OptixResult (*optixClusterAccelComputeMemoryUsage)(OptixDeviceContext context, void *buildMode, const void *buildInput, void *bufferSizes);
    OptixResult (*optixClusterAccelBuild)(OptixDeviceContext context, CUstream stream, const void *buildModeDesc, const void *buildInput, CUdeviceptr argsArray, CUdeviceptr argsCount, unsigned int argsStrideInBytes);
    OptixResult (*optixSbtRecordPackHeader)(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer);
    OptixResult (*optixLaunch)(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth);
    OptixResult (*optixCoopVecMatrixConvert)(OptixDeviceContext context, CUstream stream, unsigned int numNetworks, const void *inputNetworkDescription, CUdeviceptr inputNetworks, size_t inputNetworkStrideInBytes, const void *outputNetworkDescription, CUdeviceptr outputNetworks, size_t outputNetworkStrideInBytes);
    OptixResult (*optixCoopVecMatrixComputeSize)(OptixDeviceContext context, unsigned int N, unsigned int K, void *elementType, void *layout, size_t rowColumnStrideInBytes, size_t *sizeInBytes);
    OptixResult (*optixDenoiserCreate)(OptixDeviceContext context, int modelKind, const void *options, OptixDenoiser *returnHandle);
    OptixResult (*optixDenoiserDestroy)(OptixDenoiser handle);
    OptixResult (*optixDenoiserComputeMemoryResources)(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes);
    OptixResult (*optixDenoiserSetup)(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserInvoke)(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *guideLayer, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserComputeIntensity)(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserComputeAverageColor)(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes);
    OptixResult (*optixDenoiserCreateWithUserModel)(OptixDeviceContext context, const void *data, size_t dataSizeInBytes, OptixDenoiser *returnHandle);
} OptixFunctionTable_105;

OptixResult __cdecl optixQueryFunctionTable_105(unsigned int numOptions, int *optionKeys, const void **optionValues, void *functionTable, size_t sizeOfTable);
