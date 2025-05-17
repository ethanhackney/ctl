#include "lib.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void
do_die(const char *file, const char *func, int line, const char *fmt, ...)
{
        va_list va;
        int tmp = 0;

        fprintf(stderr, "[%s:%s:%d]: ", file, func, line);
        va_start(va, fmt);
        tmp = errno;
        vfprintf(stderr, fmt, va);
        va_end(va);
        fprintf(stderr, ": %s\n", strerror(tmp));
        exit(EXIT_FAILURE);
}

void
buf_rand(void *buf, size_t sz)
{
        uint8_t *bufp = NULL;
        uint8_t *endp = NULL;

        dbug(buf == NULL, "buf == NULL");

        endp = ((uint8_t *)buf) + sz;
        for (bufp = buf; bufp < endp; bufp++)
                *bufp = rand();
}
