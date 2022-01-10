#include "config.h"
#include "wine/port.h"

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix_types.h"

static void *libnvoptix_handle = NULL;
static OptixResult (*poptixQueryFunctionTable)(int abiId, unsigned int numOptions, void *optionKeys, const void **optionValues, void *functionTable, size_t sizeOfTable) = NULL;

#define OPTIX_MAX_ABI_VERSION 55

// TODO: repeat for each ABI version, using OptixFunctionTable from appropriate SDK

#include "nvoptix_55.h"

static OptixFunctionTable_55 optixFunctionTable_55;

// TODO: duplicate functions below for each ABI version, calling into optixFunctionTable_X instead

static const char *__cdecl optixGetErrorName_55(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_55.optixGetErrorName(result);
}

static const char *__cdecl optixGetErrorString_55(OptixResult result)
{
    TRACE("(%d)\n", result);
    return optixFunctionTable_55.optixGetErrorString(result);
}

static OptixResult __cdecl optixDeviceContextCreate_55(CUcontext fromContext, const OptixDeviceContextOptions_55 *options, OptixDeviceContext *context)
{
    TRACE("(%p, %p, %p)\n", fromContext, options, context);

    OptixDeviceContextOptions_55 opts = *options;

    if (opts.logCallbackFunction || opts.logCallbackLevel)
    {
        FIXME("log callback not supported\n");
        opts.logCallbackFunction = NULL;
        opts.logCallbackLevel = 0;
    }

    return optixFunctionTable_55.optixDeviceContextCreate(fromContext, &opts, context);
}

static OptixResult __cdecl optixDeviceContextDestroy_55(OptixDeviceContext context)
{
    TRACE("(%p)\n", context);
    return optixFunctionTable_55.optixDeviceContextDestroy(context);
}

static OptixResult __cdecl optixDeviceContextGetProperty_55(OptixDeviceContext context, int property, void *value, size_t sizeInBytes)
{
    TRACE("(%p, %d, %p, %zu)\n", context, property, value, sizeInBytes);
    return optixFunctionTable_55.optixDeviceContextGetProperty(context, property, value, sizeInBytes);
}

static OptixResult __cdecl optixDeviceContextSetLogCallback_55(OptixDeviceContext context, OptixLogCallback_55 callbackFunction, void *callbackData, unsigned int callbackLevel)
{
    FIXME("(%p, %p, %p, %u): stub\n", context, callbackFunction, callbackData, callbackLevel);
    return OPTIX_SUCCESS;
}

