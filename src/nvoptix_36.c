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
#include "nvoptix_36.h"

static OptixFunctionTable_36 optixFunctionTable_36;

/* OptiX ABI = 36 / SDK 7.1.0 */

static const char *__cdecl optixGetErrorName_36(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_36.optixGetErrorName(result);
}

static const char *__cdecl optixGetErrorString_36(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_36.optixGetErrorString(result);
}

static OptixResult __cdecl optixDeviceContextCreate_36(CUcontext fromContext, const OptixDeviceContextOptions_36 *options, OptixDeviceContext *context)
{
    TRACE("(%p, %p, %p)\n", fromContext, options, context);

    OptixDeviceContextOptions_36 opts = *options;

    if (opts.logCallbackFunction)
    {
        if (callbacks_enabled())
        {
            opts.logCallbackData = wrap_callback(opts.logCallbackFunction, opts.logCallbackData);
            opts.logCallbackFunction = log_callback;
        }
        else
        {
            WARN("log callbacks disabled\n");
            opts.logCallbackFunction = NULL;
        }
    }

    return optixFunctionTable_36.optixDeviceContextCreate(fromContext, &opts, context);
}

static OptixResult __cdecl optixDeviceContextDestroy_36(OptixDeviceContext context)
{
    TRACE("(%p)\n", context);
    return optixFunctionTable_36.optixDeviceContextDestroy(context);
}

static OptixResult __cdecl optixDeviceContextGetProperty_36(OptixDeviceContext context, int property, void *value, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", context, property, value, sizeInBytes);
    return optixFunctionTable_36.optixDeviceContextGetProperty(context, property, value, sizeInBytes);
}

static OptixResult __cdecl optixDeviceContextSetLogCallback_36(OptixDeviceContext context, OptixLogCallback callbackFunction, void *callbackData, unsigned int callbackLevel)
{
    TRACE("(%p, %p, %p, %u)\n", context, callbackFunction, callbackData, callbackLevel);

    if (callbackFunction)
    {
        if (callbacks_enabled())
        {
            callbackData = wrap_callback(callbackFunction, callbackData);
            callbackFunction = log_callback;
        }
        else
        {
            WARN("log callbacks disabled\n");
            return OPTIX_SUCCESS;
        }
    }

    return optixFunctionTable_36.optixDeviceContextSetLogCallback(context, callbackFunction, callbackData, callbackLevel);
}

