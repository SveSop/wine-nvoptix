#pragma once

// opaque pointers, I'm assuming these stay the same no matter the ABI version

typedef void *CUdeviceptr;
typedef struct CUctx_st *CUcontext;
typedef struct CUstream_st *CUstream;

// only the codes that could useful to us

typedef enum OptixResult
{
    OPTIX_SUCCESS = 0,
    OPTIX_ERROR_INVALID_VALUE = 7001,
    OPTIX_ERROR_HOST_OUT_OF_MEMORY = 7002,
    OPTIX_ERROR_DISK_CACHE_INVALID_PATH = 7010,
    OPTIX_ERROR_UNSUPPORTED_ABI_VERSION = 7801,
    OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH = 7802,
    OPTIX_ERROR_INVALID_ENTRY_FUNCTION_OPTIONS = 7803,
    OPTIX_ERROR_LIBRARY_NOT_FOUND = 7804,
    OPTIX_ERROR_ENTRY_SYMBOL_NOT_FOUND = 7805,
} OptixResult;

// opaque pointers, I'm assuming these stay the same no matter the ABI version

typedef struct OptixDeviceContext_t *OptixDeviceContext;
typedef struct OptixModule_t *OptixModule;
typedef struct OptixProgramGroup_t *OptixProgramGroup;
typedef struct OptixPipeline_t *OptixPipeline;
typedef struct OptixDenoiser_t *OptixDenoiser;
typedef struct OptixTask_t *OptixTask;

// one last type that's a number but not an enum

typedef unsigned long long OptixTraversableHandle;
