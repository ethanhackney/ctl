/**
 * ============================================================
 * dynamic array<link,type,name>
 *  @link: linkage of generated functions
 *  @type: type of array elements
 *  @name: name of generated struct and prefix of all functions
 * ============================================================
 *
 * ====================
 * public ${name}_init:
 *  initialize array
 * ====================
 * args:
 *  @ap:   pointer to ${name}
 *  @cap:  initial capacity or 0 for ARR_INIT_CAP
 *  @argc: argument count
 *  @...:  arguments
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
 * ====================
 * public ${name}_free:
 *  free array
 * ====================
 * args:
 *  @ap:   pointer to ${name}
 *  @dtor: optional element destructor
 *
 * ret:
 *  none
 *  
 * ==========================
 * public ${name}_push:
 *  add value to end of array
 * ==========================
 * args:
 *  @ap: pointer to ${name}
 *  @v:  value to add
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
 * ================================
 * public ${name}_pop:
 *  remove value from end of array
 * ================================
 * args:
 *  @ap: pointer to ${name}
 *  @vp: pointer to type (or null if do not care about element)
 *
 * re:
 *  @success: 0 and if vp provided, *vp set to last element
 *  @failure: -1 and errno set
 *
 * ============================
 * public ${name}_add:
 *  add value to array at index
 * ============================
 * args:
 *  @ap:  pointer to ${name}
 *  @idx: where to add it
 *  @v:   value to add
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
*  ==============================
 * public ${name}_addv:
 *  vector version of ${name}_add
 * ==============================
 * args:
 *  @ap:  pointer to ${name}
 *  @idx: where to add it
 *  @v:   value to add
 *  @arr: vector
 *  @len: vector length
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
 * ======================
 * public ${name}_rm:
 *  remove value at index
 * ======================
 * args:
 *  @ap:  pointer to ${name}
 *  @idx: where to add it
 *  @v:   value to add
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
 * =============================
 * public ${name}_rmv:
 *  vector version of ${name}_rm
 * =============================
 * args:
 *  @ap:  pointer to ${name}
 *  @idx: where to add it
 *  @v:   value to add
 *  @arr: vector
 *  @len: vector length
 *
 * ret:
 *  @sucess:  0
 *  @failure: -1 and errno set
 *
 * ====================
 * public ${name}_find:
 *  linear search
 * ====================
 * args:
 *  @ap:  pointer to ${name}
 *  @v:   value to search for
 *  @cmp: comparison function
 *
 * ========================
 * public ${name}_bin_find:
 *  binary search
 * ========================
 * args:
 *  @ap:  pointer to ${name}
 *  @v:   value to search for
 *  @cmp: comparison function
 *
 * =======================
 * public ${name}_bin_add:
 *  binary insertion
 * =======================
 * args:
 *  @ap:  pointer to ${name}
 *  @v:   value to add
 *  @cmp: comparison function
 */
#ifndef ARR_H
#define ARR_H

#include "lib.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* default initial capacity */
#ifndef ARR_INIT_CAP
#define ARR_INIT_CAP 32
#endif /* #ifndef ARR_INIT_CAP */

/**
 * test if state of array is valid:
 *
 * args:
 *  @_ap: pointer to array
 *
 * ret:
 *  may exit process
 */
#define ARR_OK(_ap) do {                                        \
        dbug((_ap) == NULL, "ap == NULL");                      \
        dbug((_ap)->arr == NULL, "ap->arr == NULL");            \
        dbug((_ap)->cap == 0, "ap->cap == 0");                  \
        dbug((_ap)->len > (_ap)->cap, "ap->len > ap->cap");     \
} while (0)

/**
 * iterate through array:
 *
 * args:
 *  @_ap: pointer to array
 *  @_p:  element pointer
 */
#define ARR_FOR_EACH(_ap, _p) \
        for (_p = (_ap)->arr; _p != (_ap)->arr + (_ap)->len; _p++)

/**
 * define a new dynamic array:
 *
 * args:
 *  @_link: linkage of generated functions
 *  @_type: type of array elements
 *  @_name: name of generated struct
 */
