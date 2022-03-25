/*
 * MIT License
 * Copyright (C) 2022 Sveinar Søpler
 * Copyright (C) 2022 Krzysztof Bogacki
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

#include <dlfcn.h>
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix.h"
#include "nvoptix_41.h"

static OptixFunctionTable_41 optixFunctionTable_41;

/* OptiX ABI = 41 / SDK 7.2.0 */

static const char *__cdecl optixGetErrorName_41(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_41.optixGetErrorName(result);
}

static const char *__cdecl optixGetErrorString_41(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_41.optixGetErrorString(result);
}

static OptixResult __cdecl optixDeviceContextCreate_41(CUcontext fromContext, const OptixDeviceContextOptions_41 *options, OptixDeviceContext *context)
{
    TRACE("(%p, %p, %p)\n", fromContext, options, context);

    OptixDeviceContextOptions_41 opts = *options;

    if (opts.logCallbackFunction || opts.logCallbackLevel)
    {
        FIXME("log callback not supported\n");
        opts.logCallbackFunction = NULL;
        opts.logCallbackLevel = 0;
    }

    return optixFunctionTable_41.optixDeviceContextCreate(fromContext, &opts, context);
}

static OptixResult __cdecl optixDeviceContextDestroy_41(OptixDeviceContext context)
{
    TRACE("(%p)\n", context);
    return optixFunctionTable_41.optixDeviceContextDestroy(context);
}

static OptixResult __cdecl optixDeviceContextGetProperty_41(OptixDeviceContext context, int property, void *value, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", context, property, value, sizeInBytes);
    return optixFunctionTable_41.optixDeviceContextGetProperty(context, property, value, sizeInBytes);
}

static OptixResult __cdecl optixDeviceContextSetLogCallback_41(OptixDeviceContext context, OptixLogCallback callbackFunction, void *callbackData, unsigned int callbackLevel)
{
    FIXME("(%p, %p, %p, %u): stub\n", context, callbackFunction, callbackData, callbackLevel);
    return OPTIX_SUCCESS;
}

