#ifndef CTL_ARR_H
#define CTL_ARR_H

#include "../lib/include/util.h"
#include <stdlib.h>
#include <string.h>

/* default initial capacity */
#ifndef CTL_ARR_INIT_CAP
#define CTL_ARR_INIT_CAP 32
#endif /* #ifndef ARR_INIT_CAP */

/* if debugging */
#ifdef DBUG
/**
 * test if state of array is valid:
 *
 * args:
 *  @_ap: pointer to array
 *
 * ret:
 *  may exit process
 */
#define CTL_ARR_OK(_ap) do {                                    \
        dbug((_ap) == NULL, "ap == NULL");                      \
        dbug((_ap)->arr == NULL, "ap->arr == NULL");            \
        dbug((_ap)->cap == 0, "ap->cap == 0");                  \
        dbug((_ap)->len > (_ap)->cap, "ap->len > ap->cap");     \
} while (0)
#else
#define CTL_ARR_OK(_ap)
#endif /* #ifdef DBUG */

/**
 * iterate through array:
 *
 * args:
 *  @_ap: pointer to array
 *  @_p:  element pointer
 */
#define CTL_ARR_FOR_EACH(_ap, _p) \
        for (_p = (_ap)->arr; _p != (_ap)->arr + (_ap)->len; _p++)

/**
 * define a new dynamic array:
 *
 * args:
 *  @_link: linkage of generated functions
 *  @_type: type of array elements
 *  @_name: name of generated struct and prefix of function names
 */
