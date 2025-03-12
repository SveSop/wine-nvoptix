#pragma once

#include "windef.h"
#include "ntuser.h"
#include "nvoptix.h"

struct callback_t
{
    OptixLogCallback [[gnu::ms_abi]] func;
    void* data;
};

struct log_callback_params
{
    struct dispatch_callback_params dispatch;
    struct callback_t callback;
    unsigned int level;
    size_t offset;
    char buffer[1];
};

NTSTATUS WINAPI call_log_callback(void *args, ULONG size);
