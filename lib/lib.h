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
 * compare two integers:
 *
 * args:
 *  @_a: first integer
 *  @_b: second integer
 *
 * ret:
 *  < 0 if _a < _b
 *  = 0 if _a = _b
 *  > 0 if _a > _b
 */
#define INT_CMP(_a, _b) \
        ((_a) <= (_b) ? ((_a) < (_b) ? -1 : 0) : 1)

/**
 * compare two floats:
 *
 * args:
 *  @_a: first float
 *  @_b: second float
 *
 * ret:
 *  < 0 if _a < _b
 *  = 0 if _a = _b
 *  > 0 if _a > _b
 */
#define FLOAT_CMP(_a, _b) \
        (fabs((_a) - (_b)) < FLT_EPSILON ? 0 : ((_a) - (_b)) > 0 ? 1 : -1)

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
#define NUM_CMP(_name, _numcmp, _type)          \
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
        return _numcmp ## _CMP(a, b);           \
}

NUM_CMP(u64, INT, uint64_t)
NUM_CMP(u32, INT, uint32_t)
NUM_CMP(u16, INT, uint16_t)
NUM_CMP(u8, INT, uint8_t)
NUM_CMP(s64, INT, int64_t)
NUM_CMP(s32, INT, int32_t)
NUM_CMP(s16, INT, int16_t)
NUM_CMP(s8, INT, int8_t)
NUM_CMP(float, FLOAT, float)
NUM_CMP(double, FLOAT, double)
       
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
