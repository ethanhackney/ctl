#ifndef LIB_H
#define LIB_H

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* these help scripts find private/public functions */
#define PRIVATE
#define PUBLIC

/**
 * get min of two numbers:
 *
 * args:
 *  @_a: first number
 *  @_b: second number
 *
 * ret:
 *  @success: min number
 *  @failure: does not
 *
 * NOTE:
 *  has side effects
 */
#define MIN(_a, _b) \
        ((_a) < (_b) ? (_a) : (_b))

/**
 * convert to string:
 *
 * args:
 *  @v: value to convert
 *
 * ret:
 *  string of v
 */
#define TO_STR(v) #v

/**
 * generate function to compare two numbers:
 *
 * args:
 *  @_name: function name prefix 
 *  @_type: type of arguments
 *
 * ret:
 *  < 0 if _a < _b
 *  = 0 if _a = _b
 *  > 0 if _a > _b
 */
#define INT_CMP(_name, _type)                   \
/**                                             \
 * compare two numbers:                         \
 *                                              \
 * args:                                        \
 *  @a: first number                            \
 *  @b: second number                           \
 *                                              \
 * ret:                                         \
 *  < 0 if _a < _b                              \
 *  = 0 if _a = _b                              \
 *  > 0 if _a > _b                              \
 */                                             \
static inline int                               \
_name ## _cmp(const _type a, const _type b)     \
{                                               \
        if (a < b)                              \
                return -1;                      \
        else if (a == b)                        \
                return 0;                       \
        else                                    \
                return 1;                       \
}

INT_CMP(u64, uint64_t)
INT_CMP(u32, uint32_t)
INT_CMP(u16, uint16_t)
INT_CMP(u8, uint8_t)
INT_CMP(s64, int64_t)
INT_CMP(s32, int32_t)
INT_CMP(s16, int16_t)
INT_CMP(s8, int8_t)
       
/**
 * randomize contents of buffer:
 *
 * args:
 *  @buf:  buffer
 *  @sz:   size of buffer
 */
void buf_rand(void *buf, size_t sz);

/**
 * print message + errno message and exit:
 *
 * args:
 *  @file: file
 *  @func: function
 *  @line: line
 *  @fmt:  format string
 *  @...:  arguments
 *
 * ret:
 *  exit process
 */
void do_die(const char *file, const char *func, int line, const char *fmt, ...);

/**
 * print message + errno message and exit:
 *
 * args:
 *  @_fmt: format string
 *  @...:  arguments
 *
 * ret:
 *  exit process
 */
#define die(_fmt, ...) \
        do_die(__FILE__, __func__, __LINE__, _fmt, ##__VA_ARGS__)

/* if debugging */
#ifdef DBUG
/**
 * test condition and if true, print message and exit:
 *
 * args:
 *  @cond: condition to test
 *  @file: name of file
 *  @func: name of function
 *  @line: line number
 *  @fmt:  format string
 *  @...:  arguments
 *
 * ret:
 *  exit process
 */
static inline void
do_dbug(bool cond,
        const char *file,
        const char *func,
        int line,
        const char *fmt, ...)
{
        va_list va;

        if (!cond)
                return;

        fprintf(stderr, "[%s:%s:%d]: ", file, func, line);
        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        va_end(va);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
}
#else
static inline void
do_dbug(bool cond,
        const char *file,
        const char *func,
        int line,
        const char *fmt, ...)
{
}
#endif /* #ifdef DBUG */

/**
 * test condition and if true, print message and exit:
 *
 * args:
 *  @_cond: condition to test
 *  @_fmt:  format string
 *  @...:   arguments
 *
 * ret:
 *  exit process
 */
#define dbug(_cond, _fmt, ...) \
        do_dbug(_cond, __FILE__, __func__, __LINE__, _fmt, ##__VA_ARGS__)

#endif /* #ifndef LIB_H */
