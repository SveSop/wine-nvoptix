/*
 * MIT License
 * Copyright (C) 2023-2025 Sveinar Søpler
 * Copyright (C) 2023 Krzysztof Bogacki
 * Copyright (C) 2022 Timothée Barral
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wine/debug.h"
#include "wine/unixlib.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix.h"
#include "nvoptix_private.h"
#include "nvoptix_105.h"

/* OptiX ABI = 105 / SDK 9.0.0 */

static const char *optixGetErrorName_105(OptixResult result)
{
    struct optixGetError_params _params = { result };
    TRACE("(%d)\n", result);
    return NVOPTIX_CALL(optixGetErrorName_105, &_params)
        ? "Unknown Error"
        : _params._result;
}

static const char *optixGetErrorString_105(OptixResult result)
{
    struct optixGetError_params _params = { result };
    TRACE("(%d)\n", result);
    return NVOPTIX_CALL(optixGetErrorString_105, &_params)
        ? "Unknown Error"
        : _params._result;
}

static OptixResult optixDeviceContextCreate_105(CUcontext fromContext, const OptixDeviceContextOptions_41 *options, OptixDeviceContext *context)
{
    struct optixDeviceContextCreate_105_params _params = { fromContext, options, context };
    TRACE("(%p, %p, %p)\n", fromContext, options, context);
    return NVOPTIX_CALL(optixDeviceContextCreate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextDestroy_105(OptixDeviceContext context)
{
    struct optixDeviceContextDestroy_105_params _params = { context };
    TRACE("(%p)\n", context);
    return NVOPTIX_CALL(optixDeviceContextDestroy_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextGetProperty_105(OptixDeviceContext context, int property, void *value, size_t sizeInBytes)
{
    struct optixDeviceContextGetProperty_105_params _params = { context, property, value, sizeInBytes };
    TRACE("(%p, %d, %p, %zu)\n", context, property, value, sizeInBytes);
    return NVOPTIX_CALL(optixDeviceContextGetProperty_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextSetLogCallback_105(OptixDeviceContext context, OptixLogCallback callbackFunction, void *callbackData, unsigned int callbackLevel)
{
    struct optixDeviceContextSetLogCallback_105_params _params = { context, callbackFunction, callbackData, callbackLevel };
    TRACE("(%p, %p, %p, %u)\n", context, callbackFunction, callbackData, callbackLevel);
    return NVOPTIX_CALL(optixDeviceContextSetLogCallback_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextSetCacheEnabled_105(OptixDeviceContext context, int enabled)
{
    struct optixDeviceContextSetCacheEnabled_105_params _params = { context, enabled };
    TRACE("(%p, %d)\n", context, enabled);
    return NVOPTIX_CALL(optixDeviceContextSetCacheEnabled_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextSetCacheLocation_105(OptixDeviceContext context, const char *location)
{
    struct optixDeviceContextSetCacheLocation_105_params _params = { context, location };
    TRACE("(%p, %s)\n", context, location);

    if (!location) return OPTIX_ERROR_DISK_CACHE_INVALID_PATH;

    WCHAR location_wide[MAX_PATH];

    MultiByteToWideChar(CP_ACP, 0, location, -1, location_wide, ARRAY_SIZE(location_wide));

    char *unix_location = wine_get_unix_file_name(location_wide);

    _params.location = unix_location;

    OptixResult result = NVOPTIX_CALL(optixDeviceContextSetCacheLocation_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;

    HeapFree(GetProcessHeap(), 0, unix_location);

    return result;
}

static OptixResult optixDeviceContextSetCacheDatabaseSizes_105(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark)
{
    struct optixDeviceContextSetCacheDatabaseSizes_105_params _params = { context, lowWaterMark, highWaterMark };
    TRACE("(%p, %zu, %zu)\n", context, lowWaterMark, highWaterMark);
    return NVOPTIX_CALL(optixDeviceContextSetCacheDatabaseSizes_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextGetCacheEnabled_105(OptixDeviceContext context, int *enabled)
{
    struct optixDeviceContextGetCacheEnabled_105_params _params = { context, enabled };
    TRACE("(%p, %p)\n", context, enabled);
    return NVOPTIX_CALL(optixDeviceContextGetCacheEnabled_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDeviceContextGetCacheLocation_105(OptixDeviceContext context, char *location, size_t locationSize)
{
    struct optixDeviceContextGetCacheLocation_105_params _params = { context, location, locationSize };
    TRACE("(%p, %p, %zu)\n", context, location, locationSize);

    OptixResult result = NVOPTIX_CALL(optixDeviceContextGetCacheLocation_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;

    if (result != OPTIX_SUCCESS) return result;

    WCHAR *dos_location = wine_get_dos_file_name(location);

    if (!WideCharToMultiByte(CP_ACP, 0, dos_location, -1, location, locationSize, NULL, NULL)) result = OPTIX_ERROR_INVALID_VALUE;

    HeapFree(GetProcessHeap(), 0, dos_location);

    return result;
}

static OptixResult optixDeviceContextGetCacheDatabaseSizes_105(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark)
{
    struct optixDeviceContextGetCacheDatabaseSizes_105_params _params = { context, lowWaterMark, highWaterMark };
    TRACE("(%p, %p, %p)\n", context, lowWaterMark, highWaterMark);
    return NVOPTIX_CALL(optixDeviceContextGetCacheDatabaseSizes_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixModuleCreate_105(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module)
{
    struct optixModuleCreate_105_params _params = { context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module };
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module);
    return NVOPTIX_CALL(optixModuleCreate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixModuleCreateWithTasks_105(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module, OptixTask *firstTask)
{
    struct optixModuleCreateWithTasks_105_params _params = { context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module, firstTask };
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module, firstTask);
    return NVOPTIX_CALL(optixModuleCreateWithTasks_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixModuleGetCompilationState_105(OptixModule module, int *state)
{
    struct optixModuleGetCompilationState_105_params _params = { module, state };
    TRACE("(%p, %p)\n", module, state);
    return NVOPTIX_CALL(optixModuleGetCompilationState_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixModuleDestroy_105(OptixModule module)
{
    struct optixModuleDestroy_105_params _params = { module };
    TRACE("(%p)\n", module);
    return NVOPTIX_CALL(optixModuleDestroy_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixBuiltinISModuleGet_105(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule)
{
    struct optixBuiltinISModuleGet_105_params _params = { context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule };
    TRACE("(%p, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
    return NVOPTIX_CALL(optixBuiltinISModuleGet_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixTaskExecute_105(OptixTask task, OptixTask *additionalTasks, unsigned int maxNumAdditionalTasks, unsigned int *numAdditionalTasksCreated)
{
    struct optixTaskExecute_105_params _params = { task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated };
    TRACE("(%p, %p, %u, %p)\n", task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated);
    return NVOPTIX_CALL(optixTaskExecute_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixProgramGroupCreate_105(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups)
{
    struct optixProgramGroupCreate_105_params _params = { context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups };
    TRACE("(%p, %p, %u, %p, %p, %p, %p)\n", context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
    return NVOPTIX_CALL(optixProgramGroupCreate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixProgramGroupDestroy_105(OptixProgramGroup programGroup)
{
    struct optixProgramGroupDestroy_105_params _params = { programGroup };
    TRACE("(%p)\n", programGroup);
    return NVOPTIX_CALL(optixProgramGroupDestroy_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixProgramGroupGetStackSize_105(OptixProgramGroup programGroup, void *stackSizes, OptixPipeline pipeline)
{
    struct optixProgramGroupGetStackSize_105_params _params = { programGroup, stackSizes, pipeline };
    TRACE("(%p, %p, %p)\n", programGroup, stackSizes, pipeline);
    return NVOPTIX_CALL(optixProgramGroupGetStackSize_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixPipelineCreate_105(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline)
{
    struct optixPipelineCreate_105_params _params = { context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline };
    TRACE("(%p, %p, %p, %p, %u, %p, %p, %p)\n", context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
    return NVOPTIX_CALL(optixPipelineCreate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixPipelineDestroy_105(OptixPipeline pipeline)
{
    struct optixPipelineDestroy_105_params _params = { pipeline };
    TRACE("(%p)\n", pipeline);
    return NVOPTIX_CALL(optixPipelineDestroy_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixPipelineSetStackSize_105(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth)
{
    struct optixPipelineSetStackSize_105_params _params = { pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth };
    TRACE("(%p, %u, %u, %u, %u)\n", pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
    return NVOPTIX_CALL(optixPipelineSetStackSize_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelComputeMemoryUsage_105(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes)
{
    struct optixAccelComputeMemoryUsage_105_params _params = { context, accelOptions, buildInputs, numBuildInputs, bufferSizes };
    TRACE("(%p, %p, %p, %u, %p)\n", context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
    return NVOPTIX_CALL(optixAccelComputeMemoryUsage_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelBuild_105(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties)
{
    struct optixAccelBuild_105_params _params = { context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties };
    TRACE("(%p, %p, %p, %p, %u, %p, %zu, %p, %zu, %p, %p, %u)\n", context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
    return NVOPTIX_CALL(optixAccelBuild_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelGetRelocationInfo_105(OptixDeviceContext context, OptixTraversableHandle handle, void *info)
{
    struct optixAccelGetRelocationInfo_105_params _params = { context, handle, info };
    TRACE("(%p, %llu, %p)\n", context, handle, info);
    return NVOPTIX_CALL(optixAccelGetRelocationInfo_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixCheckRelocationCompatibility_105(OptixDeviceContext context, const void *info, int *compatible)
{
    struct optixCheckRelocationCompatibility_105_params _params = { context, info, compatible };
    TRACE("(%p, %p, %p)\n", context, info, compatible);
    return NVOPTIX_CALL(optixCheckRelocationCompatibility_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelRelocate_105(OptixDeviceContext context, CUstream stream, const void *info, const void *relocateInputs, size_t numRelocateInputs, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle)
{
    struct optixAccelRelocate_105_params _params = { context, stream, info, relocateInputs, numRelocateInputs, targetAccel, targetAccelSizeInBytes, targetHandle };
    TRACE("(%p, %p, %p, %p, %zu, %p, %zu, %p)\n", context, stream, info, relocateInputs, numRelocateInputs, targetAccel, targetAccelSizeInBytes, targetHandle);
    return NVOPTIX_CALL(optixAccelRelocate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelCompact_105(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle)
{
    struct optixAccelCompact_105_params _params = { context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle };
    TRACE("(%p, %p, %llu, %p, %zu, %p)\n", context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
    return NVOPTIX_CALL(optixAccelCompact_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixAccelEmitProperty_105(OptixDeviceContext context, CUstream stream, OptixTraversableHandle handle, const void *emittedProperty)
{
    struct optixAccelEmitProperty_105_params _params = { context, stream, handle, emittedProperty };
    TRACE("(%p, %p, %llu, %p)\n", context, stream, handle, emittedProperty);
    return NVOPTIX_CALL(optixAccelEmitProperty_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixConvertPointerToTraversableHandle_105(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle)
{
    struct optixConvertPointerToTraversableHandle_105_params _params = { onDevice, pointer, traversableType, traversableHandle };
    TRACE("(%p, %p, %d, %p)\n", onDevice, pointer, traversableType, traversableHandle);
    return NVOPTIX_CALL(optixConvertPointerToTraversableHandle_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixOpacityMicromapArrayComputeMemoryUsage_105(OptixDeviceContext context, const void *buildInput, void *bufferSizes)
{
    struct optixOpacityMicromapArrayComputeMemoryUsage_105_params _params = { context, buildInput, bufferSizes };
    TRACE("(%p, %p, %p)\n", context, buildInput, bufferSizes);
    return NVOPTIX_CALL(optixOpacityMicromapArrayComputeMemoryUsage_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixOpacityMicromapArrayBuild_105(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers)
{
    struct optixOpacityMicromapArrayBuild_105_params _params = { context, stream, buildInput, buffers };
    TRACE("(%p, %p, %p, %p)\n", context, stream, buildInput, buffers);
    return NVOPTIX_CALL(optixOpacityMicromapArrayBuild_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixOpacityMicromapArrayGetRelocationInfo_105(OptixDeviceContext context, CUdeviceptr opacityMicromapArray, void *info)
{
    struct optixOpacityMicromapArrayGetRelocationInfo_105_params _params = { context, opacityMicromapArray, info };
    TRACE("(%p, %p, %p)\n", context, opacityMicromapArray, info);
    return NVOPTIX_CALL(optixOpacityMicromapArrayGetRelocationInfo_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixOpacityMicromapArrayRelocate_105(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr targetOpacityMicromapArray, size_t targetOpacityMicromapArraySizeInBytes)
{
    struct optixOpacityMicromapArrayRelocate_105_params _params = { context, stream, info, targetOpacityMicromapArray, targetOpacityMicromapArraySizeInBytes };
    TRACE("(%p, %p, %p, %p, %zu)\n", context, stream, info, targetOpacityMicromapArray, targetOpacityMicromapArraySizeInBytes);
    return NVOPTIX_CALL(optixOpacityMicromapArrayRelocate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDisplacementMicromapArrayComputeMemoryUsage_105(OptixDeviceContext context, const void *buildInput, void *bufferSizes)
{
    struct optixDisplacementMicromapArrayComputeMemoryUsage_105_params _params = { context, buildInput, bufferSizes };
    TRACE("(%p, %p, %p)\n", context, buildInput, bufferSizes);
    return NVOPTIX_CALL(optixDisplacementMicromapArrayComputeMemoryUsage_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDisplacementMicromapArrayBuild_105(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers)
{
    struct optixDisplacementMicromapArrayBuild_105_params _params = { context, stream, buildInput, buffers };
    TRACE("(%p, %p, %p, %p)\n", context, stream, buildInput, buffers);
    return NVOPTIX_CALL(optixDisplacementMicromapArrayBuild_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixClusterAccelComputeMemoryUsage_105(OptixDeviceContext context, void *buildMode, const void *buildInput, void *bufferSizes)
{
    struct optixClusterAccelComputeMemoryUsage_105_params _params = { context, buildMode, buildInput, bufferSizes };
    TRACE("%p, %p, %p, %p)\n", context, buildMode, buildInput, bufferSizes);
    return NVOPTIX_CALL(optixClusterAccelComputeMemoryUsage_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixClusterAccelBuild_105(OptixDeviceContext context, CUstream stream, const void *buildModeDesc, const void *buildInput, CUdeviceptr argsArray, CUdeviceptr argsCount, unsigned int argsStrideInBytes)
{
    struct optixClusterAccelBuild_105_params _params = { context, stream, buildModeDesc, buildInput, argsArray, argsCount, argsStrideInBytes };
    TRACE("(%p, %p, %p, %p, %p, %p, %u)\n", context, stream, buildModeDesc, buildInput, argsArray, argsCount, argsStrideInBytes);
    return NVOPTIX_CALL(optixClusterAccelBuild_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixSbtRecordPackHeader_105(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer)
{
    struct optixSbtRecordPackHeader_105_params _params = { programGroup, sbtRecordHeaderHostPointer };
    TRACE("(%p, %p)\n", programGroup, sbtRecordHeaderHostPointer);
    return NVOPTIX_CALL(optixSbtRecordPackHeader_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixLaunch_105(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth)
{
    struct optixLaunch_105_params _params = { pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth };
    TRACE("(%p, %p, %p, %zu, %p, %u, %u, %u)\n", pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
    return NVOPTIX_CALL(optixLaunch_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixCoopVecMatrixConvert_105(OptixDeviceContext context, CUstream stream, unsigned int numNetworks, const void *inputNetworkDescription, CUdeviceptr inputNetworks, size_t inputNetworkStrideInBytes, const void *outputNetworkDescription, CUdeviceptr outputNetworks, size_t outputNetworkStrideInBytes)
{
    struct optixCoopVecMatrixConvert_105_params _params = { context, stream, numNetworks, inputNetworkDescription, inputNetworks, inputNetworkStrideInBytes, outputNetworkDescription, outputNetworks, outputNetworkStrideInBytes };
    TRACE("(%p, %p, %u, %p, %p, %zu, %p, %p, %zu)\n", context, stream, numNetworks, inputNetworkDescription, inputNetworks, inputNetworkStrideInBytes, outputNetworkDescription, outputNetworks, outputNetworkStrideInBytes);
    return NVOPTIX_CALL(optixCoopVecMatrixConvert_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixCoopVecMatrixComputeSize_105(OptixDeviceContext context, unsigned int N, unsigned int K, void *elementType, void *layout, size_t rowColumnStrideInBytes, size_t *sizeInBytes)
{
    struct optixCoopVecMatrixComputeSize_105_params _params = { context, N, K, elementType, layout, rowColumnStrideInBytes, sizeInBytes };
    TRACE("(%p, %u, %u, %p, %p, %zu, %p)\n", context, N, K, elementType, layout, rowColumnStrideInBytes, sizeInBytes);
    return NVOPTIX_CALL(optixCoopVecMatrixComputeSize_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserCreate_105(OptixDeviceContext context, int modelKind, const void *options, OptixDenoiser *returnHandle)
{
    struct optixDenoiserCreate_105_params _params = { context, modelKind, options, returnHandle };
    TRACE("(%p, %d, %p, %p)\n", context, modelKind, options, returnHandle);
    return NVOPTIX_CALL(optixDenoiserCreate_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserDestroy_105(OptixDenoiser handle)
{
    struct optixDenoiserDestroy_105_params _params = { handle };
    TRACE("(%p)\n", handle);
    return NVOPTIX_CALL(optixDenoiserDestroy_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserComputeMemoryResources_105(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes)
{
    struct optixDenoiserComputeMemoryResources_105_params _params = { handle, maximumInputWidth, maximumInputHeight, returnSizes };
    TRACE("(%p, %u, %u, %p)\n", handle, maximumInputWidth, maximumInputHeight, returnSizes);
    return NVOPTIX_CALL(optixDenoiserComputeMemoryResources_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserSetup_105(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    struct optixDenoiserSetup_105_params _params = { denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes };
    TRACE("(%p, %p, %u, %u, %p, %zu, %p, %zu)\n", denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
    return NVOPTIX_CALL(optixDenoiserSetup_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserInvoke_105(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *guideLayer, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    struct optixDenoiserInvoke_105_params _params = { denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes };
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %u, %u, %u, %p, %zu)\n", denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes);
    return NVOPTIX_CALL(optixDenoiserInvoke_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserComputeIntensity_105(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    struct optixDenoiserComputeIntensity_105_params _params = { handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes };
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
    return NVOPTIX_CALL(optixDenoiserComputeIntensity_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserComputeAverageColor_105(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    struct optixDenoiserComputeAverageColor_105_params _params = { handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes };
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
    return NVOPTIX_CALL(optixDenoiserComputeAverageColor_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

static OptixResult optixDenoiserCreateWithUserModel_105(OptixDeviceContext context, const void *data, size_t dataSizeInBytes, OptixDenoiser *returnHandle)
{
    struct optixDenoiserCreateWithUserModel_105_params _params = { context, data, dataSizeInBytes, returnHandle };
    TRACE("(%p, %p, %zu, %p)\n", context, data, dataSizeInBytes, returnHandle);
    return NVOPTIX_CALL(optixDenoiserCreateWithUserModel_105, &_params)
        ? OPTIX_ERROR_INTERNAL_ERROR
        : _params._result;
}

OptixFunctionTable_105 optixFunctionTable_105 = {
#define ASSIGN_FUNCPTR(f) .f = f ## _105
    ASSIGN_FUNCPTR(optixGetErrorName),
    ASSIGN_FUNCPTR(optixGetErrorString),
    ASSIGN_FUNCPTR(optixDeviceContextCreate),
    ASSIGN_FUNCPTR(optixDeviceContextDestroy),
    ASSIGN_FUNCPTR(optixDeviceContextGetProperty),
    ASSIGN_FUNCPTR(optixDeviceContextSetLogCallback),
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheEnabled),
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheLocation),
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheDatabaseSizes),
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheEnabled),
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheLocation),
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheDatabaseSizes),
    ASSIGN_FUNCPTR(optixModuleCreate),
    ASSIGN_FUNCPTR(optixModuleCreateWithTasks),
    ASSIGN_FUNCPTR(optixModuleGetCompilationState),
    ASSIGN_FUNCPTR(optixModuleDestroy),
    ASSIGN_FUNCPTR(optixBuiltinISModuleGet),
    ASSIGN_FUNCPTR(optixTaskExecute),
    ASSIGN_FUNCPTR(optixProgramGroupCreate),
    ASSIGN_FUNCPTR(optixProgramGroupDestroy),
    ASSIGN_FUNCPTR(optixProgramGroupGetStackSize),
    ASSIGN_FUNCPTR(optixPipelineCreate),
    ASSIGN_FUNCPTR(optixPipelineDestroy),
    ASSIGN_FUNCPTR(optixPipelineSetStackSize),
    ASSIGN_FUNCPTR(optixAccelComputeMemoryUsage),
    ASSIGN_FUNCPTR(optixAccelBuild),
    ASSIGN_FUNCPTR(optixAccelGetRelocationInfo),
    ASSIGN_FUNCPTR(optixCheckRelocationCompatibility),
    ASSIGN_FUNCPTR(optixAccelRelocate),
    ASSIGN_FUNCPTR(optixAccelCompact),
    ASSIGN_FUNCPTR(optixAccelEmitProperty),
    ASSIGN_FUNCPTR(optixConvertPointerToTraversableHandle),
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayComputeMemoryUsage),
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayBuild),
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayGetRelocationInfo),
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayRelocate),
    ASSIGN_FUNCPTR(optixDisplacementMicromapArrayComputeMemoryUsage),
    ASSIGN_FUNCPTR(optixDisplacementMicromapArrayBuild),
    ASSIGN_FUNCPTR(optixClusterAccelComputeMemoryUsage),
    ASSIGN_FUNCPTR(optixClusterAccelBuild),
    ASSIGN_FUNCPTR(optixSbtRecordPackHeader),
    ASSIGN_FUNCPTR(optixLaunch),
    ASSIGN_FUNCPTR(optixCoopVecMatrixConvert),
    ASSIGN_FUNCPTR(optixCoopVecMatrixComputeSize),
    ASSIGN_FUNCPTR(optixDenoiserCreate),
    ASSIGN_FUNCPTR(optixDenoiserDestroy),
    ASSIGN_FUNCPTR(optixDenoiserComputeMemoryResources),
    ASSIGN_FUNCPTR(optixDenoiserSetup),
    ASSIGN_FUNCPTR(optixDenoiserInvoke),
    ASSIGN_FUNCPTR(optixDenoiserComputeIntensity),
    ASSIGN_FUNCPTR(optixDenoiserComputeAverageColor),
    ASSIGN_FUNCPTR(optixDenoiserCreateWithUserModel),
#undef ASSIGN_FUNCPTR
};
