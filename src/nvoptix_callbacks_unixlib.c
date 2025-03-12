#if 0
#pragma makedep unix
#endif

#define WINE_UNIX_LIB

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "nvoptix_callbacks_unixlib.h"

pthread_rwlock_t callbacks_lock;
struct callback_t *callbacks = NULL;
UINT64 log_callback = 0;
static size_t callbacks_count = 0;

void *wrap_callback(OptixLogCallback func, void *data)
{
    if (pthread_rwlock_wrlock(&callbacks_lock))
    {
        return (void*)~(size_t)0;
    }

    struct callback_t *new_callbacks = reallocarray(callbacks, callbacks_count + 1, sizeof(struct callback_t));

    if (!new_callbacks)
    {
        pthread_rwlock_unlock(&callbacks_lock);

        return (void*)~(size_t)0;
    }

    callbacks = new_callbacks;

    struct callback_t *callback = &callbacks[callbacks_count];

    *(void**)&callback->func = func;
    callback->data = data;

    size_t offset = callbacks_count++;

    pthread_rwlock_unlock(&callbacks_lock);

    return (void*)offset;
}

void unix_log_callback(unsigned int level, const char *tag, const char *message, void *cbdata)
{
    size_t offset = (size_t)cbdata;

    if (offset < 0 || offset >= callbacks_count || pthread_rwlock_rdlock(&callbacks_lock))
        return;

    struct callback_t callback = callbacks[offset];

    pthread_rwlock_unlock(&callbacks_lock);

    size_t message_len = strlen(message);
    size_t tag_len = strlen(tag);
    size_t len = offsetof(struct log_callback_params, buffer) + message_len + tag_len + 2;

    struct log_callback_params *params = malloc(len);

    if (!params)
        return;

    offset = tag_len + 1;

    params->dispatch.callback = log_callback;
    params->callback = callback;
    params->level = level;
    params->offset = offset;
    memcpy(&params->buffer, tag, offset);
    memcpy(&params->buffer + offset, message, message_len + 1);

    void *ret_ptr;
    ULONG ret_len;

    KeUserDispatchCallback(&params->dispatch, len, &ret_ptr, &ret_len);

    free(params);
}
