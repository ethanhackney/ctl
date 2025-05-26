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

#define PDQ_THRESH 24       /* pdqsort threshold */
#define PDQ_RECUR  (1 << 7) /* pdqsort recursion limit */

/**
 * generate a pdqsort:
 *
 * args:
 *  @_link: linkage of generated functions
 *  @_type: type of array
 *  @_name: prefix of function names
 *  @_cmp:  comparison function
 *
 * ret:
 *  nothing
 */
#define PDQ_DEF(_link, _type, _name)                                            \
                                                                                \
/**                                                                             \
 * insertion sort:                                                              \
 *                                                                              \
 * args:                                                                        \
 *  @arr: array of _type                                                        \
 *  @len: length of array                                                       \
 *  @cmp: comparison function                                                   \
 *                                                                              \
 * ret:                                                                         \
 *  nothing                                                                     \
 */                                                                             \
_link void                                                                      \
_name ## _isort(_type *arr, size_t len, int (*cmp)(const _type, const _type))   \
{                                                                               \
        size_t i = 0;                                                           \
        size_t j = 0;                                                           \
        _type v;                                                                \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        for (i = 1; i < len; i++) {                                             \
                v = arr[i];                                                     \
                for (j = i; j > 0; j--) {                                       \
                        if (cmp(arr[j - 1], v) < 0)                             \
                                break;                                          \
                        arr[j] = arr[j - 1];                                    \
                }                                                               \
                arr[j] = v;                                                     \
        }                                                                       \
}                                                                               \
                                                                                \
/**                                                                             \
 * heapify array:                                                               \
 *                                                                              \
 * args:                                                                        \
 *  @arr:  array of _type                                                       \
 *  @len:  length of array                                                      \
 *  @cmp:  comparison function                                                  \
 *  @root: root node                                                            \
 */                                                                             \
_link void                                                                      \
_name ## _heapify(_type *arr,                                                   \
                  size_t len,                                                   \
                  size_t root,                                                  \
                  int (*cmp)(const _type, const _type))                         \
{                                                                               \
        size_t max = 0;                                                         \
        size_t left = 0;                                                        \
        size_t right = 0;                                                       \
        _type tmp;                                                              \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(root >= len, "root >= len");                                       \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
                                                                                \
        for (;;) {                                                              \
                max = root;                                                     \
                left = (root * 2) + 1;                                          \
                right = (root * 2) + 2;                                         \
                                                                                \
                if (left < len) {                                               \
                        if (cmp(arr[left], arr[max]) > 0)                       \
                                max = left;                                     \
                }                                                               \
                                                                                \
                if (right < len) {                                              \
                        if (cmp(arr[right], arr[max]) > 0)                      \
                                max = right;                                    \
                }                                                               \
                                                                                \
                if (max == root)                                                \
                        break;                                                  \
                                                                                \
                tmp = arr[root];                                                \
                arr[root] = arr[max];                                           \
                arr[max] = tmp;                                                 \
                root = max;                                                     \
        }                                                                       \
}                                                                               \
                                                                                \
/**                                                                             \
 * heapsort:                                                                    \
 *                                                                              \
 * args:                                                                        \
 *  @arr: array of _type                                                        \
 *  @len: length of array                                                       \
 *  @cmp: comparison function                                                   \
 */                                                                             \
_link void                                                                      \
_name ## _heapsort(_type *arr,                                                  \
                  size_t len,                                                   \
                  int (*cmp)(const _type, const _type))                         \
{                                                                               \
        size_t root = 0;                                                        \
        size_t i = 0;                                                           \
        _type tmp;                                                              \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
                                                                                \
        for (i = len / 2; i > 0; i--)                                           \
                _name ## _heapify(arr, len, i - 1, cmp);                        \
                                                                                \
        root = 0;                                                               \
        for (i = len; i > 1; i--) {                                             \
                tmp = arr[root];                                                \
                arr[root] = arr[i - 1];                                         \
                arr[i - 1] = tmp;                                               \
                _name ## _heapify(arr, i - 1, root, cmp);                       \
        }                                                                       \
}                                                                               \
                                                                                \
/**                                                                             \
 * test if array is sorted:                                                     \
 *                                                                              \
 * args:                                                                        \
 *  @arr: array                                                                 \
 *  @len: array length                                                          \
 *  @cmp: comparison function                                                   \
 *                                                                              \
 * ret:                                                                         \
 *  @true:  if sorted                                                           \
 *  @false: if not                                                              \
 */                                                                             \
_link bool                                                                      \
_name ## _is_sorted(_type *arr,                                                 \
                    size_t len,                                                 \
                    int (*cmp)(const _type, const _type))                       \
{                                                                               \
        size_t i = 0;                                                           \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        for (i = 1; i < len; i++) {                                             \
                if (cmp(arr[i], arr[i - 1]) < 0)                                \
                        return false;                                           \
        }                                                                       \
                                                                                \
        return true;                                                            \
}                                                                               \
                                                                                \