#define ARR_DEF(_link, _type, _name)                            \
                                                                \
struct _name {                                                  \
        size_t  cap; /* physical length of array */             \
        size_t  len; /* logical length of array */              \
        _type  *arr; /* heap array of _type */                  \
};                                                              \
                                                                \
/**                                                             \
 * initialize _name:                                            \
 *                                                              \
 * args:                                                        \
 *  @ap:   pointer to _name                                     \
 *  @cap:  initial capacity or zero for default                 \
 *  @argc: argument count                                       \
 *  @...:  arguments                                            \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _init(struct _name *ap, size_t cap, size_t argc, ...)  \
{                                                               \
        va_list va;                                             \
        size_t i = 0;                                           \
                                                                \
        dbug(ap == NULL, "ap == NULL");                         \
                                                                \
        if (cap == 0)                                           \
                cap = ARR_INIT_CAP;                             \
        if (cap < argc)                                         \
                cap = argc;                                     \
                                                                \
        ap->arr = calloc(cap, sizeof(_type));                   \
        if (ap->arr == NULL)                                    \
                return -1;                                      \
                                                                \
        va_start(va, argc);                                     \
        for (i = 0; i < argc; i++)                              \
                ap->arr[i] = va_arg(va, _type);                 \
        va_end(va);                                             \
                                                                \
        ap->cap = cap;                                          \
        ap->len = argc;                                         \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * free _name:                                                  \
 *                                                              \
 * args:                                                        \
 *  @ap:   pointer to _name                                     \
 *  @dtor: optional destructor                                  \
 */                                                             \
_link void                                                      \
_name ## _free(struct _name *ap, void (*dtor)(_type))           \
{                                                               \
        size_t i = 0;                                           \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        if (dtor == NULL)                                       \
                goto finish;                                    \
                                                                \
        for (i = 0; i < ap->len; i++)                           \
                dtor(ap->arr[i]);                               \
                                                                \
finish:                                                         \
        free(ap->arr);                                          \
        memset(ap, 0, sizeof(*ap));                             \
}                                                               \
                                                                \
/**                                                             \
 * grow _name:                                                  \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _grow(struct _name *ap)                                \
{                                                               \
        size_t cap = 0;                                         \
        size_t diff = 0;                                        \
        _type *p = NULL;                                        \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        /* double the cap */                                    \
        cap = ap->cap * 2;                                      \
        p = ap->arr;                                            \
        p = realloc(p, sizeof(_type) * cap);                    \
        if (p == NULL)                                          \
                return -1;                                      \
                                                                \
        /* zero out newly allocated data */                     \
        diff = cap - ap->cap;                                   \
        memset(p + ap->cap, 0, sizeof(_type) * diff);           \
                                                                \
        ap->arr = p;                                            \
        ap->cap = cap;                                          \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * grow _name if needed:                                        \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _grow_if_needed(struct _name *ap)                      \
{                                                               \
        ARR_OK(ap);                                             \
                                                                \
        /* not ready to grow? */                                \
        if (ap->len < ap->cap)                                  \
                return 0;                                       \
                                                                \
        return _name ## _grow(ap);                              \
}                                                               \
                                                                \
/**                                                             \
 * add value to end of _name:                                   \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @v:  value to add                                           \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _push(struct _name *ap, _type v)                       \
{                                                               \
        ARR_OK(ap);                                             \
                                                                \
        if (_name ## _grow_if_needed(ap) < 0)                   \
                return -1;                                      \
                                                                \
        ap->arr[ap->len] = v;                                   \
        ap->len++;                                              \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * shrink _name:                                                \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _shrink(struct _name *ap)                              \
{                                                               \
        size_t cap = 0;                                         \
        _type *p = NULL;                                        \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        /* half the cap */                                      \
        cap = ap->cap / 2;                                      \
        p = ap->arr;                                            \
        p = realloc(p, sizeof(_type) * cap);                    \
        if (p == NULL)                                          \
                return -1;                                      \
                                                                \
        ap->arr = p;                                            \
        ap->cap = cap;                                          \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * shrink _name if needed:                                      \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _shrink_if_needed(struct _name *ap)                    \
{                                                               \
        ARR_OK(ap);                                             \
                                                                \
        /* not ready to shrink? */                              \
        if (ap->len > (ap->cap / 2))                            \
                return 0;                                       \
                                                                \
        /* shrinking too much? */                               \
        if ((ap->cap / 2) < ARR_INIT_CAP)                       \
                return 0;                                       \
                                                                \
        return _name ## _shrink(ap);                            \
}                                                               \
                                                                \
