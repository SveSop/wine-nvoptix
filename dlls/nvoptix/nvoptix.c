/*
 * Copyright (C) 2021 Krzysztof Bogacki
 * Copyright (C) 2022 Sveinar Søpler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
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

typedef OptixResult (optixQueryFunctionTable_t)(int abiId, unsigned int numOptions, OptixQueryFunctionTableOptions *, const void **, void *function_code, size_t sizeOfTable);
typedef OptixResult (rtGetSymbolTable_t)(void);

OptixResult __cdecl optixQueryFunctionTable(int abiId, unsigned int numOptions, OptixQueryFunctionTableOptions *, const void **, void *function_code, size_t sizeOfTable)
{
    TRACE("(%d, %p)\n", abiId, function_code);
    return optixQueryFunctionTable_t(abiId, numOptions, *, **, *function_code, sizeOfTable);
}

OptixResult __cdecl rtGetSymbolTable(void)
{
    TRACE("()\n");
    return rtGetSymbolTable_t();
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
