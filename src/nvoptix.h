 /*
 * Copyright (c) 2021 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and proprietary
 * rights in and to this software, related documentation and any modifications thereto.
 * Any use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from NVIDIA Corporation is strictly
 * prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
 * AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
 * SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
 * BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
 * INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES
 */

#pragma once

#include <stddef.h>
#include <pthread.h>

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
    OPTIX_ERROR_INVALID_FUNCTION_USE = 7204,
    OPTIX_ERROR_UNSUPPORTED_ABI_VERSION = 7801,
    OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH = 7802,
    OPTIX_ERROR_INVALID_ENTRY_FUNCTION_OPTIONS = 7803,
    OPTIX_ERROR_LIBRARY_NOT_FOUND = 7804,
    OPTIX_ERROR_ENTRY_SYMBOL_NOT_FOUND = 7805,
    OPTIX_ERROR_DEVICE_OUT_OF_MEMORY = 7807,
    OPTIX_ERROR_CUDA_ERROR  = 7900,
    OPTIX_ERROR_INTERNAL_ERROR = 7990,
    OPTIX_ERROR_UNKNOWN = 7999,
} OptixResult;

// opaque pointers, I'm assuming these stay the same no matter the ABI version

typedef struct OptixDeviceContext_t *OptixDeviceContext;
typedef struct OptixModule_t *OptixModule;
typedef struct OptixProgramGroup_t *OptixProgramGroup;
typedef struct OptixPipeline_t *OptixPipeline;
typedef struct OptixDenoiser_t *OptixDenoiser;
typedef struct OptixTask_t *OptixTask;

// one last simple type that's a number but not an enum

typedef unsigned long long OptixTraversableHandle;

// callback signatures

typedef void (*OptixLogCallback)(unsigned int level, const char *tag, const char *message, void *cbdata);

// all known variants of device context options, suffixed with the ABI version they first appear in

typedef struct OptixDeviceContextOptions_22
{
    OptixLogCallback logCallbackFunction;
    void *logCallbackData;
    int logCallbackLevel;
} OptixDeviceContextOptions_22;

typedef struct OptixDeviceContextOptions_41
{
    OptixLogCallback logCallbackFunction;
    void *logCallbackData;
    int logCallbackLevel;
    int validationMode;
} OptixDeviceContextOptions_41;

// declare a variable for our pointer to function from native libnvoptix.so

extern OptixResult (*poptixQueryFunctionTable)(int abiId, unsigned int numOptions, void *optionKeys, const void **optionValues, void *functionTable, size_t sizeOfTable);

// callback support

struct callback_t
{
    OptixLogCallback __attribute((ms_abi)) func;
    void* data;
};

extern pthread_rwlock_t callbacks_lock;
extern struct callback_t *callbacks;
_Bool callbacks_enabled(void);
void *wrap_callback(OptixLogCallback func, void *data);
void log_callback(unsigned int level, const char *tag, const char *message, void *cbdata);