/**                                                             \
 * remove element from end of _name:                            \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @vp: pointer to _type (pass NULL if you do not want it)     \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _pop(struct _name *ap, _type *vp)                      \
{                                                               \
        ARR_OK(ap);                                             \
                                                                \
        /* empty? */                                            \
        if (ap->len == 0)                                       \
                return 1;                                       \
                                                                \
        if (_name ## _shrink_if_needed(ap) < 0)                 \
                return -1;                                      \
                                                                \
        ap->len--;                                              \
        if (vp != NULL)                                         \
                *vp = ap->arr[ap->len];                         \
                                                                \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * sort _name:                                                  \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @fn: comparison function                                    \
 *   ret:                                                       \
 *    < 0 if a < b                                              \
 *    = 0 if a = b                                              \
 *    > 0 if a > b                                              \
 */                                                             \
_link void                                                      \
_name ## _sort(struct _name *ap,                                \
               int (*fn)(const void *, const void *))           \
{                                                               \
        ARR_OK(ap);                                             \
        dbug(fn == NULL, "fn == NULL");                         \
        qsort(ap->arr, ap->len, sizeof(_type), fn);             \
}                                                               \
                                                                \
/**                                                             \
 * add value to _name:                                          \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @idx: where to put v                                        \
 *  @v:   value to add                                          \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _add(struct _name *ap, size_t idx, _type v)            \
{                                                               \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        ARR_OK(ap);                                             \
        dbug(idx >= ap->cap, "idx >= ap->cap");                 \
                                                                \
        if (_name ## _grow_if_needed(ap) < 0)                   \
                return -1;                                      \
                                                                \
        src = ap->arr + idx;                                    \
        dst = src + 1;                                          \
        shift = sizeof(_type) * (ap->len - idx);                \
        memmove(dst, src, shift);                               \
                                                                \
        ap->arr[idx] = v;                                       \
        ap->len++;                                              \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * remove value from _name:                                     \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @idx: value to remove                                       \
 *  @vp:  pointer to _type (pass NULL if you do not want it)    \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _rm(struct _name *ap, size_t idx, _type *vp)           \
{                                                               \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        ARR_OK(ap);                                             \
        dbug(idx >= ap->len, "idx >= ap->len");                 \
                                                                \
        if (ap->len == 0)                                       \
                return 1;                                       \
                                                                \
        if (_name ## _shrink_if_needed(ap) < 0)                 \
                return -1;                                      \
                                                                \
        if (vp != NULL)                                         \
                *vp = ap->arr[idx];                             \
                                                                \
        dst = ap->arr + idx;                                    \
        src = dst + 1;                                          \
        shift = sizeof(_type) * (ap->len - idx - 1);            \
        memmove(src, dst, shift);                               \
                                                                \
        ap->len--;                                              \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * grow _name by amount:                                        \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @amt: amount                                                \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _grow_by(struct _name *ap, size_t amt)                 \
{                                                               \
        size_t cap = 0;                                         \
        size_t diff = 0;                                        \
        _type *p = NULL;                                        \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        /* expand by amount */                                  \
        cap = ap->cap + amt;                                    \
        p = ap->arr;                                            \
        p = realloc(p, sizeof(_type) * cap);                    \
        if (p == NULL)                                          \
                return -1;                                      \
                                                                \
        /* zero out newly allocated data */                     \
        diff = cap - ap->cap;                                   \
        memset(p + ap->cap, 0, sizeof(_type) * diff);           \
                                                                \
        ap->arr = p;                                            \
        ap->cap = cap;                                          \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * grow _name by amount if needed:                              \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @amt: amount                                                \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _grow_by_if_needed(struct _name *ap, size_t amt)       \
{                                                               \
        ARR_OK(ap);                                             \
                                                                \
        if (ap->len + amt < ap->cap)                            \
                return 0;                                       \
                                                                \
        return _name ## _grow_by(ap, amt);                      \
}                                                               \
                                                                \