static OptixResult __cdecl optixDeviceContextSetCacheEnabled_55(OptixDeviceContext context, int enabled)
{
    TRACE("(%p, %d)\n", context, enabled);
    return optixFunctionTable_55.optixDeviceContextSetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextSetCacheLocation_55(OptixDeviceContext context, const char *location)
{
    TRACE("(%p, %s)\n", context, location);

    if (!location) return OPTIX_ERROR_DISK_CACHE_INVALID_PATH;

    WCHAR location_wide[MAX_PATH];

    MultiByteToWideChar(CP_ACP, 0, location, -1, location_wide, ARRAY_SIZE(location_wide));

    char *unix_location = wine_get_unix_file_name(location_wide);

    OptixResult result = optixFunctionTable_55.optixDeviceContextSetCacheLocation(context, unix_location);

    HeapFree(GetProcessHeap(), 0, unix_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextSetCacheDatabaseSizes_55(OptixDeviceContext context, size_t lowWaterMark, size_t highWaterMark)
{
    TRACE("(%p, %zu, %zu)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_55.optixDeviceContextSetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixDeviceContextGetCacheEnabled_55(OptixDeviceContext context, int *enabled)
{
    TRACE("(%p, %p)\n", context, enabled);
    return optixFunctionTable_55.optixDeviceContextGetCacheEnabled(context, enabled);
}

static OptixResult __cdecl optixDeviceContextGetCacheLocation_55(OptixDeviceContext context, char *location, size_t locationSize)
{
    TRACE("(%p, %p, %zu)\n", context, location, locationSize);

    OptixResult result = optixFunctionTable_55.optixDeviceContextGetCacheLocation(context, location, locationSize);

    if (result != OPTIX_SUCCESS) return result;

    WCHAR *dos_location = wine_get_dos_file_name(location);

    if (!WideCharToMultiByte(CP_ACP, 0, dos_location, -1, location, locationSize, NULL, NULL)) result = OPTIX_ERROR_INVALID_VALUE;

    HeapFree(GetProcessHeap(), 0, dos_location);

    return result;
}

static OptixResult __cdecl optixDeviceContextGetCacheDatabaseSizes_55(OptixDeviceContext context, size_t *lowWaterMark, size_t *highWaterMark)
{
    TRACE("(%p, %p, %p)\n", context, lowWaterMark, highWaterMark);
    return optixFunctionTable_55.optixDeviceContextGetCacheDatabaseSizes(context, lowWaterMark, highWaterMark);
}

static OptixResult __cdecl optixModuleCreateFromPTX_55(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *PTX, size_t PTXsize, char *logString, size_t *logStringSize, OptixModule *module)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
    return optixFunctionTable_55.optixModuleCreateFromPTX(context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module);
}

static OptixResult __cdecl optixModuleCreateFromPTXWithTasks_55(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const char *PTX, size_t PTXsize, char *logString, size_t *logStringSize, OptixModule *module, OptixTask *firstTask)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module, firstTask);
    return optixFunctionTable_55.optixModuleCreateFromPTXWithTasks(context, moduleCompileOptions, pipelineCompileOptions, PTX, PTXsize, logString, logStringSize, module, firstTask);
}

static OptixResult __cdecl optixModuleGetCompilationState_55(OptixModule module, int *state)
{
    TRACE("(%p, %p)\n", module, state);
    return optixFunctionTable_55.optixModuleGetCompilationState(module, state);
}

static OptixResult __cdecl optixModuleDestroy_55(OptixModule module)
{
    TRACE("(%p)\n", module);
    return optixFunctionTable_55.optixModuleDestroy(module);
}

static OptixResult __cdecl optixBuiltinISModuleGet_55(OptixDeviceContext context, const void *moduleCompileOptions, const void *pipelineCompileOptions, const void *builtinISOptions, OptixModule *builtinModule)
{
    TRACE("(%p, %p, %p, %p, %p)\n", context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
    return optixFunctionTable_55.optixBuiltinISModuleGet(context, moduleCompileOptions, pipelineCompileOptions, builtinISOptions, builtinModule);
}

static OptixResult __cdecl optixTaskExecute_55(OptixTask task, OptixTask *additionalTasks, unsigned int maxNumAdditionalTasks, unsigned int *numAdditionalTasksCreated)
{
    TRACE("(%p, %p, %u, %p)\n", task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated);
    return optixFunctionTable_55.optixTaskExecute(task, additionalTasks, maxNumAdditionalTasks, numAdditionalTasksCreated);
}

static OptixResult __cdecl optixProgramGroupCreate_55(OptixDeviceContext context, const void *programDescriptions, unsigned int numProgramGroups, const void *options, char *logString, size_t *logStringSize, OptixProgramGroup *programGroups)
{
    TRACE("(%p, %p, %u, %p, %p, %p, %p)\n", context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
    return optixFunctionTable_55.optixProgramGroupCreate(context, programDescriptions, numProgramGroups, options, logString, logStringSize, programGroups);
}

static OptixResult __cdecl optixProgramGroupDestroy_55(OptixProgramGroup programGroup)
{
    TRACE("(%p)\n", programGroup);
    return optixFunctionTable_55.optixProgramGroupDestroy(programGroup);
}

static OptixResult __cdecl optixProgramGroupGetStackSize_55(OptixProgramGroup programGroup, void *stackSizes)
{
    TRACE("(%p, %p)\n", programGroup, stackSizes);
    return optixFunctionTable_55.optixProgramGroupGetStackSize(programGroup, stackSizes);
}

static OptixResult __cdecl optixPipelineCreate_55(OptixDeviceContext context, const void *pipelineCompileOptions, const void *pipelineLinkOptions, const OptixProgramGroup *programGroups, unsigned int numProgramGroups, char *logString, size_t *logStringSize, OptixPipeline *pipeline)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %p, %p)\n", context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
    return optixFunctionTable_55.optixPipelineCreate(context, pipelineCompileOptions, pipelineLinkOptions, programGroups, numProgramGroups, logString, logStringSize, pipeline);
}

static OptixResult __cdecl optixPipelineDestroy_55(OptixPipeline pipeline)
{
    TRACE("(%p)\n", pipeline);
    return optixFunctionTable_55.optixPipelineDestroy(pipeline);
}

static OptixResult __cdecl optixPipelineSetStackSize_55(OptixPipeline pipeline, unsigned int directCallableStackSizeFromTraversal, unsigned int directCallableStackSizeFromState, unsigned int continuationStackSize, unsigned int maxTraversableGraphDepth)
{
    TRACE("(%p, %u, %u, %u, %u)\n", pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
    return optixFunctionTable_55.optixPipelineSetStackSize(pipeline, directCallableStackSizeFromTraversal, directCallableStackSizeFromState, continuationStackSize, maxTraversableGraphDepth);
}

static OptixResult __cdecl optixAccelComputeMemoryUsage_55(OptixDeviceContext context, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, void *bufferSizes)
{
    TRACE("(%p, %p, %p, %u, %p)\n", context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
    return optixFunctionTable_55.optixAccelComputeMemoryUsage(context, accelOptions, buildInputs, numBuildInputs, bufferSizes);
}

static OptixResult __cdecl optixAccelBuild_55(OptixDeviceContext context, CUstream stream, const void *accelOptions, const void *buildInputs, unsigned int numBuildInputs, CUdeviceptr tempBuffer, size_t tempBufferSizeInBytes, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle, const void *emittedProperties, unsigned int numEmittedProperties)
{
    TRACE("(%p, %p, %p, %p, %u, %p, %zu, %p, %zu, %p, %p, %u)\n", context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
    return optixFunctionTable_55.optixAccelBuild(context, stream, accelOptions, buildInputs, numBuildInputs, tempBuffer, tempBufferSizeInBytes, outputBuffer, outputBufferSizeInBytes, outputHandle, emittedProperties, numEmittedProperties);
}

static OptixResult __cdecl optixAccelGetRelocationInfo_55(OptixDeviceContext context, OptixTraversableHandle handle, void *info)
{
    TRACE("(%p, %llu, %p)\n", context, handle, info);
    return optixFunctionTable_55.optixAccelGetRelocationInfo(context, handle, info);
}

static OptixResult __cdecl optixAccelCheckRelocationCompatibility_55(OptixDeviceContext context, const void *info, int *compatible)
{
    TRACE("(%p, %p, %p)\n", context, info, compatible);
    return optixFunctionTable_55.optixAccelCheckRelocationCompatibility(context, info, compatible);
}

static OptixResult __cdecl optixAccelRelocate_55(OptixDeviceContext context, CUstream stream, const void *info, CUdeviceptr instanceTraversableHandles, size_t numInstanceTraversableHandles, CUdeviceptr targetAccel, size_t targetAccelSizeInBytes, OptixTraversableHandle *targetHandle)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %zu, %p)\n", context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
    return optixFunctionTable_55.optixAccelRelocate(context, stream, info, instanceTraversableHandles, numInstanceTraversableHandles, targetAccel, targetAccelSizeInBytes, targetHandle);
}

static OptixResult __cdecl optixAccelCompact_55(OptixDeviceContext context, CUstream stream, OptixTraversableHandle inputHandle, CUdeviceptr outputBuffer, size_t outputBufferSizeInBytes, OptixTraversableHandle *outputHandle)
{
    TRACE("(%p, %p, %llu, %p, %zu, %p)\n", context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
    return optixFunctionTable_55.optixAccelCompact(context, stream, inputHandle, outputBuffer, outputBufferSizeInBytes, outputHandle);
}

static OptixResult __cdecl optixConvertPointerToTraversableHandle_55(OptixDeviceContext onDevice, CUdeviceptr pointer, int traversableType, OptixTraversableHandle *traversableHandle)
{
    TRACE("(%p, %p, %d, %p)\n", onDevice, pointer, traversableType, traversableHandle);
    return optixFunctionTable_55.optixConvertPointerToTraversableHandle(onDevice, pointer, traversableType, traversableHandle);
}

static void __cdecl reserved1_55(void)
{
    TRACE("()\n");
    optixFunctionTable_55.reserved1();
}

static void __cdecl reserved2_55(void)
{
    TRACE("()\n");
    optixFunctionTable_55.reserved2();
}

static OptixResult __cdecl optixSbtRecordPackHeader_55(OptixProgramGroup programGroup, void *sbtRecordHeaderHostPointer)
{
    TRACE("(%p, %p)\n", programGroup, sbtRecordHeaderHostPointer);
    return optixFunctionTable_55.optixSbtRecordPackHeader(programGroup, sbtRecordHeaderHostPointer);
}

static OptixResult __cdecl optixLaunch_55(OptixPipeline pipeline, CUstream stream, CUdeviceptr pipelineParams, size_t pipelineParamsSize, const void *sbt, unsigned int width, unsigned int height, unsigned int depth)
{
    TRACE("(%p, %p, %p, %zu, %p, %u, %u, %u)\n", pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
    return optixFunctionTable_55.optixLaunch(pipeline, stream, pipelineParams, pipelineParamsSize, sbt, width, height, depth);
}

static OptixResult __cdecl optixDenoiserCreate_55(OptixDeviceContext context, int modelKind, const void *options, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %d, %p, %p)\n", context, modelKind, options, returnHandle);
    return optixFunctionTable_55.optixDenoiserCreate(context, modelKind, options, returnHandle);
}

static OptixResult __cdecl optixDenoiserDestroy_55(OptixDenoiser handle)
{
    TRACE("(%p)\n", handle);
    return optixFunctionTable_55.optixDenoiserDestroy(handle);
}

static OptixResult __cdecl optixDenoiserComputeMemoryResources_55(const OptixDenoiser handle, unsigned int maximumInputWidth, unsigned int maximumInputHeight, void *returnSizes)
{
    TRACE("(%p, %u, %u, %p)\n", handle, maximumInputWidth, maximumInputHeight, returnSizes);
    return optixFunctionTable_55.optixDenoiserComputeMemoryResources(handle, maximumInputWidth, maximumInputHeight, returnSizes);
}

static OptixResult __cdecl optixDenoiserSetup_55(OptixDenoiser denoiser, CUstream stream, unsigned int inputWidth, unsigned int inputHeight, CUdeviceptr state, size_t stateSizeInBytes, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %u, %u, %p, %zu, %p, %zu)\n", denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
    return optixFunctionTable_55.optixDenoiserSetup(denoiser, stream, inputWidth, inputHeight, state, stateSizeInBytes, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserInvoke_55(OptixDenoiser denoiser, CUstream stream, const void *params, CUdeviceptr denoiserState, size_t denoiserStateSizeInBytes, const void *guideLayer, const void *layers, unsigned int numLayers, unsigned int inputOffsetX, unsigned int inputOffsetY, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %zu, %p, %p, %u, %u, %u, %p, %zu)\n", denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes);
    return optixFunctionTable_55.optixDenoiserInvoke(denoiser, stream, params, denoiserState, denoiserStateSizeInBytes, guideLayer, layers, numLayers, inputOffsetX, inputOffsetY, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeIntensity_55(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputIntensity, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
    return optixFunctionTable_55.optixDenoiserComputeIntensity(handle, stream, inputImage, outputIntensity, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserComputeAverageColor_55(OptixDenoiser handle, CUstream stream, const void *inputImage, CUdeviceptr outputAverageColor, CUdeviceptr scratch, size_t scratchSizeInBytes)
{
    TRACE("(%p, %p, %p, %p, %p, %zu)\n", handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
    return optixFunctionTable_55.optixDenoiserComputeAverageColor(handle, stream, inputImage, outputAverageColor, scratch, scratchSizeInBytes);
}

static OptixResult __cdecl optixDenoiserCreateWithUserModel_55(OptixDeviceContext context, const void *data, size_t dataSizeInBytes, OptixDenoiser *returnHandle)
{
    TRACE("(%p, %p, %zu, %p)\n", context, data, dataSizeInBytes, returnHandle);
    return optixFunctionTable_55.optixDenoiserCreateWithUserModel(context, data, dataSizeInBytes, returnHandle);
}

// TODO: duplicate for each ABI version

static OptixResult __cdecl optixQueryFunctionTable_55(
    unsigned int numOptions,
    int* optionKeys,
    const void** optionValues,
    void* functionTable,
    size_t sizeOfTable)
{
    if (sizeOfTable != sizeof(OptixFunctionTable_55)) return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;

    OptixResult result = poptixQueryFunctionTable(55, numOptions, optionKeys, optionValues, &optixFunctionTable_55, sizeOfTable);

    if (result != OPTIX_SUCCESS) return result;

    OptixFunctionTable_55 *table = functionTable;

    #define ASSIGN_FUNCPTR(f) *(void**)(&table->f) = (void*)&f ## _55

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
    ASSIGN_FUNCPTR(optixModuleCreateFromPTXWithTasks);
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
    ASSIGN_FUNCPTR(optixAccelCheckRelocationCompatibility);
    ASSIGN_FUNCPTR(optixAccelRelocate);
    ASSIGN_FUNCPTR(optixAccelCompact);
    ASSIGN_FUNCPTR(optixConvertPointerToTraversableHandle);
    ASSIGN_FUNCPTR(reserved1);
    ASSIGN_FUNCPTR(reserved2);
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

OptixResult __cdecl optixQueryFunctionTable(
    int abiId,
    unsigned int numOptions,
    int* optionKeys,
    const void** optionValues,
    void* functionTable,
    size_t sizeOfTable)
{
    TRACE("(%d, %u, %p, %p, %p, %zu)\n", abiId, numOptions, optionKeys, optionValues, functionTable, sizeOfTable);

    if (numOptions) FIXME("unexpected numOptions = %u\n", numOptions);

    if (optionKeys) FIXME("unexpected optionKeys = %p\n", optionKeys);

    if (optionValues) FIXME("unexpected optionValues = %p\n", optionValues);

    if (!libnvoptix_handle)
    {
        return OPTIX_ERROR_LIBRARY_NOT_FOUND;
    }
    else if (!poptixQueryFunctionTable)
    {
        return OPTIX_ERROR_ENTRY_SYMBOL_NOT_FOUND;
    }
    else if (abiId > OPTIX_MAX_ABI_VERSION)
    {
        ERR("abiId = %d > %d not supported\n", abiId, OPTIX_MAX_ABI_VERSION);
        return OPTIX_ERROR_UNSUPPORTED_ABI_VERSION;
    }
    else if (sizeOfTable > sizeof(OptixFunctionTable_55))
    {
        ERR("sizeOfTable = %zu > %zu not supported\n", sizeOfTable, sizeof(OptixFunctionTable_55));
        return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;
    }

    switch (abiId)
    {
        // TODO: add other ABI versions here
        case 55:
            return optixQueryFunctionTable_55(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        default:
            ERR("abiId = %d not supported\n", abiId);
            return OPTIX_ERROR_UNSUPPORTED_ABI_VERSION;
    }
}

int __cdecl rtGetSymbolTable()
{
    ERR("(): not implemented\n");
    return ~0;
}

static BOOL load_nvoptix(void)
{
    if (!(libnvoptix_handle = dlopen("libnvoptix.so.1", RTLD_NOW)))
    {
        ERR("Wine cannot find the libnvoptix.so library, NVIDIA Optix support disabled.\n");
        return FALSE;
    }

    #define LOAD_FUNCPTR(f) if (!(*(void **)(&p##f) = dlsym(libnvoptix_handle, #f))) { ERR("Can't find symbol %s.\n", #f); goto fail; }

    LOAD_FUNCPTR(optixQueryFunctionTable);

    #undef LOAD_FUNCPTR

    return TRUE;

fail:
    dlclose(libnvoptix_handle);
    return FALSE;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    TRACE("(%p, %u, %p)\n", instance, reason, reserved);

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(instance);
            if (!load_nvoptix()) return FALSE;
            break;
        case DLL_PROCESS_DETACH:
            if (reserved) break;
            if (libnvoptix_handle) dlclose(libnvoptix_handle);
            break;
    }

    return TRUE;
}
