/*
 * MIT License
 * Copyright (C) 2023 Sveinar Søpler
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

#include <dlfcn.h>
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix.h"
#include "nvoptix_87.h"

static OptixFunctionTable_87 optixFunctionTable_87;

/* OptiX ABI = 87 / SDK 8.0.0 */

static const char *__cdecl optixGetErrorName_87(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_87.optixGetErrorName(result);
}

static const char *__cdecl optixGetErrorString_87(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_87.optixGetErrorString(result);
}

static OptixResult __cdecl optixDeviceContextCreate_87(CUcontext fromContext, const OptixDeviceContextOptions_87 *options, OptixDeviceContext *context)
{
    TRACE("(%p, %p, %p)\n", fromContext, options, context);

    OptixDeviceContextOptions_87 opts = *options;

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

    return optixFunctionTable_87.optixDeviceContextCreate(fromContext, &opts, context);
}

static OptixResult __cdecl optixDeviceContextDestroy_87(OptixDeviceContext context)
{
    TRACE("(%p)\n", context);
    return optixFunctionTable_87.optixDeviceContextDestroy(context);
}

static OptixResult __cdecl optixDeviceContextGetProperty_87(OptixDeviceContext context, int property, void *value, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", context, property, value, sizeInBytes);
    return optixFunctionTable_87.optixDeviceContextGetProperty(context, property, value, sizeInBytes);
}

static OptixResult __cdecl optixDeviceContextSetLogCallback_87(OptixDeviceContext context, OptixLogCallback callbackFunction, void *callbackData, unsigned int callbackLevel)
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

    return optixFunctionTable_87.optixDeviceContextSetLogCallback(context, callbackFunction, callbackData, callbackLevel);
}