/**                                                             \
 * add multiple values to _name:                                \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @idx: where to add                                          \
 *  @arr: array of _type                                        \
 *  @len: length of array                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _addv(struct _name *ap,                                \
               size_t idx,                                      \
               _type *arr,                                      \
               size_t len)                                      \
{                                                               \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        ARR_OK(ap);                                             \
        dbug(idx >= ap->cap, "idx >= cap");                     \
        dbug(len == 0, "len == 0");                             \
        dbug(arr == NULL, "arr == NULL");                       \
                                                                \
        if (_name ## _grow_by_if_needed(ap, len) < 0)           \
                return -1;                                      \
                                                                \
        src = ap->arr + idx;                                    \
        dst = src + len;                                        \
        shift = sizeof(_type) * (ap->len - idx);                \
        memmove(dst, src, shift);                               \
                                                                \
        dst = ap->arr + idx;                                    \
        src = arr;                                              \
        shift = sizeof(_type) * len;                            \
        memcpy(dst, src, shift);                                \
                                                                \
        ap->len += len;                                         \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * shrink _name by amount:                                      \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @amt: amount                                                \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _shrink_by(struct _name *ap, size_t amt)               \
{                                                               \
        size_t cap = 0;                                         \
        _type *p = NULL;                                        \
                                                                \
        ARR_OK(ap);                                             \
        dbug(amt > ap->len, "amt > ap->len");                   \
                                                                \
        /* shrink by amount */                                  \
        cap = ap->cap - amt;                                    \
        p = ap->arr;                                            \
        p = realloc(p, sizeof(_type) * cap);                    \
        if (p == NULL)                                          \
                return -1;                                      \
                                                                \
        ap->arr = p;                                            \
        ap->cap = cap;                                          \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * shrink _name by amount if needed:                            \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @amt: amount                                                \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _shrink_by_if_needed(struct _name *ap, size_t amt)     \
{                                                               \
        size_t new = 0;                                         \
                                                                \
        ARR_OK(ap);                                             \
        dbug(amt > ap->len, "amt > ap->len");                   \
                                                                \
        new = ap->len - amt;                                    \
        if (new > (ap->cap / 2))                                \
                return 0;                                       \
                                                                \
        if (new < ARR_INIT_CAP)                                 \
                return 0;                                       \
                                                                \
        return _name ## _shrink_by(ap, amt);                    \
}                                                               \
                                                                \
/**                                                             \
 * remove multiple values to _name:                             \
 *                                                              \
 * args:                                                        \
 *  @ap:  pointer to _name                                      \
 *  @idx: where to add                                          \
 *  @len: length of array                                       \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
_link int                                                       \
_name ## _rmv(struct _name *ap,                                 \
               size_t idx,                                      \
               _type *arr,                                      \
               size_t len)                                      \
{                                                               \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        ARR_OK(ap);                                             \
        dbug(idx >= ap->len, "idx >= len");                     \
        dbug(len == 0, "len == 0");                             \
        dbug(len > ap->len - idx, "len > ap->len - idx");       \
                                                                \
        if (_name ## _shrink_by_if_needed(ap, len) < 0)         \
                return -1;                                      \
                                                                \
        if (arr != NULL) {                                      \
                src = ap->arr + idx;                            \
                dst = arr;                                      \
                shift = sizeof(_type) * len;                    \
                memcpy(dst, src, shift);                        \
        }                                                       \
                                                                \
        /* is there any thing left? */                          \
        if (idx + len < ap->len) {                              \
                /* shift down */                                \
                src = ap->arr + idx;                            \
                dst = src + len;                                \
                shift = (ap->len - idx - len) * sizeof(_type);  \
                memmove(dst, src, shift);                       \
        }                                                       \
                                                                \
        ap->len -= len;                                         \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * linear search on _name:                                      \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @v:  value to search for                                    \
 *  @cmp:                                                       \
 *   ret:                                                       \
 *    < 0 if a < b                                              \
 *    = 0 if a = b                                              \
 *    > 0 if a > b                                              \
 *                                                              \
 * ret:                                                         \
 *  @success: index of element                                  \
 *  @failure: one past last valid element                       \
 */                                                             \
