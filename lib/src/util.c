#include "../include/util.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void
do_die(const char *file, const char *func, int line, const char *fmt, ...)
{
        va_list va;
        int tmp = 0;

        tmp = errno;
        fprintf(stderr, "[%s:%s:%d]: ", file, func, line);
        va_start(va, fmt);
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

#ifdef CTL_DBUG
        dbug(buf == NULL, "buf == NULL");
#endif /* #ifdef CTL_DBUG */

        endp = ((uint8_t *)buf) + sz;
        for (bufp = buf; bufp < endp; bufp++)
                *bufp = (uint8_t)rand();
}