#define CTL_ARR_DEF(_link, _type, _name)                        \
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
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
PUBLIC _link int                                                \
_name ## _init(struct _name *ap, size_t cap)                    \
{                                                               \
        dbug(ap == NULL, "ap == NULL");                         \
                                                                \
        if (cap == 0)                                           \
                cap = CTL_ARR_INIT_CAP;                         \
                                                                \
        ap->arr = calloc(cap, sizeof(_type));                   \
        if (ap->arr == NULL)                                    \
                return -1;                                      \
                                                                \
        ap->cap = cap;                                          \
        ap->len = 0;                                            \
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
PUBLIC _link void                                               \
_name ## _free(struct _name *ap, void (*dtor)(_type))           \
{                                                               \
        size_t i = 0;                                           \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
PRIVATE _link int                                               \
_name ## _grow(struct _name *ap)                                \
{                                                               \
        size_t cap = 0;                                         \
        size_t diff = 0;                                        \
        _type *p = NULL;                                        \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
PRIVATE _link int                                               \
_name ## _grow_if_needed(struct _name *ap)                      \
{                                                               \
        CTL_ARR_OK(ap);                                         \
                                                                \
        /* not ready to grow? */                                \
        if (ap->len < ap->cap)                                  \
                return 0;                                       \
                                                                \
        return _name ## _grow(ap);                              \
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
PRIVATE _link int                                               \
_name ## _shrink(struct _name *ap)                              \
{                                                               \
        size_t cap = 0;                                         \
        _type *p = NULL;                                        \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
PRIVATE _link int                                               \
_name ## _shrink_if_needed(struct _name *ap)                    \
{                                                               \
        CTL_ARR_OK(ap);                                         \
                                                                \
        /* not ready to shrink? */                              \
        if (ap->len > (ap->cap / 2))                            \
                return 0;                                       \
                                                                \
        /* shrinking too much? */                               \
        if ((ap->cap / 2) < CTL_ARR_INIT_CAP)                   \
                return 0;                                       \
                                                                \
        return _name ## _shrink(ap);                            \
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
PRIVATE _link int                                               \
_name ## _grow_by(struct _name *ap, size_t amt)                 \
{                                                               \
        size_t cap = 0;                                         \
        size_t diff = 0;                                        \
        _type *p = NULL;                                        \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
PRIVATE _link int                                               \
_name ## _grow_by_if_needed(struct _name *ap, size_t amt)       \
{                                                               \
        CTL_ARR_OK(ap);                                         \
                                                                \
        if (ap->len + amt < ap->cap)                            \
                return 0;                                       \
                                                                \
        return _name ## _grow_by(ap, amt);                      \
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
PRIVATE _link int                                               \
_name ## _shrink_by(struct _name *ap, size_t amt)               \
{                                                               \
        size_t cap = 0;                                         \
        _type *p = NULL;                                        \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
PRIVATE _link int                                               \
_name ## _shrink_by_if_needed(struct _name *ap, size_t amt)     \
{                                                               \
        size_t newlen = 0;                                      \
                                                                \
        CTL_ARR_OK(ap);                                         \
        dbug(amt > ap->len, "amt > ap->len");                   \
                                                                \
        newlen = ap->len - amt;                                 \
        if (newlen > (ap->cap / 2))                             \
                return 0;                                       \
                                                                \
        if (newlen < CTL_ARR_INIT_CAP)                          \
                return 0;                                       \
                                                                \
        return _name ## _shrink_by(ap, amt);                    \
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
PDQ_DEF(_link, _type, _name)                                    \
PUBLIC _link void                                               \
_name ## _sort(struct _name *ap,                                \
               int (*fn)(const _type, const _type))             \
{                                                               \
        CTL_ARR_OK(ap);                                         \
        dbug(fn == NULL, "fn == NULL");                         \
        _name ## _pdq(ap->arr, ap->len, fn);                    \
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
PUBLIC _link int                                                \
_name ## _addv(struct _name *ap,                                \
               size_t idx,                                      \
               _type *arr,                                      \
               size_t len)                                      \
{                                                               \
        size_t shift = 0;                                       \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
 * remove multiple values to _name:                             \
 *                                                              \
 * args:                                                        \
 *  @ap:   pointer to _name                                     \
 *  @idx:  where to add                                         \
 *  @arr:  destination (pass NULL if do not care)               \
 *  @len:  length of array                                      \
 *  @dtor: optional destructor                                  \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
PUBLIC _link int                                                \
_name ## _rmv(struct _name *ap,                                 \
               size_t idx,                                      \
               _type *arr,                                      \
               size_t len,                                      \
               void (*dtor)(_type))                             \
{                                                               \
        size_t shift = 0;                                       \
        size_t i = 0;                                           \
        _type *src = NULL;                                      \
        _type *dst = NULL;                                      \
                                                                \
        CTL_ARR_OK(ap);                                         \
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
        } else if (dtor != NULL) {                              \
                for (i = 0; i < len; i++)                       \
                        dtor(ap->arr[idx + i]);                 \
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
PUBLIC _link int                                                \
_name ## _add(struct _name *ap, size_t idx, _type v)            \
{                                                               \
        return _name ## _addv(ap, idx, &v, 1);                  \
}                                                               \
                                                                \
/**                                                             \
 * remove value from _name:                                     \
 *                                                              \
 * args:                                                        \
 *  @ap:    pointer to _name                                    \
 *  @idx:  value to remove                                      \
 *  @vp:   pointer to _type (pass NULL if you do not want it)   \
 *  @dtor: optional destructor                                  \
 *                                                              \
 * ret:                                                         \
 *  @success: 0                                                 \
 *  @failure: -1 and errno set                                  \
 */                                                             \
PUBLIC _link int                                                \
_name ## _rm(struct _name *ap,                                  \
             size_t idx,                                        \
             _type *vp,                                         \
             void (*dtor)(_type))                               \
{                                                               \
        return _name ## _rmv(ap, idx, vp, 1, dtor);             \
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
PUBLIC _link size_t                                             \
_name ## _find(const struct _name *ap,                          \
               _type v,                                         \
               int (*cmp)(const _type, const _type))            \
{                                                               \
        size_t i = 0;                                           \
                                                                \
        CTL_ARR_OK(ap);                                         \
        dbug(cmp == NULL, "cmp == NULL");                       \
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
PUBLIC _link int                                                \
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
        CTL_ARR_OK(ap);                                         \
        dbug(cmp == NULL, "cmp == NULL");                       \
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
        CTL_ARR_OK(ap);                                         \
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
PUBLIC _link size_t                                             \
_name ## _bin_find(const struct _name *ap,                      \
                   _type v,                                     \
                   int (*cmp)(const _type, const _type))        \
{                                                               \
        size_t low = 0;                                         \
        size_t high = 0;                                        \
        size_t mid = 0;                                         \
        int diff = 0;                                           \
                                                                \
        CTL_ARR_OK(ap);                                         \
        dbug(cmp == NULL, "cmp == NULL");                       \
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
}                                                               \
                                                                \
/**                                                             \
 * binary removal on _name:                                     \
 *                                                              \
 * args:                                                        \
 *  @ap:   pointer to _name                                     \
 *  @v:    value to remove                                      \
 *  @dtor: optional destructor                                  \
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
PUBLIC _link int                                                \
_name ## _bin_rm(struct _name *ap,                              \
                  _type v,                                      \
                  void (*dtor)(_type),                          \
                  int (*cmp)(const _type, const _type))         \
{                                                               \
        size_t i = 0;                                           \
                                                                \
        CTL_ARR_OK(ap);                                         \
        dbug(cmp == NULL, "cmp == NULL");                       \
                                                                \
        i = _name ## _bin_find(ap, v, cmp);                     \
        if (i == ap->len)                                       \
                return 0;                                       \
                                                                \
        return _name ## _rm(ap, i, NULL, dtor);                 \
}

#endif /* #ifndef CTL_ARR_H */