/**                                                                             \
 * get median of three values:                                                  \
 *                                                                              \
 * args:                                                                        \
 *  @a:   pointer to first value                                                \
 *  @b:   pointer to second value                                               \
 *  @c:   pointer to third value                                                \
 *  @cmp: comparison function                                                   \
 *                                                                              \
 * ret:                                                                         \
 *  median of a, b, and c                                                       \
 */                                                                             \
_link _type                                                                     \
_name ## _median(_type *a,                                                      \
                 _type *b,                                                      \
                 _type *c,                                                      \
                 int (*cmp)(const _type, const _type))                          \
{                                                                               \
        int ab_diff = 0;                                                        \
        int bc_diff = 0;                                                        \
        int ac_diff = 0;                                                        \
                                                                                \
        dbug(a == NULL, "a == NULL");                                           \
        dbug(b == NULL, "b == NULL");                                           \
        dbug(c == NULL, "c == NULL");                                           \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        ab_diff = cmp(*a, *b);                                                  \
        bc_diff = cmp(*b, *c);                                                  \
        ac_diff = cmp(*a, *c);                                                  \
                                                                                \
        if ((ab_diff > 0) != (ac_diff > 0))                                     \
                return *a;                                                      \
        else if ((ab_diff < 0) != (bc_diff > 0))                                \
                return *b;                                                      \
        else                                                                    \
                return *c;                                                      \
}                                                                               \
                                                                                \
/**                                                                             \
 * partition array:                                                             \
 *                                                                              \
 * args:                                                                        \
 *  @arr:   array of _type                                                      \
 *  @len:   length of array                                                     \
 *  @pivot: pivot                                                               \
 *  @cmp:   comparison function                                                 \
 *                                                                              \
 * ret:                                                                         \
 *  pointer to partition point                                                  \
 */                                                                             \
_link _type *                                                                   \
_name ## _part(_type *arr,                                                      \
               size_t len,                                                      \
               _type pivot,                                                     \
               int (*cmp)(const _type, const _type))                            \
{                                                                               \
        _type *p = NULL;                                                        \
        _type *end = NULL;                                                      \
        _type tmp;                                                              \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        p = arr;                                                                \
        end = p + len;                                                          \
        for (;;) {                                                              \
                while (cmp(*p, pivot) < 0)                                      \
                        p++;                                                    \
                                                                                \
                end--;                                                          \
                while (cmp(*end, pivot) > 0)                                    \
                        end--;                                                  \
                                                                                \
                if (p >= end)                                                   \
                        return p;                                               \
                                                                                \
                tmp = *p;                                                       \
                *p = *end;                                                      \
                *end = tmp;                                                     \
                p++;                                                            \
        }                                                                       \
}                                                                               \
                                                                                \
/**                                                                             \
 * do pdqsort:                                                                  \
 *                                                                              \
 * args:                                                                        \
 *  @arr:   array                                                               \
 *  @len:   length of array                                                     \
 *  @depth: recursion depth                                                     \
 *  @cmp:   comparison function                                                 \
 */                                                                             \
_link void                                                                      \
_name ## _do_pdq(_type *arr,                                                    \
                 size_t len,                                                    \
                 size_t depth,                                                  \
                 int (*cmp)(const _type, const _type))                          \
{                                                                               \
        size_t mid = 0;                                                         \
        size_t leftlen = 0;                                                     \
        size_t rightlen = 0;                                                    \
        _type *p = NULL;                                                        \
        _type pivot;                                                            \
                                                                                \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        if (len <= PDQ_THRESH) {                                                \
                _name ## _isort(arr, len, cmp);                                 \
                return;                                                         \
        }                                                                       \
                                                                                \
        if (depth == 0) {                                                       \
                _name ## _heapsort(arr, len, cmp);                              \
                return;                                                         \
        }                                                                       \
                                                                                \
        if (_name ## _is_sorted(arr, len, cmp))                                 \
                return;                                                         \
                                                                                \
        mid = len / 2;                                                          \
        pivot = _name ## _median(arr, arr + mid, arr + len - 1, cmp);           \
        p = _name ## _part(arr, len, pivot, cmp);                               \
                                                                                \
        leftlen = (size_t)(p - arr);                                            \
        rightlen = len - leftlen;                                               \
        _name ## _do_pdq(arr, leftlen, depth - 1, cmp);                         \
        _name ## _do_pdq(p, rightlen, depth - 1, cmp);                          \
}                                                                               \
                                                                                \
/**                                                                             \
 * pdqsort:                                                                     \
 *                                                                              \
 * args:                                                                        \
 *  @arr:   array                                                               \
 *  @len:   length of array                                                     \
 *  @depth: recursion depth                                                     \
 *  @cmp:   comparison function                                                 \
 */                                                                             \
_link void                                                                      \
_name ## _pdq(_type *arr,                                                       \
             size_t len,                                                        \
             int (*cmp)(const _type, const _type))                              \
{                                                                               \
        dbug(arr == NULL, "arr == NULL");                                       \
        dbug(len == 0, "len == 0");                                             \
        dbug(cmp == NULL, "cmp == NULL");                                       \
                                                                                \
        _name ## _do_pdq(arr, len, PDQ_RECUR, cmp);                             \
}

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
