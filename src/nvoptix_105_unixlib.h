#pragma once

#define WINE_UNIX_LIB

#include "winternl.h"
#include "winbase.h"
#include "ntstatus.h"
#include "wine/unixlib.h"

#include "nvoptix_105.h"

NTSTATUS wine_optixGetErrorName_105(void *args);
NTSTATUS wine_optixGetErrorString_105(void *args);
NTSTATUS wine_optixDeviceContextCreate_105(void *args);
NTSTATUS wine_optixDeviceContextDestroy_105(void *args);
NTSTATUS wine_optixDeviceContextGetProperty_105(void *args);
NTSTATUS wine_optixDeviceContextSetLogCallback_105(void *args);
NTSTATUS wine_optixDeviceContextSetCacheEnabled_105(void *args);
NTSTATUS wine_optixDeviceContextSetCacheLocation_105(void *args);
NTSTATUS wine_optixDeviceContextSetCacheDatabaseSizes_105(void *args);
NTSTATUS wine_optixDeviceContextGetCacheEnabled_105(void *args);
NTSTATUS wine_optixDeviceContextGetCacheLocation_105(void *args);
NTSTATUS wine_optixDeviceContextGetCacheDatabaseSizes_105(void *args);
NTSTATUS wine_optixModuleCreate_105(void *args);
NTSTATUS wine_optixModuleCreateWithTasks_105(void *args);
NTSTATUS wine_optixModuleGetCompilationState_105(void *args);
NTSTATUS wine_optixModuleDestroy_105(void *args);
NTSTATUS wine_optixBuiltinISModuleGet_105(void *args);
NTSTATUS wine_optixTaskExecute_105(void *args);
NTSTATUS wine_optixProgramGroupCreate_105(void *args);
NTSTATUS wine_optixProgramGroupDestroy_105(void *args);
NTSTATUS wine_optixProgramGroupGetStackSize_105(void *args);
NTSTATUS wine_optixPipelineCreate_105(void *args);
NTSTATUS wine_optixPipelineDestroy_105(void *args);
NTSTATUS wine_optixPipelineSetStackSize_105(void *args);
NTSTATUS wine_optixAccelComputeMemoryUsage_105(void *args);
NTSTATUS wine_optixAccelBuild_105(void *args);
NTSTATUS wine_optixAccelGetRelocationInfo_105(void *args);
NTSTATUS wine_optixCheckRelocationCompatibility_105(void *args);
NTSTATUS wine_optixAccelRelocate_105(void *args);
NTSTATUS wine_optixAccelCompact_105(void *args);
NTSTATUS wine_optixAccelEmitProperty_105(void *args);
NTSTATUS wine_optixConvertPointerToTraversableHandle_105(void *args);
NTSTATUS wine_optixOpacityMicromapArrayComputeMemoryUsage_105(void *args);
NTSTATUS wine_optixOpacityMicromapArrayBuild_105(void *args);
NTSTATUS wine_optixOpacityMicromapArrayGetRelocationInfo_105(void *args);
NTSTATUS wine_optixOpacityMicromapArrayRelocate_105(void *args);
NTSTATUS wine_optixDisplacementMicromapArrayComputeMemoryUsage_105(void *args);
NTSTATUS wine_optixDisplacementMicromapArrayBuild_105(void *args);
NTSTATUS wine_optixClusterAccelComputeMemoryUsage_105(void *args);
NTSTATUS wine_optixClusterAccelBuild_105(void *args);
NTSTATUS wine_optixSbtRecordPackHeader_105(void *args);
NTSTATUS wine_optixLaunch_105(void *args);
NTSTATUS wine_optixCoopVecMatrixConvert_105(void *args);
NTSTATUS wine_optixCoopVecMatrixComputeSize_105(void *args);
NTSTATUS wine_optixDenoiserCreate_105(void *args);
NTSTATUS wine_optixDenoiserDestroy_105(void *args);
NTSTATUS wine_optixDenoiserComputeMemoryResources_105(void *args);
NTSTATUS wine_optixDenoiserSetup_105(void *args);
NTSTATUS wine_optixDenoiserInvoke_105(void *args);
NTSTATUS wine_optixDenoiserComputeIntensity_105(void *args);
NTSTATUS wine_optixDenoiserComputeAverageColor_105(void *args);
NTSTATUS wine_optixDenoiserCreateWithUserModel_105(void *args);
