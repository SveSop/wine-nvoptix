#if 0
#pragma makedep unix
#endif

#define WINE_UNIX_LIB

#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>

#include "nvoptix_private.h"
#include "nvoptix_105_unixlib.h"
#include "nvoptix_callbacks_unixlib.h"

static void *libnvoptix_handle = NULL;

static OptixResult (*poptixQueryFunctionTable)(int abiId, unsigned int numOptions, void *optionKeys, const void **optionValues, void *functionTable, size_t sizeOfTable) = NULL;

static NTSTATUS attach(void* args)
{
    struct attach_params *params = args;
    pthread_rwlockattr_t attr;

    params->result = OPTIX_ERROR_INTERNAL_ERROR;

    if (!(libnvoptix_handle = dlopen("libnvoptix.so.1", RTLD_NOW)))
    {
        params->result = OPTIX_ERROR_LIBRARY_NOT_FOUND;
        return STATUS_SUCCESS;
    }

    #define LOAD_FUNCPTR(f) if (!(*(void **)(&p##f) = dlsym(libnvoptix_handle, #f))) { params->result = OPTIX_ERROR_ENTRY_SYMBOL_NOT_FOUND; goto end; }

    LOAD_FUNCPTR(optixQueryFunctionTable);

    #undef LOAD_FUNCPTR

    if (pthread_rwlockattr_init(&attr))
        goto end;

    if (pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
        goto end;

    if (!pthread_rwlock_init(&callbacks_lock, &attr))
        params->result = OPTIX_SUCCESS;

    pthread_rwlockattr_destroy(&attr);

end:
    if (params->result)
        dlclose(libnvoptix_handle);
    else
        log_callback = params->log_callback;

    return STATUS_SUCCESS;
}

static NTSTATUS detach(void *args)
{
    if (libnvoptix_handle)
        dlclose(libnvoptix_handle);

    free(callbacks);

    pthread_rwlock_destroy(&callbacks_lock);

    return STATUS_SUCCESS;
}

static OptixResult check_abi(int abiId, size_t sizeOfTable, void **functionTable)
{
    switch (abiId)
    {
#define CASE(abi) case abi: \
            if (sizeOfTable == sizeof(OptixFunctionTable_ ## abi)) \
            { \
                *functionTable = &optixFunctionTable_ ## abi; \
                return OPTIX_SUCCESS; \
            } \
            else \
            { \
                return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH; \
            }
        CASE(105);
#undef CASE
        default: return OPTIX_ERROR_UNSUPPORTED_ABI_VERSION;
    }
}

static NTSTATUS wine_optixQueryFunctionTable(void *args)
{
    struct optixQueryFunctionTable_params *params = args;
    void* functionTable;

    if (!libnvoptix_handle)
    {
        params->_result = OPTIX_ERROR_LIBRARY_NOT_FOUND;
        return STATUS_SUCCESS;
    }
    else if (!poptixQueryFunctionTable)
    {
        params->_result = OPTIX_ERROR_ENTRY_SYMBOL_NOT_FOUND;
        return STATUS_SUCCESS;
    }
    else if ((params->_result = check_abi(params->abiId, params->sizeOfTable, &functionTable)))
    {
        return STATUS_SUCCESS;
    }

    params->_result = poptixQueryFunctionTable(
        params->abiId,
        params->numOptions,
        params->optionKeys,
        params->optionValues,
        functionTable,
        params->sizeOfTable);

    return STATUS_SUCCESS;
}

const unixlib_entry_t __wine_unix_call_funcs[] =
{
    attach,
    detach,
    wine_optixQueryFunctionTable,
    wine_optixGetErrorName_105,
    wine_optixGetErrorString_105,
    wine_optixDeviceContextCreate_105,
    wine_optixDeviceContextDestroy_105,
    wine_optixDeviceContextGetProperty_105,
    wine_optixDeviceContextSetLogCallback_105,
    wine_optixDeviceContextSetCacheEnabled_105,
    wine_optixDeviceContextSetCacheLocation_105,
    wine_optixDeviceContextSetCacheDatabaseSizes_105,
    wine_optixDeviceContextGetCacheEnabled_105,
    wine_optixDeviceContextGetCacheLocation_105,
    wine_optixDeviceContextGetCacheDatabaseSizes_105,
    wine_optixModuleCreate_105,
    wine_optixModuleCreateWithTasks_105,
    wine_optixModuleGetCompilationState_105,
    wine_optixModuleDestroy_105,
    wine_optixBuiltinISModuleGet_105,
    wine_optixTaskExecute_105,
    wine_optixProgramGroupCreate_105,
    wine_optixProgramGroupDestroy_105,
    wine_optixProgramGroupGetStackSize_105,
    wine_optixPipelineCreate_105,
    wine_optixPipelineDestroy_105,
    wine_optixPipelineSetStackSize_105,
    wine_optixAccelComputeMemoryUsage_105,
    wine_optixAccelBuild_105,
    wine_optixAccelGetRelocationInfo_105,
    wine_optixCheckRelocationCompatibility_105,
    wine_optixAccelRelocate_105,
    wine_optixAccelCompact_105,
    wine_optixAccelEmitProperty_105,
    wine_optixConvertPointerToTraversableHandle_105,
    wine_optixOpacityMicromapArrayComputeMemoryUsage_105,
    wine_optixOpacityMicromapArrayBuild_105,
    wine_optixOpacityMicromapArrayGetRelocationInfo_105,
    wine_optixOpacityMicromapArrayRelocate_105,
    wine_optixDisplacementMicromapArrayComputeMemoryUsage_105,
    wine_optixDisplacementMicromapArrayBuild_105,
    wine_optixClusterAccelComputeMemoryUsage_105,
    wine_optixClusterAccelBuild_105,
    wine_optixSbtRecordPackHeader_105,
    wine_optixLaunch_105,
    wine_optixCoopVecMatrixConvert_105,
    wine_optixCoopVecMatrixComputeSize_105,
    wine_optixDenoiserCreate_105,
    wine_optixDenoiserDestroy_105,
    wine_optixDenoiserComputeMemoryResources_105,
    wine_optixDenoiserSetup_105,
    wine_optixDenoiserInvoke_105,
    wine_optixDenoiserComputeIntensity_105,
    wine_optixDenoiserComputeAverageColor_105,
    wine_optixDenoiserCreateWithUserModel_105,
};