static OptixResult __cdecl optixDeviceContextSetCacheEnabled_87(OptixDeviceContext context, int enabled)
{
    TRACE("(%p, %d)\n", context, enabled);
    return optixFunctionTable_87.optixDeviceContextSetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextSetCacheLocation_87(OptixDeviceContext context, const char *location)
{
    TRACE("(%p, %s)\n", context, location);

    if (!location) return OPTIX_ERROR_DISK_CACHE_INVALID_PATH;

    WCHAR location_wide[MAX_PATH];

    MultiByteToWideChar(CP_ACP, 0, location, -1, location_wide, ARRAY_SIZE(location_wide));

    char *unix_location = wine_get_unix_file_name(location_wide);

    OptixResult result = optixFunctionTable_87.optixDeviceContextSetCacheLocation(context, unix_location);

    HeapFree(GetProcessHeap(), 0, unix_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextSetCacheDatabaseSizes_87(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark)
{
    TRACE("(%p, %zu, %zu)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_87.optixDeviceContextSetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixDeviceContextGetCacheEnabled_87(OptixDeviceContext context, int *enabled)
{
    TRACE("(%p, %p)\n", context, enabled);
    return optixFunctionTable_87.optixDeviceContextGetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextGetCacheLocation_87(OptixDeviceContext context, char *location, size_t locationSize)
{
    TRACE("(%p, %p, %zu)\n", context, location, locationSize);

    OptixResult result = optixFunctionTable_87.optixDeviceContextGetCacheLocation(context, location, locationSize);

    if (result != OPTIX_SUCCESS) return result;

    WCHAR *dos_location = wine_get_dos_file_name(location);

    if (!WideCharToMultiByte(CP_ACP, 0, dos_location, -1, location, locationSize, NULL, NULL)) result = OPTIX_ERROR_INVALID_VALUE;

    HeapFree(GetProcessHeap(), 0, dos_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextGetCacheDatabaseSizes_87(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark)
{
    TRACE("(%p, %p, %p)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_87.optixDeviceContextGetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixModuleCreate_87(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module);
    return optixFunctionTable_87.optixModuleCreate(context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module);
}

static OptixResult __cdecl optixModuleCreateWithTasks_87(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *input, size_t inputSize, char *logString, size_t *logStringSize, OptixModule *module, OptixTask *firstTask)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module, firstTask);
    return optixFunctionTable_87.optixModuleCreateWithTasks(context, moduleCompileOptions, pipelineCompileOptions, input, inputSize, logString, logStringSize, module, firstTask);
}

static OptixResult __cdecl optixModuleGetCompilationState_87(OptixModule module, int *state)
{
    TRACE("(%p, %p)\n", module, state);
    return optixFunctionTable_87.optixModuleGetCompilationState(module, state);
}

static OptixResult __cdecl optixModuleDestroy_87(OptixModule module)
{
    TRACE("(%p)\n", module);
    return optixFunctionTable_87.optixModuleDestroy(module);
}

static OptixResult __cdecl optixBuiltinISModuleGet_87(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule)
{
    TRACE("(%p, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
    return optixFunctionTable_87.optixBuiltinISModuleGet(context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
}

static OptixResult __cdecl optixTaskExecute_87(OptixTask task, OptixTask *additionalTasks, unsigned int maxNumAdditionalTasks, unsigned int *numAdditionalTasksCreated)
{
    TRACE("(%p, %p, %u, %p)\n", task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated);
    return optixFunctionTable_87.optixTaskExecute(task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated);
}

static OptixResult __cdecl optixProgramGroupCreate_87(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups)
{
    TRACE("(%p, %p, %u, %p, %p, %p, %p)\n", context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
    return optixFunctionTable_87.optixProgramGroupCreate(context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
}

static OptixResult __cdecl optixProgramGroupDestroy_87(OptixProgramGroup programGroup)
{
    TRACE("(%p)\n", programGroup);
    return optixFunctionTable_87.optixProgramGroupDestroy(programGroup);
}

static OptixResult __cdecl optixProgramGroupGetStackSize_87(OptixProgramGroup programGroup, void *stackSizes, OptixPipeline pipeline)
{
    TRACE("(%p, %p, %p)\n", programGroup, stackSizes, pipeline);
    return optixFunctionTable_87.optixProgramGroupGetStackSize(programGroup, stackSizes, pipeline);
}

static OptixResult __cdecl optixPipelineCreate_87(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %p, %p)\n", context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
    return optixFunctionTable_87.optixPipelineCreate(context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
}

static OptixResult __cdecl optixPipelineDestroy_87(OptixPipeline pipeline)
{
    TRACE("(%p)\n", pipeline);
    return optixFunctionTable_87.optixPipelineDestroy(pipeline);
}

static OptixResult __cdecl optixPipelineSetStackSize_87(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth)
{
    TRACE("(%p, %u, %u, %u, %u)\n", pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
    return optixFunctionTable_87.optixPipelineSetStackSize(pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
}

static OptixResult __cdecl optixAccelComputeMemoryUsage_87(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes)
{
    TRACE("(%p, %p, %p, %u, %p)\n", context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
    return optixFunctionTable_87.optixAccelComputeMemoryUsage(context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
}

static OptixResult __cdecl optixAccelBuild_87(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %zu, %p, %zu, %p, %p, %u)\n", context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
    return optixFunctionTable_87.optixAccelBuild(context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
}

static OptixResult __cdecl optixAccelGetRelocationInfo_87(OptixDeviceContext context, OptixTraversableHandle handle, void *info)
{
    TRACE("(%p, %llu, %p)\n", context, handle, info);
    return optixFunctionTable_87.optixAccelGetRelocationInfo(context, handle, info);
}

static OptixResult __cdecl optixCheckRelocationCompatibility_87(OptixDeviceContext context, const void *info, int *compatible)
{
    TRACE("(%p, %p, %p)\n", context, info, compatible);
    return optixFunctionTable_87.optixCheckRelocationCompatibility(context, info, compatible);
}

static OptixResult __cdecl optixAccelRelocate_87(OptixDeviceContext context, CUstream stream, const void *info, const void *relocateInputs, size_t numRelocateInputs, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %zu, %p)\n", context, stream, info, relocateInputs, numRelocateInputs, targetAccel, targetAccelSizeInBytes, targetHandle);
    return optixFunctionTable_87.optixAccelRelocate(context, stream, info, relocateInputs, numRelocateInputs, targetAccel, targetAccelSizeInBytes, targetHandle);
}

static OptixResult __cdecl optixAccelCompact_87(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle)
{
    TRACE("(%p, %p, %llu, %p, %zu, %p)\n", context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
    return optixFunctionTable_87.optixAccelCompact(context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
}

static OptixResult __cdecl optixAccelEmitProperty_87(OptixDeviceContext context, CUstream stream, OptixTraversableHandle handle, const void *emittedProperty)
{
    TRACE("(%p, %p, %llu, %p)\n", context, stream, handle, emittedProperty);
    return optixFunctionTable_87.optixAccelEmitProperty(context, stream, handle, emittedProperty);
}

static OptixResult __cdecl optixConvertPointerToTraversableHandle_87(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle)
{
    TRACE("(%p, %p, %d, %p)\n", onDevice, pointer, traversableType, traversableHandle);
    return optixFunctionTable_87.optixConvertPointerToTraversableHandle(onDevice, pointer, traversableType, traversableHandle);
}

static OptixResult __cdecl optixOpacityMicromapArrayComputeMemoryUsage_87(OptixDeviceContext context, const void *buildInput, void *bufferSizes)
{
    TRACE("(%p, %p, %p)\n", context, buildInput, bufferSizes);
    return optixFunctionTable_87.optixOpacityMicromapArrayComputeMemoryUsage(context, buildInput, bufferSizes);
}

static OptixResult __cdecl optixOpacityMicromapArrayBuild_87(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers)
{
    TRACE("(%p, %p, %p, %p)\n", context, stream, buildInput, buffers);
    return optixFunctionTable_87.optixOpacityMicromapArrayBuild(context, stream, buildInput, buffers);
}

static OptixResult __cdecl optixOpacityMicromapArrayGetRelocationInfo_87(OptixDeviceContext context, CUdeviceptr opacityMicromapArray, void *info)
{
    TRACE("(%p, %p, %p)\n", context, opacityMicromapArray, info);
    return optixFunctionTable_87.optixOpacityMicromapArrayGetRelocationInfo(context, opacityMicromapArray, info);
}

static OptixResult __cdecl optixOpacityMicromapArrayRelocate_87(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr targetOpacityMicromapArray, size_t targetOpacityMicromapArraySizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %zu)\n", context, stream, info, targetOpacityMicromapArray, targetOpacityMicromapArraySizeInBytes);
    return optixFunctionTable_87.optixOpacityMicromapArrayRelocate(context, stream, info, targetOpacityMicromapArray, targetOpacityMicromapArraySizeInBytes);
}

static OptixResult __cdecl optixDisplacementMicromapArrayComputeMemoryUsage_87(OptixDeviceContext context, const void *buildInput, void *bufferSizes)
{
    TRACE("(%p, %p, %p)\n", context, buildInput, bufferSizes);
    return optixFunctionTable_87.optixDisplacementMicromapArrayComputeMemoryUsage(context, buildInput, bufferSizes);
}

static OptixResult __cdecl optixDisplacementMicromapArrayBuild_87(OptixDeviceContext context, CUstream stream, const void *buildInput, const void *buffers)
{
    TRACE("(%p, %p, %p, %p)\n", context, stream, buildInput, buffers);
    return optixFunctionTable_87.optixDisplacementMicromapArrayBuild(context, stream, buildInput, buffers);
}

static OptixResult __cdecl optixSbtRecordPackHeader_87(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer)
{
    TRACE("(%p, %p)\n", programGroup, sbtRecordHeaderHostPointer);
    return optixFunctionTable_87.optixSbtRecordPackHeader(programGroup, sbtRecordHeaderHostPointer);
}

static OptixResult __cdecl optixLaunch_87(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth)
{
    TRACE("(%p, %p, %p, %zu, %p, %u, %u, %u)\n", pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
    return optixFunctionTable_87.optixLaunch(pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
}

static OptixResult __cdecl optixDenoiserCreate_87(OptixDeviceContext context, int modelKind, const void *options, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %d, %p, %p)\n", context, modelKind, options, returnHandle);
    return optixFunctionTable_87.optixDenoiserCreate(context, modelKind, options, returnHandle);
}

static OptixResult __cdecl optixDenoiserDestroy_87(OptixDenoiser handle)
{
    TRACE("(%p)\n", handle);
    return optixFunctionTable_87.optixDenoiserDestroy(handle);
}

static OptixResult __cdecl optixDenoiserComputeMemoryResources_87(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes)
{
    TRACE("(%p, %u, %u, %p)\n", handle, maximumInputWidth, maximumInputHeight, returnSizes);
    return optixFunctionTable_87.optixDenoiserComputeMemoryResources(handle, maximumInputWidth, maximumInputHeight, returnSizes);
}

static OptixResult __cdecl optixDenoiserSetup_87(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %u, %u, %p, %zu, %p, %zu)\n", denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
    return optixFunctionTable_87.optixDenoiserSetup(denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserInvoke_87(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *guideLayer, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %u, %u, %u, %p, %zu)\n", denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes);
    return optixFunctionTable_87.optixDenoiserInvoke(denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeIntensity_87(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
    return optixFunctionTable_87.optixDenoiserComputeIntensity(handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeAverageColor_87(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
    return optixFunctionTable_87.optixDenoiserComputeAverageColor(handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserCreateWithUserModel_87(OptixDeviceContext context, const void *data, size_t dataSizeInBytes, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %p, %zu, %p)\n", context, data, dataSizeInBytes, returnHandle);
    return optixFunctionTable_87.optixDenoiserCreateWithUserModel(context, data, dataSizeInBytes, returnHandle);
}

OptixResult __cdecl optixQueryFunctionTable_87(
    unsigned int numOptions,
    int *optionKeys,
    const void **optionValues,
    void *functionTable,
    size_t sizeOfTable)
{
    if (sizeOfTable != sizeof(OptixFunctionTable_87)) return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;

    OptixResult result = poptixQueryFunctionTable(87, numOptions, optionKeys, optionValues, &optixFunctionTable_87, sizeOfTable);

    if (result != OPTIX_SUCCESS) return result;

    OptixFunctionTable_87 *table = functionTable;

    #define ASSIGN_FUNCPTR(f) *(void**)(&table->f) = (void*)&f ## _87

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
    ASSIGN_FUNCPTR(optixModuleCreate);
    ASSIGN_FUNCPTR(optixModuleCreateWithTasks);
    ASSIGN_FUNCPTR(optixModuleGetCompilationState);
    ASSIGN_FUNCPTR(optixModuleDestroy);
    ASSIGN_FUNCPTR(optixBuiltinISModuleGet);
    ASSIGN_FUNCPTR(optixTaskExecute);
    ASSIGN_FUNCPTR(optixProgramGroupCreate);
    ASSIGN_FUNCPTR(optixProgramGroupDestroy);
    ASSIGN_FUNCPTR(optixProgramGroupGetStackSize);
    ASSIGN_FUNCPTR(optixPipelineCreate);
    ASSIGN_FUNCPTR(optixPipelineDestroy);
    ASSIGN_FUNCPTR(optixPipelineSetStackSize);
    ASSIGN_FUNCPTR(optixAccelComputeMemoryUsage);
    ASSIGN_FUNCPTR(optixAccelBuild);
    ASSIGN_FUNCPTR(optixAccelGetRelocationInfo);
    ASSIGN_FUNCPTR(optixCheckRelocationCompatibility);
    ASSIGN_FUNCPTR(optixAccelRelocate);
    ASSIGN_FUNCPTR(optixAccelCompact);
    ASSIGN_FUNCPTR(optixAccelEmitProperty);
    ASSIGN_FUNCPTR(optixConvertPointerToTraversableHandle);
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayRelocate);
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayGetRelocationInfo);
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayBuild);
    ASSIGN_FUNCPTR(optixOpacityMicromapArrayComputeMemoryUsage);
    ASSIGN_FUNCPTR(optixCheckRelocationCompatibility);
    ASSIGN_FUNCPTR(optixDisplacementMicromapArrayComputeMemoryUsage);
    ASSIGN_FUNCPTR(optixDisplacementMicromapArrayBuild);
    ASSIGN_FUNCPTR(optixSbtRecordPackHeader);
    ASSIGN_FUNCPTR(optixLaunch);
    ASSIGN_FUNCPTR(optixDenoiserCreate);
    ASSIGN_FUNCPTR(optixDenoiserDestroy);
    ASSIGN_FUNCPTR(optixDenoiserComputeMemoryResources);
    ASSIGN_FUNCPTR(optixDenoiserSetup);
    ASSIGN_FUNCPTR(optixDenoiserInvoke);
    ASSIGN_FUNCPTR(optixDenoiserComputeIntensity);
    ASSIGN_FUNCPTR(optixDenoiserComputeAverageColor);
    ASSIGN_FUNCPTR(optixDenoiserCreateWithUserModel);

    #undef ASSIGN_FUNCPTR

    return OPTIX_SUCCESS;
}

