#if 0
#pragma makedep unix
#endif

#define WINE_UNIX_LIB

#include <dlfcn.h>
#include <stdarg.h>

#include "nvoptix_private.h"
#include "nvoptix_callbacks_unixlib.h"
#include "nvoptix_105_unixlib.h"

OptixFunctionTable_105 optixFunctionTable_105;

/* OptiX ABI = 105 / SDK 9.0.0 */

NTSTATUS wine_optixGetErrorName_105(void *args)
{
    struct optixGetError_params *params = args;
    params->_result = optixFunctionTable_105.optixGetErrorName(params->result);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixGetErrorString_105(void *args)
{
    struct optixGetError_params *params = args;
    params->_result = optixFunctionTable_105.optixGetErrorString(params->result);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextCreate_105(void *args)
{
    struct optixDeviceContextCreate_105_params *params = args;
    OptixDeviceContextOptions_41 opts = *params->options;

    if (opts.logCallbackFunction)
    {
        opts.logCallbackData = wrap_callback(opts.logCallbackFunction, opts.logCallbackData);
        opts.logCallbackFunction = unix_log_callback;
    }

    params->_result = optixFunctionTable_105.optixDeviceContextCreate(params->fromContext, &opts, params->context);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextDestroy_105(void *args)
{
    struct optixDeviceContextDestroy_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextDestroy(params->context);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextGetProperty_105(void *args)
{
    struct optixDeviceContextGetProperty_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextGetProperty(params->context, params->property, params->value, params->sizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextSetLogCallback_105(void *args)
{
    struct optixDeviceContextSetLogCallback_105_params *params = args;

    if (params->callbackFunction)
    {
        params->callbackData = wrap_callback(params->callbackFunction, params->callbackData);
        params->callbackFunction = unix_log_callback;
    }

    params->_result = optixFunctionTable_105.optixDeviceContextSetLogCallback(params->context, params->callbackFunction, params->callbackData, params->callbackLevel);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextSetCacheEnabled_105(void *args)
{
    struct optixDeviceContextSetCacheEnabled_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextSetCacheEnabled(params->context, params->enabled);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextSetCacheLocation_105(void *args)
{
    struct optixDeviceContextSetCacheLocation_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextSetCacheLocation(params->context, params->location);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextSetCacheDatabaseSizes_105(void *args)
{
    struct optixDeviceContextSetCacheDatabaseSizes_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextSetCacheDatabaseSizes(params->context, params->lowWaterMark, params->highWaterMark);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextGetCacheEnabled_105(void *args)
{
    struct optixDeviceContextGetCacheEnabled_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextGetCacheEnabled(params->context, params->enabled);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextGetCacheLocation_105(void *args)
{
    struct optixDeviceContextGetCacheLocation_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextGetCacheLocation(params->context, params->location, params->locationSize);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDeviceContextGetCacheDatabaseSizes_105(void *args)
{
    struct optixDeviceContextGetCacheDatabaseSizes_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDeviceContextGetCacheDatabaseSizes(params->context, params->lowWaterMark, params->highWaterMark);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixModuleCreate_105(void *args)
{
    struct optixModuleCreate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixModuleCreate(params->context, params->moduleCompileOptions, params->pipelineCompileOptions, params->input, params->inputSize, params->logString, params->logStringSize, params->module);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixModuleCreateWithTasks_105(void *args)
{
    struct optixModuleCreateWithTasks_105_params *params = args;
    params->_result = optixFunctionTable_105.optixModuleCreateWithTasks(params->context, params->moduleCompileOptions, params->pipelineCompileOptions, params->input, params->inputSize, params->logString, params->logStringSize, params->module, params->firstTask);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixModuleGetCompilationState_105(void *args)
{
    struct optixModuleGetCompilationState_105_params *params = args;
    params->_result = optixFunctionTable_105.optixModuleGetCompilationState(params->module, params->state);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixModuleDestroy_105(void *args)
{
    struct optixModuleDestroy_105_params *params = args;
    params->_result = optixFunctionTable_105.optixModuleDestroy(params->module);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixBuiltinISModuleGet_105(void *args)
{
    struct optixBuiltinISModuleGet_105_params *params = args;
    params->_result = optixFunctionTable_105.optixBuiltinISModuleGet(params->context, params->moduleCompileOptions, params->pipelineCompileOptions, params->builtinISOptions, params->builtinModule);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixTaskExecute_105(void *args)
{
    struct optixTaskExecute_105_params *params = args;
    params->_result = optixFunctionTable_105.optixTaskExecute(params->task, params->additionalTasks, params->maxNumAdditionalTasks, params->numAdditionalTasksCreated);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixProgramGroupCreate_105(void *args)
{
    struct optixProgramGroupCreate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixProgramGroupCreate(params->context, params->programDescriptions, params->numProgramGroups, params->options, params->logString, params->logStringSize, params->programGroups);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixProgramGroupDestroy_105(void *args)
{
    struct optixProgramGroupDestroy_105_params *params = args;
    params->_result = optixFunctionTable_105.optixProgramGroupDestroy(params->programGroup);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixProgramGroupGetStackSize_105(void *args)
{
    struct optixProgramGroupGetStackSize_105_params *params = args;
    params->_result = optixFunctionTable_105.optixProgramGroupGetStackSize(params->programGroup, params->stackSizes, params->pipeline);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixPipelineCreate_105(void *args)
{
    struct optixPipelineCreate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixPipelineCreate(params->context, params->pipelineCompileOptions, params->pipelineLinkOptions, params->programGroups, params->numProgramGroups, params->logString, params->logStringSize, params->pipeline);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixPipelineDestroy_105(void *args)
{
    struct optixPipelineDestroy_105_params *params = args;
    params->_result = optixFunctionTable_105.optixPipelineDestroy(params->pipeline);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixPipelineSetStackSize_105(void *args)
{
    struct optixPipelineSetStackSize_105_params *params = args;
    params->_result = optixFunctionTable_105.optixPipelineSetStackSize(params->pipeline, params->directCallableStackSizeFromTraversal, params->directCallableStackSizeFromState, params->continuationStackSize, params->maxTraversableGraphDepth);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelComputeMemoryUsage_105(void *args)
{
    struct optixAccelComputeMemoryUsage_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelComputeMemoryUsage(params->context, params->accelOptions, params->buildInputs, params->numBuildInputs, params->bufferSizes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelBuild_105(void *args)
{
    struct optixAccelBuild_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelBuild(params->context, params->stream, params->accelOptions, params->buildInputs, params->numBuildInputs, params->tempBuffer, params->tempBufferSizeInBytes, params->outputBuffer, params->outputBufferSizeInBytes, params->outputHandle, params->emittedProperties, params->numEmittedProperties);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelGetRelocationInfo_105(void *args)
{
    struct optixAccelGetRelocationInfo_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelGetRelocationInfo(params->context, params->handle, params->info);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixCheckRelocationCompatibility_105(void *args)
{
    struct optixCheckRelocationCompatibility_105_params *params = args;
    params->_result = optixFunctionTable_105.optixCheckRelocationCompatibility(params->context, params->info, params->compatible);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelRelocate_105(void *args)
{
    struct optixAccelRelocate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelRelocate(params->context, params->stream, params->info,params-> relocateInputs, params->numRelocateInputs, params->targetAccel, params->targetAccelSizeInBytes, params->targetHandle);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelCompact_105(void *args)
{
    struct optixAccelCompact_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelCompact(params->context, params->stream, params->inputHandle,params-> outputBuffer, params->outputBufferSizeInBytes,params-> outputHandle);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixAccelEmitProperty_105(void *args)
{
    struct optixAccelEmitProperty_105_params *params = args;
    params->_result = optixFunctionTable_105.optixAccelEmitProperty(params->context, params->stream, params->handle,params-> emittedProperty);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixConvertPointerToTraversableHandle_105(void *args)
{
    struct optixConvertPointerToTraversableHandle_105_params *params = args;
    params->_result = optixFunctionTable_105.optixConvertPointerToTraversableHandle(params->onDevice, params->pointer, params->traversableType, params->traversableHandle);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixOpacityMicromapArrayComputeMemoryUsage_105(void *args)
{
    struct optixOpacityMicromapArrayComputeMemoryUsage_105_params *params = args;
    params->_result = optixFunctionTable_105.optixOpacityMicromapArrayComputeMemoryUsage(params->context, params->buildInput, params->bufferSizes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixOpacityMicromapArrayBuild_105(void *args)
{
    struct optixOpacityMicromapArrayBuild_105_params *params = args;
    params->_result = optixFunctionTable_105.optixOpacityMicromapArrayBuild(params->context, params->stream, params->buildInput, params->buffers);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixOpacityMicromapArrayGetRelocationInfo_105(void *args)
{
    struct optixOpacityMicromapArrayGetRelocationInfo_105_params *params = args;
    params->_result = optixFunctionTable_105.optixOpacityMicromapArrayGetRelocationInfo(params->context, params->opacityMicromapArray, params->info);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixOpacityMicromapArrayRelocate_105(void *args)
{
    struct optixOpacityMicromapArrayRelocate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixOpacityMicromapArrayRelocate(params->context,params-> stream, params->info, params->targetOpacityMicromapArray, params->targetOpacityMicromapArraySizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDisplacementMicromapArrayComputeMemoryUsage_105(void *args)
{
    struct optixDisplacementMicromapArrayComputeMemoryUsage_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDisplacementMicromapArrayComputeMemoryUsage(params->context, params->buildInput, params->bufferSizes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDisplacementMicromapArrayBuild_105(void *args)
{
    struct optixDisplacementMicromapArrayBuild_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDisplacementMicromapArrayBuild(params->context, params->stream, params->buildInput, params->buffers);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixClusterAccelComputeMemoryUsage_105(void *args)
{
    struct optixClusterAccelComputeMemoryUsage_105_params *params = args;
    params->_result = optixFunctionTable_105.optixClusterAccelComputeMemoryUsage(params->context, params->buildMode, params->buildInput, params->bufferSizes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixClusterAccelBuild_105(void *args)
{
    struct optixClusterAccelBuild_105_params *params = args;
    params->_result = optixFunctionTable_105.optixClusterAccelBuild(params->context, params->stream, params->buildModeDesc, params->buildInput, params->argsArray, params->argsCount, params->argsStrideInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixSbtRecordPackHeader_105(void *args)
{
    struct optixSbtRecordPackHeader_105_params *params = args;
    params->_result = optixFunctionTable_105.optixSbtRecordPackHeader(params->programGroup, params->sbtRecordHeaderHostPointer);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixLaunch_105(void *args)
{
    struct optixLaunch_105_params *params = args;
    params->_result = optixFunctionTable_105.optixLaunch(params->pipeline, params->stream, params->pipelineParams, params->pipelineParamsSize, params->sbt, params->width, params->height, params->depth);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixCoopVecMatrixConvert_105(void *args)
{
    struct optixCoopVecMatrixConvert_105_params *params = args;
    params->_result = optixFunctionTable_105.optixCoopVecMatrixConvert(params->context, params->stream, params->numNetworks, params->inputNetworkDescription, params->inputNetworks, params->inputNetworkStrideInBytes, params->outputNetworkDescription, params->outputNetworks, params->outputNetworkStrideInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixCoopVecMatrixComputeSize_105(void *args)
{
    struct optixCoopVecMatrixComputeSize_105_params *params = args;
    params->_result = optixFunctionTable_105.optixCoopVecMatrixComputeSize(params->context, params->N, params->K, params->elementType, params->layout, params->rowColumnStrideInBytes, params->sizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserCreate_105(void *args)
{
    struct optixDenoiserCreate_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserCreate(params->context, params->modelKind, params->options, params->returnHandle);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserDestroy_105(void *args)
{
    struct optixDenoiserDestroy_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserDestroy(params->handle);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserComputeMemoryResources_105(void *args)
{
    struct optixDenoiserComputeMemoryResources_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserComputeMemoryResources(params->handle, params->maximumInputWidth, params->maximumInputHeight, params->returnSizes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserSetup_105(void *args)
{
    struct optixDenoiserSetup_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserSetup(params->denoiser, params->stream, params->inputWidth, params->inputHeight, params->state, params->stateSizeInBytes, params->scratch, params->scratchSizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserInvoke_105(void *args)
{
    struct optixDenoiserInvoke_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserInvoke(params->denoiser, params->stream, params->params, params->denoiserState, params->denoiserStateSizeInBytes, params->guideLayer, params->layers, params->numLayers, params->inputOffsetX, params->inputOffsetY, params->scratch, params->scratchSizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserComputeIntensity_105(void *args)
{
    struct optixDenoiserComputeIntensity_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserComputeIntensity(params->handle, params->stream, params->inputImage, params->outputIntensity, params->scratch, params->scratchSizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserComputeAverageColor_105(void *args)
{
    struct optixDenoiserComputeAverageColor_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserComputeAverageColor(params->handle, params->stream, params->inputImage, params->outputAverageColor, params->scratch, params->scratchSizeInBytes);
    return STATUS_SUCCESS;
}

NTSTATUS wine_optixDenoiserCreateWithUserModel_105(void *args)
{
    struct optixDenoiserCreateWithUserModel_105_params *params = args;
    params->_result = optixFunctionTable_105.optixDenoiserCreateWithUserModel(params->context, params->data, params->dataSizeInBytes, params->returnHandle);
    return STATUS_SUCCESS;
}
