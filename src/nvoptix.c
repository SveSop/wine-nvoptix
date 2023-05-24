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
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix.h"
#include "nvoptix_68.h"
#include "nvoptix_60.h"
#include "nvoptix_55.h"
#include "nvoptix_47.h"
#include "nvoptix_41.h"
#include "nvoptix_36.h"
#include "nvoptix_22.h"

static void *libnvoptix_handle = NULL;
OptixResult (*poptixQueryFunctionTable)(int abiId, unsigned int numOptions, void *optionKeys, const void **optionValues, void *functionTable, size_t sizeOfTable) = NULL;

#define OPTIX_MAX_ABI_VERSION 84

OptixResult __cdecl optixQueryFunctionTable(
    int abiId,
    unsigned int numOptions,
    int *optionKeys,
    const void **optionValues,
    void *functionTable,
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
    else if (sizeOfTable > sizeof(OptixFunctionTable_68))
    {
        ERR("sizeOfTable = %zu > %zu not supported\n", sizeOfTable, sizeof(OptixFunctionTable_68));
        return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;
    }

    switch (abiId)
    {
        // TODO: add other ABI versions here
        case 84:
            return optixQueryFunctionTable_84(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 68:
            return optixQueryFunctionTable_68(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 60:
            return optixQueryFunctionTable_60(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 55:
            return optixQueryFunctionTable_55(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 47:
            return optixQueryFunctionTable_47(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 41:
            return optixQueryFunctionTable_41(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 36:
            return optixQueryFunctionTable_36(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
        case 22:
            return optixQueryFunctionTable_22(numOptions, optionKeys, optionValues, functionTable, sizeOfTable);
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

    if (callbacks_enabled())
    {
        pthread_rwlockattr_t attr;

        if (pthread_rwlockattr_init(&attr))
        {
            ERR("Failed to initialize rwlockattr.\n");
            goto fail;
        }

        if (pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
        {
            ERR("Failed to configure rwlockattr.\n");
            goto fail;
        }

        if (pthread_rwlock_init(&callbacks_lock, &attr))
        {
            ERR("Failed to initialize rwlock.\n");

            if (pthread_rwlockattr_destroy(&attr))
                ERR("Failed to destroy rwlockattr.\n");

            goto fail;
        }

        if (pthread_rwlockattr_destroy(&attr))
            ERR("Failed to destroy rwlockattr.\n");
    }

    return TRUE;

fail:
    dlclose(libnvoptix_handle);
    return FALSE;
}

static void unload_nvoptix(void)
{
    if (libnvoptix_handle)
        dlclose(libnvoptix_handle);

    if (callbacks)
        free(callbacks);

    if (callbacks_enabled() && pthread_rwlock_destroy(&callbacks_lock))
        ERR("Failed to destroy rwlock.\n");
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
            unload_nvoptix();
            break;
    }

    return TRUE;
}
