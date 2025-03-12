#pragma once

#include "nvoptix.h"
#include "basetsd.h"

#define CAT2_(a, b) a##b
#define CAT2(a, b) CAT2_(a, b)
#define NVOPTIX_CALL(func, params) WINE_UNIX_CALL(unix_ ## func, params)
#define OPTIX_MAX_ABI_VERSION 105

struct attach_params
{
    UINT64 log_callback;
    OptixResult result;
};

struct optixQueryFunctionTable_params
{
    int abiId;
    unsigned int numOptions;
    int *optionKeys;
    const void **optionValues;
    size_t sizeOfTable;
    OptixResult _result;
};

struct optixGetError_params
{
    OptixResult result;
    const char *_result;
};

enum nvoptix_funcs
{
    unix_attach,
    unix_detach,
    unix_optixQueryFunctionTable,
    unix_optixGetErrorName_105,
    unix_optixGetErrorString_105,
    unix_optixDeviceContextCreate_105,
    unix_optixDeviceContextDestroy_105,
    unix_optixDeviceContextGetProperty_105,
    unix_optixDeviceContextSetLogCallback_105,
    unix_optixDeviceContextSetCacheEnabled_105,
    unix_optixDeviceContextSetCacheLocation_105,
    unix_optixDeviceContextSetCacheDatabaseSizes_105,
    unix_optixDeviceContextGetCacheEnabled_105,
    unix_optixDeviceContextGetCacheLocation_105,
    unix_optixDeviceContextGetCacheDatabaseSizes_105,
    unix_optixModuleCreate_105,
    unix_optixModuleCreateWithTasks_105,
    unix_optixModuleGetCompilationState_105,
    unix_optixModuleDestroy_105,
    unix_optixBuiltinISModuleGet_105,
    unix_optixTaskExecute_105,
    unix_optixProgramGroupCreate_105,
    unix_optixProgramGroupDestroy_105,
    unix_optixProgramGroupGetStackSize_105,
    unix_optixPipelineCreate_105,
    unix_optixPipelineDestroy_105,
    unix_optixPipelineSetStackSize_105,
    unix_optixAccelComputeMemoryUsage_105,
    unix_optixAccelBuild_105,
    unix_optixAccelGetRelocationInfo_105,
    unix_optixCheckRelocationCompatibility_105,
    unix_optixAccelRelocate_105,
    unix_optixAccelCompact_105,
    unix_optixAccelEmitProperty_105,
    unix_optixConvertPointerToTraversableHandle_105,
    unix_optixOpacityMicromapArrayComputeMemoryUsage_105,
    unix_optixOpacityMicromapArrayBuild_105,
    unix_optixOpacityMicromapArrayGetRelocationInfo_105,
    unix_optixOpacityMicromapArrayRelocate_105,
    unix_optixDisplacementMicromapArrayComputeMemoryUsage_105,
    unix_optixDisplacementMicromapArrayBuild_105,
    unix_optixClusterAccelComputeMemoryUsage_105,
    unix_optixClusterAccelBuild_105,
    unix_optixSbtRecordPackHeader_105,
    unix_optixLaunch_105,
    unix_optixCoopVecMatrixConvert_105,
    unix_optixCoopVecMatrixComputeSize_105,
    unix_optixDenoiserCreate_105,
    unix_optixDenoiserDestroy_105,
    unix_optixDenoiserComputeMemoryResources_105,
    unix_optixDenoiserSetup_105,
    unix_optixDenoiserInvoke_105,
    unix_optixDenoiserComputeIntensity_105,
    unix_optixDenoiserComputeAverageColor_105,
    unix_optixDenoiserCreateWithUserModel_105,
};
