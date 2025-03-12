/*
 * MIT License
 * Copyright (C) 2023-2025 Sveinar Søpler
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

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wine/debug.h"
#include "wine/unixlib.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include "nvoptix.h"
#include "nvoptix_private.h"
#include "nvoptix_callbacks.h"
#include "nvoptix_105.h"
// #include "nvoptix_93.h"
// #include "nvoptix_87.h"
// #include "nvoptix_84.h"
// #include "nvoptix_68.h"
// #include "nvoptix_60.h"
// #include "nvoptix_55.h"
// #include "nvoptix_47.h"
// #include "nvoptix_41.h"
// #include "nvoptix_36.h"
// #include "nvoptix_22.h"

static OptixResult attach_result;

OptixResult __cdecl optixQueryFunctionTable(
    int abiId,
    unsigned int numOptions,
    int *optionKeys,
    const void **optionValues,
    void *functionTable,
    size_t sizeOfTable)
{
    struct optixQueryFunctionTable_params params = { abiId, numOptions, optionKeys, optionValues, sizeOfTable };
    NTSTATUS status;

    TRACE("(%d, %u, %p, %p, %p, %zu)\n", abiId, numOptions, optionKeys, optionValues, functionTable, sizeOfTable);

    if (numOptions) FIXME("unexpected numOptions = %u\n", numOptions);

    if (optionKeys) FIXME("unexpected optionKeys = %p\n", optionKeys);

    if (optionValues) FIXME("unexpected optionValues = %p\n", optionValues);

    if (attach_result)
    {
        ERR("unexpected attach result %d\n", attach_result);
        return attach_result;
    }
    else if (abiId > OPTIX_MAX_ABI_VERSION)
    {
        ERR("abiId = %d > %d not supported\n", abiId, OPTIX_MAX_ABI_VERSION);
        return OPTIX_ERROR_UNSUPPORTED_ABI_VERSION;
    }
    else if (sizeOfTable > sizeof(CAT2(OptixFunctionTable_, OPTIX_MAX_ABI_VERSION)))
    {
        ERR("sizeOfTable = %zu > %zu not supported\n", sizeOfTable, sizeof(CAT2(OptixFunctionTable_, OPTIX_MAX_ABI_VERSION)));
        return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH;
    }
    else if ((status = NVOPTIX_CALL(optixQueryFunctionTable, &params)))
    {
        ERR("WINE_UNIX_CALL(optixQueryFunctionTable, abiId = %d) failed, status %lx\n", abiId, (unsigned long)status);
        return OPTIX_ERROR_INTERNAL_ERROR;
    }

    if (!params._result)
    {
        switch (abiId)
        {

#define CASE(abi) case abi: \
                if (sizeOfTable != sizeof(CAT2(OptixFunctionTable_, abi))) \
                { \
                    ERR("sizeOfTable = %zu != %zu for abiId = %d\n", sizeOfTable, sizeof(CAT2(OptixFunctionTable_, abi)), abi); \
                    return OPTIX_ERROR_FUNCTION_TABLE_SIZE_MISMATCH; \
                } \
                *(OptixFunctionTable_ ## abi*)functionTable = optixFunctionTable_ ## abi; \
                break;

            CASE(105);
            // CASE(93);
            // CASE(87);
            // CASE(84);
            // CASE(68);
            // CASE(60);
            // CASE(55);
            // CASE(47);
            // CASE(41);
            // CASE(36);
            // CASE(22);
#undef CASE
            default:
                ERR("abiId = %d not supported\n", abiId);
                return OPTIX_ERROR_UNSUPPORTED_ABI_VERSION;
        }
    }

    return params._result;
}

int __cdecl rtGetSymbolTable()
{
    FIXME("(): not implemented\n");
    return ~0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    struct attach_params attach_params = { .log_callback = (UINT64)call_log_callback };
    NTSTATUS status;

    TRACE("(%p, %lu, %p)\n", instance, (unsigned long)reason, reserved);

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(instance);
            if ((status = __wine_init_unix_call()))
            {
                ERR("__wine_init_unix_call failed, status %lx\n", (unsigned long)status);
                return FALSE;
            }
            if ((status = NVOPTIX_CALL(attach, &attach_params)))
            {
                ERR("WINE_UNIX_CALL(attach) failed, status %lx\n", (unsigned long)status);
                return FALSE;
            }
            attach_result = attach_params.result;
            break;
        case DLL_PROCESS_DETACH:
            if (reserved)
                break;
            NVOPTIX_CALL(detach, NULL);
            break;
    }

    return TRUE;
}
