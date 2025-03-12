#include <stdarg.h>
#include "windows.h"
#include "nvoptix_callbacks.h"
#include "ntstatus.h"

NTSTATUS WINAPI call_log_callback(void *args, ULONG size)
{
    struct log_callback_params *params = args;

    params->callback.func(params->level, params->buffer, params->buffer + params->offset, params->callback.data);

    return STATUS_SUCCESS;
}