#pragma once

#define WINE_UNIX_LIB

#include <pthread.h>

#include "winternl.h"
#include "winbase.h"
#include "ntstatus.h"
#include "wine/unixlib.h"

#include "nvoptix_callbacks.h"

extern pthread_rwlock_t callbacks_lock;
extern struct callback_t *callbacks;
extern UINT64 log_callback;
void *wrap_callback(OptixLogCallback func, void *data);
void unix_log_callback(unsigned int level, const char *tag, const char *message, void *cbdata);
