/*
 * MIT License
 * Copyright (C) 2022 Sveinar Søpler
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

#include "config.h"
#include "wine/port.h"

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);
#include "nvoptix.h"

#pragma push_macro("__declspec")
#undef __declspec
#define __declspec(x) __cdecl

#define _WINDOWS

#pragma pop_macro("__declspec")

static void *libnvoptix_handle = NULL;

static OptixResult (*poptixQueryFunctionTable)(int abiId, unsigned int numOptions, OptixQueryFunctionTableOptions *FuncOpt, const void **Info, void *functionTable, size_t sizeOfTable);
static OptixResult (*prtGetSymbolTable)(void);

OptixResult __cdecl optixQueryFunctionTable(int abiId, unsigned int numOptions, OptixQueryFunctionTableOptions *FuncOpt, const void **Info, void *functionTable, size_t sizeOfTable)
{
    TRACE("(AbiID: %d, numOptions: %u, FunctionTablePtr: %p, InfoPtr: %p, Size: %ld)\n", abiId, numOptions, functionTable, Info, sizeOfTable);
    return poptixQueryFunctionTable(abiId, numOptions, FuncOpt, Info, functionTable, sizeOfTable);
}

OptixResult __cdecl rtGetSymbolTable(void)
{
    TRACE("()\n");
    return prtGetSymbolTable();
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
    LOAD_FUNCPTR(rtGetSymbolTable);

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
