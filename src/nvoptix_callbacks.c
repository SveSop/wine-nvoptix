#include "windef.h"
#include "winbase.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvoptix);

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#include "nvoptix.h"

pthread_rwlock_t callbacks_lock;
struct callback_t *callbacks = NULL;
static ptrdiff_t callbacks_count = 0;

_Bool callbacks_enabled(void)
{
    static int enabled = -1;

    if (enabled == -1)
    {
        char *env = getenv("WINE_NVOPTIX_CALLBACKS");

        if (env)
        {
            enabled = atoi(env) ? 1 : 0;

            if (!enabled) WARN("Log callbacks disabled\n");
        }
        else
        {
            enabled = 1;
        }
    }

    return enabled;
}

void *wrap_callback(OptixLogCallback func, void *data)
{
    if (pthread_rwlock_wrlock(&callbacks_lock))
    {
        ERR("(%p, %p): Failed to acquire writer lock\n", func, data);
        return (void*)~(ptrdiff_t)0;
    }

    struct callback_t *new_callbacks = reallocarray(
        callbacks, callbacks_count + 1, sizeof(struct callback_t));

    if (!new_callbacks)
    {
        ERR("(%p, %p): Failed to reallocate callbacks array\n", func, data);

        if (pthread_rwlock_unlock(&callbacks_lock))
            ERR("(%p, %p): Failed to release writer lock\n", func, data);

        return (void*)~(ptrdiff_t)0;
    }

    callbacks = new_callbacks;

    struct callback_t *callback = &callbacks[callbacks_count];

    *(void**)&callback->func = func;
    callback->data = data;

    ptrdiff_t offset = callbacks_count++;

    if (pthread_rwlock_unlock(&callbacks_lock))
        ERR("(%p, %p): Failed to release writer lock\n", func, data);

    TRACE("(%p, %p) = %td\n", func, data, offset);

    return (void*)offset;
}

void log_callback(unsigned int level, const char *tag, const char *message, void *cbdata)
{
    TRACE("(%u, %s, %p, %p)\n", level, tag, message, cbdata);

    ptrdiff_t offset = (ptrdiff_t)cbdata;

    if (offset < 0 || offset >= callbacks_count)
    {
        ERR("Failed to find callback for offset = %td\n", offset);
        return;
    }

    if (pthread_rwlock_rdlock(&callbacks_lock))
    {
        ERR("Failed to acquire reader lock for offset = %td\n", offset);
        return;
    }

    struct callback_t *callback = &callbacks[offset];

    OptixLogCallback __attribute((ms_abi)) func = callback->func;
    void *data = callback->data;

    if (pthread_rwlock_unlock(&callbacks_lock))
        ERR("Failed to release reader lock for offset = %td\n", offset);

    func(level, tag, message, data);
}
