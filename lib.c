#include "lib.h"
#include <errno.h>
#include <string.h>

/**
 * print message + errno message and exit:
 *
 * args:
 *  @fmt: format string
 *  @...: arguments
 *
 * ret:
 *  exit process
 */
void
die(const char *fmt, ...)
{
        va_list va;
        int tmp = 0;

        va_start(va, fmt);
        tmp = errno;
        vfprintf(stderr, fmt, va);
        va_end(va);
        fprintf(stderr, ": %s\n", strerror(tmp));
        exit(EXIT_FAILURE);
}