_link size_t                                                    \
_name ## _find(const struct _name *ap,                          \
               _type v,                                         \
               int (*cmp)(const _type, const _type))            \
{                                                               \
        size_t i = 0;                                           \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        for (i = 0; i < ap->len; i++) {                         \
                if (cmp(ap->arr[i], v) == 0)                    \
                        break;                                  \
        }                                                       \
                                                                \
        return i;                                               \
}                                                               \
                                                                \
/**                                                             \
 * binary insertion on _name:                                   \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @v:  value to add                                           \
 *  @cmp:                                                       \
 *   ret:                                                       \
 *    < 0 if a < b                                              \
 *    = 0 if a = b                                              \
 *    > 0 if a > b                                              \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1                                                \
 */                                                             \
_link size_t                                                    \
_name ## _bin_add(struct _name *ap,                             \
                  _type v,                                      \
                  int (*cmp)(const _type, const _type))         \
{                                                               \
        size_t low = 0;                                         \
        size_t high = 0;                                        \
        size_t mid = 0;                                         \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
        int diff = 0;                                           \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        if (_name ## _grow_if_needed(ap) < 0)                   \
                return -1;                                      \
                                                                \
        low = 0;                                                \
        high = ap->len;                                         \
        while (low < high) {                                    \
                mid = low + (high - low) / 2;                   \
                diff = cmp(ap->arr[mid], v);                    \
                if (diff < 0)                                   \
                        low = mid + 1;                          \
                else                                            \
                        high = mid;                             \
        }                                                       \
                                                                \
        if (low < ap->len) {                                    \
                src = ap->arr + low;                            \
                dst = ap->arr + low + 1;                        \
                shift = sizeof(_type) * (ap->len - low);        \
                memmove(dst, src, shift);                       \
        }                                                       \
                                                                \
        ap->arr[low] = v;                                       \
        ap->len++;                                              \
        return 0;                                               \
}                                                               \
                                                                \
/**                                                             \
 * clear _name:                                                 \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 */                                                             \
 _link void                                                     \
 _name ## _clear(struct _name *ap)                              \
{                                                               \
        ARR_OK(ap);                                             \
        ap->len = 0;                                            \
}                                                               \
                                                                \
/**                                                             \
 * binary search on _name:                                      \
 *                                                              \
 * args:                                                        \
 *  @ap: pointer to _name                                       \
 *  @v:  value to search for                                    \
 *  @cmp:                                                       \
 *   ret:                                                       \
 *    < 0 if a < b                                              \
 *    = 0 if a = b                                              \
 *    > 0 if a > b                                              \
 *                                                              \
 * ret:                                                         \
 *  @success: index of element                                  \
 *  @failure: one past last valid element                       \
 */                                                             \
_link size_t                                                    \
_name ## _bin_find(const struct _name *ap,                      \
                   _type v,                                     \
                   int (*cmp)(const _type, const _type))        \
{                                                               \
        size_t low = 0;                                         \
        size_t high = 0;                                        \
        size_t mid = 0;                                         \
        int diff = 0;                                           \
                                                                \
        ARR_OK(ap);                                             \
                                                                \
        low = 0;                                                \
        high = ap->len;                                         \
        while (low < high) {                                    \
                mid = low + (high - low) / 2;                   \
                diff = cmp(ap->arr[mid], v);                    \
                if (diff < 0)                                   \
                        low = mid + 1;                          \
                else if (diff > 0)                              \
                        high = mid;                             \
                else                                            \
                        return mid;                             \
        }                                                       \
                                                                \
        return ap->len;                                         \
}

#endif /* #ifndef ARR_H */