static OptixResult __cdecl optixDeviceContextSetCacheEnabled_36(OptixDeviceContext context, int enabled)
{
    TRACE("(%p, %d)\n", context, enabled);
    return optixFunctionTable_36.optixDeviceContextSetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextSetCacheLocation_36(OptixDeviceContext context, const char *location)
{
    TRACE("(%p, %s)\n", context, location);

    if (!location) return OPTIX_ERROR_DISK_CACHE_INVALID_PATH;

    WCHAR location_wide[MAX_PATH];

    MultiByteToWideChar(CP_ACP, 0, location, -1, location_wide, ARRAY_SIZE(location_wide));

    char *unix_location = wine_get_unix_file_name(location_wide);

    OptixResult result = optixFunctionTable_36.optixDeviceContextSetCacheLocation(context, unix_location);

    HeapFree(GetProcessHeap(), 0, unix_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextSetCacheDatabaseSizes_36(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark)
{
    TRACE("(%p, %zu, %zu)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_36.optixDeviceContextSetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixDeviceContextGetCacheEnabled_36(OptixDeviceContext context, int *enabled)
{
    TRACE("(%p, %p)\n", context, enabled);
    return optixFunctionTable_36.optixDeviceContextGetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextGetCacheLocation_36(OptixDeviceContext context, char *location, size_t locationSize)
{
    TRACE("(%p, %p, %zu)\n", context, location, locationSize);

    OptixResult result = optixFunctionTable_36.optixDeviceContextGetCacheLocation(context, location, locationSize);

    if (result != OPTIX_SUCCESS) return result;

    WCHAR *dos_location = wine_get_dos_file_name(location);

    if (!WideCharToMultiByte(CP_ACP, 0, dos_location, -1, location, locationSize, NULL, NULL)) result = OPTIX_ERROR_INVALID_VALUE;

    HeapFree(GetProcessHeap(), 0, dos_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextGetCacheDatabaseSizes_36(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark)
{
    TRACE("(%p, %p, %p)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_36.optixDeviceContextGetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixModuleCreateFromPTX_36(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *PTX, size_t PTXsize, char *logString, size_t *logStringSize, OptixModule *module)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
    return optixFunctionTable_36.optixModuleCreateFromPTX(context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
}

static OptixResult __cdecl optixModuleDestroy_36(OptixModule module)
{
    TRACE("(%p)\n", module);
    return optixFunctionTable_36.optixModuleDestroy(module);
}

static OptixResult __cdecl optixBuiltinISModuleGet_36(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule)
{
    TRACE("(%p, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
    return optixFunctionTable_36.optixBuiltinISModuleGet(context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
}

static OptixResult __cdecl optixProgramGroupCreate_36(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups)
{
    TRACE("(%p, %p, %u, %p, %p, %p, %p)\n", context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
    return optixFunctionTable_36.optixProgramGroupCreate(context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
}

static OptixResult __cdecl optixProgramGroupDestroy_36(OptixProgramGroup programGroup)
{
    TRACE("(%p)\n", programGroup);
    return optixFunctionTable_36.optixProgramGroupDestroy(programGroup);
}

static OptixResult __cdecl optixProgramGroupGetStackSize_36(OptixProgramGroup programGroup, void *stackSizes)
{
    TRACE("(%p, %p)\n", programGroup, stackSizes);
    return optixFunctionTable_36.optixProgramGroupGetStackSize(programGroup, stackSizes);
}

static OptixResult __cdecl optixPipelineCreate_36(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %p, %p)\n", context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
    return optixFunctionTable_36.optixPipelineCreate(context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
}

static OptixResult __cdecl optixPipelineDestroy_36(OptixPipeline pipeline)
{
    TRACE("(%p)\n", pipeline);
    return optixFunctionTable_36.optixPipelineDestroy(pipeline);
}

static OptixResult __cdecl optixPipelineSetStackSize_36(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth)
{
    TRACE("(%p, %u, %u, %u, %u)\n", pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
    return optixFunctionTable_36.optixPipelineSetStackSize(pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
}

static OptixResult __cdecl optixAccelComputeMemoryUsage_36(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes)
{
    TRACE("(%p, %p, %p, %u, %p)\n", context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
    return optixFunctionTable_36.optixAccelComputeMemoryUsage(context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
}

static OptixResult __cdecl optixAccelBuild_36(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %zu, %p, %zu, %p, %p, %u)\n", context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
    return optixFunctionTable_36.optixAccelBuild(context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
}

static OptixResult __cdecl optixAccelGetRelocationInfo_36(OptixDeviceContext context, OptixTraversableHandle handle, void *info)
{
    TRACE("(%p, %llu, %p)\n", context, handle, info);
    return optixFunctionTable_36.optixAccelGetRelocationInfo(context, handle, info);
}

static OptixResult __cdecl optixAccelCheckRelocationCompatibility_36(OptixDeviceContext context, const void *info, int *compatible)
{
    TRACE("(%p, %p, %p)\n", context, info, compatible);
    return optixFunctionTable_36.optixAccelCheckRelocationCompatibility(context, info, compatible);
}

static OptixResult __cdecl optixAccelRelocate_36(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr instanceTraversableHandles, size_t numInstanceTraversableHandles, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %zu, %p)\n", context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
    return optixFunctionTable_36.optixAccelRelocate(context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
}

static OptixResult __cdecl optixAccelCompact_36(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle)
{
    TRACE("(%p, %p, %llu, %p, %zu, %p)\n", context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
    return optixFunctionTable_36.optixAccelCompact(context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
}

static OptixResult __cdecl optixConvertPointerToTraversableHandle_36(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle)
{
    TRACE("(%p, %p, %d, %p)\n", onDevice, pointer, traversableType, traversableHandle);
    return optixFunctionTable_36.optixConvertPointerToTraversableHandle(onDevice, pointer, traversableType, traversableHandle);
}

static OptixResult __cdecl optixSbtRecordPackHeader_36(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer)
{
    TRACE("(%p, %p)\n", programGroup, sbtRecordHeaderHostPointer);
    return optixFunctionTable_36.optixSbtRecordPackHeader(programGroup, sbtRecordHeaderHostPointer);
}

static OptixResult __cdecl optixLaunch_36(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth)
{
    TRACE("(%p, %p, %p, %zu, %p, %u, %u, %u)\n", pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
    return optixFunctionTable_36.optixLaunch(pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
}

static OptixResult __cdecl optixDenoiserCreate_36(OptixDeviceContext context, const void *options, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %p, %p)\n", context, options, returnHandle);
    return optixFunctionTable_36.optixDenoiserCreate(context, options, returnHandle);
}

static OptixResult __cdecl optixDenoiserDestroy_36(OptixDenoiser handle)
{
    TRACE("(%p)\n", handle);
    return optixFunctionTable_36.optixDenoiserDestroy(handle);
}

static OptixResult __cdecl optixDenoiserComputeMemoryResources_36(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes)
{
    TRACE("(%p, %u, %u, %p)\n", handle, maximumInputWidth, maximumInputHeight, returnSizes);
    return optixFunctionTable_36.optixDenoiserComputeMemoryResources(handle, maximumInputWidth, maximumInputHeight, returnSizes);
}

static OptixResult __cdecl optixDenoiserSetup_36(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %u, %u, %p, %zu, %p, %zu)\n", denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
    return optixFunctionTable_36.optixDenoiserSetup(denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserInvoke_36(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, const void *outputLayer, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %u, %u, %u, %p, %p, %zu)\n", denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, layers, numLayers, inputOffsetX, inputOffsetY, outputLayer, scratch, scratchSizeInBytes);
    return optixFunctionTable_36.optixDenoiserInvoke(denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, layers, numLayers, inputOffsetX, inputOffsetY, outputLayer, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserSetModel_36(OptixDenoiser handle, int kind, void *data, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", handle, kind, data, sizeInBytes);
    return optixFunctionTable_36.optixDenoiserSetModel(handle, kind, data, sizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeIntensity_36(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
    return optixFunctionTable_36.optixDenoiserComputeIntensity(handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
}

OptixResult __cdecl optixQueryFunctionTable_36(
        unsigned int numOptions,
        int *optionKeys,
        const void **optionValues,
        void *functionTable,
        size_t sizeOfTable)
{
    if (sizeOfTable != sizeof(OptixFunctionTable_36)) return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;

    OptixResult result = poptixQueryFunctionTable(36, numOptions, optionKeys, optionValues, &optixFunctionTable_36, sizeOfTable);

    if (result != OPTIX_SUCCESS) return result;

    OptixFunctionTable_36 *table = functionTable;

#define ASSIGN_FUNCPTR(f) *(void**)(&table->f) = (void*)&f ## _36

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

#undef ASSIGN_FUNCPTR

    return OPTIX_SUCCESS;
}