static OptixResult __cdecl optixDeviceContextSetCacheEnabled_41(OptixDeviceContext context, int enabled)
{
    TRACE("(%p, %d)\n", context, enabled);
    return optixFunctionTable_41.optixDeviceContextSetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextSetCacheLocation_41(OptixDeviceContext context, const char *location)
{
    TRACE("(%p, %s)\n", context, location);

    if (!location) return OPTIX_ERROR_DISK_CACHE_INVALID_PATH;

    WCHAR location_wide[MAX_PATH];

    MultiByteToWideChar(CP_ACP, 0, location, -1, location_wide, ARRAY_SIZE(location_wide));

    char *unix_location = wine_get_unix_file_name(location_wide);

    OptixResult result = optixFunctionTable_41.optixDeviceContextSetCacheLocation(context, unix_location);

    HeapFree(GetProcessHeap(), 0, unix_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextSetCacheDatabaseSizes_41(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark)
{
    TRACE("(%p, %zu, %zu)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_41.optixDeviceContextSetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixDeviceContextGetCacheEnabled_41(OptixDeviceContext context, int *enabled)
{
    TRACE("(%p, %p)\n", context, enabled);
    return optixFunctionTable_41.optixDeviceContextGetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextGetCacheLocation_41(OptixDeviceContext context, char *location, size_t locationSize)
{
    TRACE("(%p, %p, %zu)\n", context, location, locationSize);

    OptixResult result = optixFunctionTable_41.optixDeviceContextGetCacheLocation(context, location, locationSize);

    if (result != OPTIX_SUCCESS) return result;

    WCHAR *dos_location = wine_get_dos_file_name(location);

    if (!WideCharToMultiByte(CP_ACP, 0, dos_location, -1, location, locationSize, NULL, NULL)) result = OPTIX_ERROR_INVALID_VALUE;

    HeapFree(GetProcessHeap(), 0, dos_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextGetCacheDatabaseSizes_41(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark)
{
    TRACE("(%p, %p, %p)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_41.optixDeviceContextGetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixModuleCreateFromPTX_41(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *PTX, size_t PTXsize, char *logString, size_t *logStringSize, OptixModule *module)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
    return optixFunctionTable_41.optixModuleCreateFromPTX(context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
}

static OptixResult __cdecl optixModuleDestroy_41(OptixModule module)
{
    TRACE("(%p)\n", module);
    return optixFunctionTable_41.optixModuleDestroy(module);
}

static OptixResult __cdecl optixBuiltinISModuleGet_41(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule)
{
    TRACE("(%p, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
    return optixFunctionTable_41.optixBuiltinISModuleGet(context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
}

static OptixResult __cdecl optixProgramGroupCreate_41(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups)
{
    TRACE("(%p, %p, %u, %p, %p, %p, %p)\n", context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
    return optixFunctionTable_41.optixProgramGroupCreate(context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
}

static OptixResult __cdecl optixProgramGroupDestroy_41(OptixProgramGroup programGroup)
{
    TRACE("(%p)\n", programGroup);
    return optixFunctionTable_41.optixProgramGroupDestroy(programGroup);
}

static OptixResult __cdecl optixProgramGroupGetStackSize_41(OptixProgramGroup programGroup, void *stackSizes)
{
    TRACE("(%p, %p)\n", programGroup, stackSizes);
    return optixFunctionTable_41.optixProgramGroupGetStackSize(programGroup, stackSizes);
}

static OptixResult __cdecl optixPipelineCreate_41(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %p, %p)\n", context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
    return optixFunctionTable_41.optixPipelineCreate(context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
}

static OptixResult __cdecl optixPipelineDestroy_41(OptixPipeline pipeline)
{
    TRACE("(%p)\n", pipeline);
    return optixFunctionTable_41.optixPipelineDestroy(pipeline);
}

static OptixResult __cdecl optixPipelineSetStackSize_41(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth)
{
    TRACE("(%p, %u, %u, %u, %u)\n", pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
    return optixFunctionTable_41.optixPipelineSetStackSize(pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
}

static OptixResult __cdecl optixAccelComputeMemoryUsage_41(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes)
{
    TRACE("(%p, %p, %p, %u, %p)\n", context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
    return optixFunctionTable_41.optixAccelComputeMemoryUsage(context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
}

static OptixResult __cdecl optixAccelBuild_41(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %zu, %p, %zu, %p, %p, %u)\n", context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
    return optixFunctionTable_41.optixAccelBuild(context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
}

static OptixResult __cdecl optixAccelGetRelocationInfo_41(OptixDeviceContext context, OptixTraversableHandle handle, void *info)
{
    TRACE("(%p, %llu, %p)\n", context, handle, info);
    return optixFunctionTable_41.optixAccelGetRelocationInfo(context, handle, info);
}

static OptixResult __cdecl optixAccelCheckRelocationCompatibility_41(OptixDeviceContext context, const void *info, int *compatible)
{
    TRACE("(%p, %p, %p)\n", context, info, compatible);
    return optixFunctionTable_41.optixAccelCheckRelocationCompatibility(context, info, compatible);
}

static OptixResult __cdecl optixAccelRelocate_41(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr instanceTraversableHandles, size_t numInstanceTraversableHandles, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %zu, %p)\n", context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
    return optixFunctionTable_41.optixAccelRelocate(context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
}

static OptixResult __cdecl optixAccelCompact_41(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle)
{
    TRACE("(%p, %p, %llu, %p, %zu, %p)\n", context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
    return optixFunctionTable_41.optixAccelCompact(context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
}

static OptixResult __cdecl optixConvertPointerToTraversableHandle_41(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle)
{
    TRACE("(%p, %p, %d, %p)\n", onDevice, pointer, traversableType, traversableHandle);
    return optixFunctionTable_41.optixConvertPointerToTraversableHandle(onDevice, pointer, traversableType, traversableHandle);
}

static OptixResult __cdecl optixSbtRecordPackHeader_41(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer)
{
    TRACE("(%p, %p)\n", programGroup, sbtRecordHeaderHostPointer);
    return optixFunctionTable_41.optixSbtRecordPackHeader(programGroup, sbtRecordHeaderHostPointer);
}

static OptixResult __cdecl optixLaunch_41(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth)
{
    TRACE("(%p, %p, %p, %zu, %p, %u, %u, %u)\n", pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
    return optixFunctionTable_41.optixLaunch(pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
}

static OptixResult __cdecl optixDenoiserCreate_41(OptixDeviceContext context, const void *options, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %p, %p)\n", context, options, returnHandle);
    return optixFunctionTable_41.optixDenoiserCreate(context, options, returnHandle);
}

static OptixResult __cdecl optixDenoiserDestroy_41(OptixDenoiser handle)
{
    TRACE("(%p)\n", handle);
    return optixFunctionTable_41.optixDenoiserDestroy(handle);
}

static OptixResult __cdecl optixDenoiserComputeMemoryResources_41(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes)
{
    TRACE("(%p, %u, %u, %p)\n", handle, maximumInputWidth, maximumInputHeight, returnSizes);
    return optixFunctionTable_41.optixDenoiserComputeMemoryResources(handle, maximumInputWidth, maximumInputHeight, returnSizes);
}

static OptixResult __cdecl optixDenoiserSetup_41(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %u, %u, %p, %zu, %p, %zu)\n", denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
    return optixFunctionTable_41.optixDenoiserSetup(denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserInvoke_41(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, const void *outputLayer, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %u, %u, %u, %p, %p, %zu)\n", denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, layers, numLayers, inputOffsetX, inputOffsetY, outputLayer, scratch, scratchSizeInBytes);
    return optixFunctionTable_41.optixDenoiserInvoke(denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, layers, numLayers, inputOffsetX, inputOffsetY, outputLayer, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserSetModel_41(OptixDenoiser handle, int kind, void *data, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", handle, kind, data, sizeInBytes);
    return optixFunctionTable_41.optixDenoiserSetModel(handle, kind, data, sizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeIntensity_41(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
    return optixFunctionTable_41.optixDenoiserComputeIntensity(handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeAverageColor_41(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
    return optixFunctionTable_41.optixDenoiserComputeAverageColor(handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
}

OptixResult __cdecl optixQueryFunctionTable_41(
        unsigned int numOptions,
        int *optionKeys,
        const void **optionValues,
        void *functionTable,
        size_t sizeOfTable)
{
    if (sizeOfTable != sizeof(OptixFunctionTable_41)) return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;

    OptixResult result = poptixQueryFunctionTable(41, numOptions, optionKeys, optionValues, &optixFunctionTable_41, sizeOfTable);

    if (result != OPTIX_SUCCESS) return result;

    OptixFunctionTable_41 *table = functionTable;

#define ASSIGN_FUNCPTR(f) *(void**)(&table->f) = (void*)&f ## _41

    ASSIGN_FUNCPTR(optixGetErrorName);
    ASSIGN_FUNCPTR(optixGetErrorString);
    ASSIGN_FUNCPTR(optixDeviceContextCreate);
    ASSIGN_FUNCPTR(optixDeviceContextDestroy);
    ASSIGN_FUNCPTR(optixDeviceContextGetProperty);
    ASSIGN_FUNCPTR(optixDeviceContextSetLogCallback);
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheEnabled);
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheLocation);
    ASSIGN_FUNCPTR(optixDeviceContextSetCacheDatabaseSizes);
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheEnabled);
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheLocation);
    ASSIGN_FUNCPTR(optixDeviceContextGetCacheDatabaseSizes);
    ASSIGN_FUNCPTR(optixModuleCreateFromPTX);
    ASSIGN_FUNCPTR(optixModuleDestroy);
    ASSIGN_FUNCPTR(optixBuiltinISModuleGet);
    ASSIGN_FUNCPTR(optixProgramGroupCreate);
    ASSIGN_FUNCPTR(optixProgramGroupDestroy);
    ASSIGN_FUNCPTR(optixProgramGroupGetStackSize);
    ASSIGN_FUNCPTR(optixPipelineCreate);
    ASSIGN_FUNCPTR(optixPipelineDestroy);
    ASSIGN_FUNCPTR(optixPipelineSetStackSize);
    ASSIGN_FUNCPTR(optixAccelComputeMemoryUsage);
    ASSIGN_FUNCPTR(optixAccelBuild);
    ASSIGN_FUNCPTR(optixAccelGetRelocationInfo);
    ASSIGN_FUNCPTR(optixAccelCheckRelocationCompatibility);
    ASSIGN_FUNCPTR(optixAccelRelocate);
    ASSIGN_FUNCPTR(optixAccelCompact);
    ASSIGN_FUNCPTR(optixConvertPointerToTraversableHandle);
    ASSIGN_FUNCPTR(optixSbtRecordPackHeader);
    ASSIGN_FUNCPTR(optixLaunch);
    ASSIGN_FUNCPTR(optixDenoiserCreate);
    ASSIGN_FUNCPTR(optixDenoiserDestroy);
    ASSIGN_FUNCPTR(optixDenoiserComputeMemoryResources);
    ASSIGN_FUNCPTR(optixDenoiserSetup);
    ASSIGN_FUNCPTR(optixDenoiserInvoke);
    ASSIGN_FUNCPTR(optixDenoiserSetModel);
    ASSIGN_FUNCPTR(optixDenoiserComputeIntensity);
    ASSIGN_FUNCPTR(optixDenoiserComputeAverageColor);

#undef ASSIGN_FUNCPTR

    return OPTIX_SUCCESS;
}

