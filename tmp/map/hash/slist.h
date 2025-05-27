#ifndef CTL_HASH_SLIST_H
#define CTL_HASH_SLIST_H

#include "hash.h"
#include <stdlib.h>

/* if debugging */
#ifdef CTL_DBUG
/**
 * test if state of hash map is valid:
 *
 * args:
 *  @_name: name of struct
 *  @_hsp:  pointer to hash map
 *
 * ret:
 *  may exit process
 */
#define CTL_HASH_SLIST_OK(_name, _hsp) do {                             \
        struct _name ## _slink *_p = NULL;                              \
        ctl_hash_size_t _cap = 0;                                       \
        ctl_hash_size_t _len = 0;                                       \
        ctl_hash_size_t _i = 0;                                         \
                                                                        \
        dbug((_hsp) == NULL, "hsp == NULL");                            \
        dbug((_hsp)->arr == NULL, "hsp->arr == NULL");                  \
        dbug((_hsp)->cmp == NULL, "hsp->cmp == NULL");                  \
        dbug((_hsp)->seed == 0, "hsp->seed == 0");                      \
                                                                        \
        _cap = (_hsp)->cap;                                             \
        dbug(_cap < CTL_HASH_INIT_CAP, "hsp->cap < CTL_HASH_INIT_CAP"); \
        dbug((_cap & (_cap - 1)) != 0, "hsp->cap not power of 2");      \
                                                                        \
        dbug((_hsp)->len > _cap, "hsp->len > hsp->cap");                \
        _len = 0;                                                       \
        for (_i = 0; _i < (_cap); _i++) {                               \
                for (_p = (_hsp)->arr[_i]; _p != NULL; _p = _p->next)   \
                        _len++;                                         \
        }                                                               \
        dbug(_len != (_hsp)->len, "hsp->len != len");                   \
} while (0)

/**
 * do dbug:
 *
 * args:
 *  @_cond: condition
 *  @_fmt:  format string
 *  @...:   arguments
 *
 * ret:
 *  @success: nothing
 *  @failure: exit process
 */
#define CTL_HASH_SLIST_DBUG(_cond, _fmt, ...) \
        dbug(_cond, _fmt, ##__VA_ARGS__)

#else
#define CTL_HASH_SLIST_OK(_name, _hsp)        /* no-op */
#define CTL_HASH_SLIST_DBUG(_cond, _fmt, ...) /* no-op */
#endif /* #ifdef CTL_DBUG */

/**
 * iterate through hash map safely:
 *
 * args:
 *  @_hsp:  pointer to hash map
 *  @_i:    index
 *  @_p:    pointer to link
 *  @_next: pointer to next link
 *  @_seen: seen count
 */
#define CTL_HASH_SLIST_FOR_EACH_SAFE(_hsp, _i, _p, _next, _seen)        \
        for (_i = 0; _seen < (_hsp)->len; _i++)                         \
                for (_p = (_hsp)->arr[_i];                              \
                     _p != NULL && ((_next = (_p)->next), 1);           \
                     _p = _next, _seen++)

/**
 * iterate through hash map:
 *
 * args:
 *  @_name: name of hash map
 *  @_hsp:  pointer to hash map
 *  @_kp:   key-value pair pointer
 *  @_body: body of loop
 */
#define CTL_HASH_SLIST_FOR_EACH(_name, _hsp, _kp, _body) do {           \
        struct _name ## _slink *_p = NULL;                              \
        ctl_hash_size_t _seen = 0;                                      \
        ctl_hash_size_t _i = 0;                                         \
                                                                        \
        for (_i = 0; _seen < (_hsp)->len; _i++) {                       \
                for (_p = (_hsp)->arr[_i]; _p != NULL; _p = _p->next) { \
                        _kp = &_p->kvp;                                 \
                        _body;                                          \
                        _seen++;                                        \
                }                                                       \
        }                                                               \
} while (0)

/**
 * define hash function for pointer type:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_PTR_HASH_DEF(_link, _ktype, _name)               \
/**                                                                     \
 * hash key:                                                            \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name                                              \
 *  @key: key                                                           \
 *  @len: key length                                                    \
 *                                                                      \
 * ret:                                                                 \
 *  @success: hash                                                      \
 *  @failure: does not                                                  \
 */                                                                     \
PRIVATE _link ctl_hash_size_t                                           \
_name ## _do_hash(struct _name *hsp, const _ktype key, size_t len)      \
{                                                                       \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
        return ctl_hash(key, len, hsp->seed);                           \
}

/**
 * define hash function for non-pointer type:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type of array elements
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_HASH_DEF(_link, _ktype, _name)                   \
/**                                                                     \
 * hash key:                                                            \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name                                              \
 *  @key: key                                                           \
 *  @len: key length                                                    \
 *                                                                      \
 * ret:                                                                 \
 *  @success: hash                                                      \
 *  @failure: does not                                                  \
 */                                                                     \
PRIVATE _link ctl_hash_size_t                                           \
_name ## _do_hash(struct _name *hsp, const _ktype key, size_t len)      \
{                                                                       \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
        return ctl_hash(&key, len, hsp->seed);                          \
}

/**
 * declare a new dynamic hash map struct with singly linked chaining:
 *
 * args:
 *  @_ktype: key type of map
 *  @_vtype: value type of map
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_DECL(_ktype, _vtype, _name)                      \
                                                                        \
/* key-value pair */                                                    \
struct _name ## _kvp {                                                  \
        _ktype kv_key; /* key */                                        \
        _vtype kv_val; /* value */                                      \
        size_t kv_len; /* key length */                                 \
};                                                                      \
                                                                        \
/* hash link */                                                         \
struct _name ## _slink {                                                \
        struct _name ## _slink *next; /* next on list */                \
        struct _name ## _kvp    kvp;  /* key-value pair */              \
        ctl_hash_size_t         hash; /* saved hash */                  \
};                                                                      \
                                                                        \
/* hash map */                                                          \
struct _name {                                                          \
        struct _name ## _slink **arr;  /* heap array of links */        \
        ctl_hash_size_t          cap;  /* physical length of map */     \
        ctl_hash_size_t          len;  /* logical length of map */      \
        ctl_hash_size_t          seed; /* seed */                       \
        /**                                                             \
         * key compare:                                                 \
         *                                                              \
         * args:                                                        \
         *  @a:    first key                                            \
         *  @alen: first key length                                     \
         *  @b:    second key                                           \
         *  @blen: second key length                                    \
         *                                                              \
         * ret:                                                         \
         *  = 0 if a  = b                                               \
         *  ! 0 if a != b                                               \
         */                                                             \
        int (*cmp)(const _ktype a,                                      \
                   size_t alen,                                         \
                   const _ktype b,                                      \
                   size_t blen);                                        \
};


/**
 * define a new dynamic hash map with singly linked chaining:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type of map
 *  @_vtype: value type of map
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_DO_HASH_SLIST_DEF(_link, _ktype, _vtype, _name)             \
                                                                        \
/**                                                                     \
 * initialize _name:                                                    \
 *                                                                      \
 * args:                                                                \
 *  @hsp:  pointer to _name:                                            \
 *  @seed: seed                                                         \
 *  @cmp:  key compare                                                  \
 *   ret:                                                               \
 *    = 0 if a  = b                                                     \
 *    ! 0 if a != b                                                     \
 *                                                                      \
 * ret:                                                                 \
 *  @success: 0                                                         \
 *  @failure: -1 and errno set                                          \
 */                                                                     \
PUBLIC _link int                                                        \
_name ## _init(struct _name *hsp,                                       \
               ctl_hash_size_t seed,                                    \
               int (*cmp)(const _ktype a,                               \
                          size_t alen,                                  \
                          const _ktype b,                               \
                          size_t blen))                                 \
{                                                                       \
        CTL_HASH_SLIST_DBUG(hsp == NULL, "hsp == NULL");                \
        CTL_HASH_SLIST_DBUG(cmp == NULL, "cmp == NULL");                \
                                                                        \
        if (seed == 0)                                                  \
                seed = (ctl_hash_size_t)time(NULL);                     \
                                                                        \
        hsp->cap = CTL_HASH_INIT_CAP;                                   \
        hsp->arr = calloc(hsp->cap, sizeof(*hsp->arr));                 \
        if (hsp->arr == NULL)                                           \
                return -1;                                              \
                                                                        \
        hsp->seed = seed;                                               \
        hsp->cmp = cmp;                                                 \
        hsp->len = 0;                                                   \
        return 0;                                                       \
}                                                                       \
                                                                        \
/**                                                                     \
 * free _name:                                                          \
 *                                                                      \
 * args:                                                                \
 *  @hsp:  pointer to _name                                             \
 *  @dtor: optional destructor                                          \
 */                                                                     \
PUBLIC _link void                                                       \
_name ## _free(struct _name *hsp, void (*dtor)(struct _name ## _kvp *)) \
{                                                                       \
        struct _name ## _slink *next = NULL;                            \
        struct _name ## _slink *p = NULL;                               \
        ctl_hash_size_t seen = 0;                                       \
        ctl_hash_size_t i = 0;                                          \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        CTL_HASH_SLIST_FOR_EACH_SAFE(hsp, i, p, next, seen) {           \
                if (dtor != NULL)                                       \
                        dtor(&p->kvp);                                  \
                free(p);                                                \
        }                                                               \
                                                                        \
        free(hsp->arr);                                                 \
        memset(hsp, 0, sizeof(*hsp));                                   \
}                                                                       \
                                                                        \
/**                                                                     \
 * do search for entry:                                                 \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name                                              \
 *  @key: key                                                           \
 *  @len: key length                                                    \
 *  @i:   bucket                                                        \
 *                                                                      \
 * ret:                                                                 \
 *  @success: pointer to entry                                          \
 *  @failure: NULL                                                      \
 */                                                                     \
PRIVATE _link struct _name ## _slink *                                  \
_name ## _do_get(struct _name *hsp,                                     \
                 const _ktype key,                                      \
                 size_t len,                                            \
                 ctl_hash_size_t i)                                     \
{                                                                       \
        struct _name ## _slink *p = NULL;                               \
        struct _name ## _kvp *kp = NULL;                                \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        for (p = hsp->arr[i]; p != NULL; p = p->next) {                 \
                kp = &p->kvp;                                           \
                if (hsp->cmp(kp->kv_key, kp->kv_len, key, len) == 0)    \
                        return p;                                       \
        }                                                               \
                                                                        \
        return NULL;                                                    \
}                                                                       \
                                                                        \
/**                                                                     \
 * resize table:                                                        \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name:                                             \
 *  @cap: new capacity                                                  \
 *                                                                      \
 * ret:                                                                 \
 *  @success: 0                                                         \
 *  @failure: -1 and errno set                                          \
 */                                                                     \
PRIVATE _link int                                                       \
_name ## _resize(struct _name *hsp, ctl_hash_size_t cap)                \
{                                                                       \
        struct _name ## _slink **arr = NULL;                            \
        struct _name ## _slink *next = NULL;                            \
        struct _name ## _slink *p = NULL;                               \
        ctl_hash_size_t seen = 0;                                       \
        ctl_hash_size_t i = 0;                                          \
        ctl_hash_size_t j = 0;                                          \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        arr = calloc(cap, sizeof(*arr));                                \
        if (arr == NULL)                                                \
                return -1;                                              \
                                                                        \
        CTL_HASH_SLIST_FOR_EACH_SAFE(hsp, i, p, next, seen) {           \
                j = p->hash & (cap - 1);                                \
                p->next = arr[j];                                       \
                arr[j] = p;                                             \
        }                                                               \
                                                                        \
        free(hsp->arr);                                                 \
        hsp->arr = arr;                                                 \
        hsp->cap = cap;                                                 \
        return 0;                                                       \
}                                                                       \
                                                                        \
/**                                                                     \
 * set or get entry:                                                    \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name:                                             \
 *  @key: key                                                           \
 *  @len: key length                                                    \
 *  @val: value                                                         \
 *                                                                      \
 * ret:                                                                 \
 *  @success: 0                                                         \
 *  @failure: NULL and errno set                                        \
 */                                                                     \
PUBLIC _link struct _name ## _kvp *                                     \
_name ## _set_or_get(struct _name *hsp,                                 \
                     _ktype key,                                        \
                     size_t len,                                        \
                     _vtype val)                                        \
{                                                                       \
        struct _name ## _slink *p = NULL;                               \
        ctl_hash_size_t hash = 0;                                       \
        ctl_hash_size_t i = 0;                                          \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        hash = _name ## _do_hash(hsp, key, len);                        \
        i = hash & (hsp->cap - 1);                                      \
                                                                        \
        p = _name ## _do_get(hsp, key, len, i);                         \
        if (p != NULL)                                                  \
                return &p->kvp;                                         \
                                                                        \
        if (hsp->len >= (hsp->cap - (hsp->cap >> 2))) {                 \
                if (_name ## _resize(hsp, hsp->cap << 1) < 0)           \
                        return NULL;                                    \
                i = hash & (hsp->cap - 1);                              \
        }                                                               \
                                                                        \
        p = calloc(1, sizeof(*p));                                      \
        if (p == NULL)                                                  \
                return NULL;                                            \
                                                                        \
        p->kvp.kv_key = key;                                            \
        p->kvp.kv_len = len;                                            \
        p->kvp.kv_val = val;                                            \
        p->hash = hash;                                                 \
        p->next = hsp->arr[i];                                          \
        hsp->arr[i] = p;                                                \
        hsp->len++;                                                     \
        return &p->kvp;                                                 \
}                                                                       \
                                                                        \
/**                                                                     \
 * search for entry:                                                    \
 *                                                                      \
 * args:                                                                \
 *  @hsp: pointer to _name                                              \
 *  @key: key                                                           \
 *  @len: key length                                                    \
 *                                                                      \
 * ret:                                                                 \
 *  @success: pointer to entry                                          \
 *  @failure: NULL                                                      \
 */                                                                     \
PUBLIC _link struct _name ## _kvp *                                     \
_name ## _get(struct _name *hsp,                                        \
              const _ktype key,                                         \
              size_t len)                                               \
{                                                                       \
        struct _name ## _slink *p = NULL;                               \
        ctl_hash_size_t hash = 0;                                       \
        ctl_hash_size_t i = 0;                                          \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        hash = _name ## _do_hash(hsp, key, len);                        \
        i = hash & (hsp->cap - 1);                                      \
                                                                        \
        p = _name ## _do_get(hsp, key, len, i);                         \
        if (p == NULL)                                                  \
                return NULL;                                            \
                                                                        \
        return &p->kvp;                                                 \
}                                                                       \
                                                                        \
/**                                                                     \
 * remove entry:                                                        \
 *                                                                      \
 * args:                                                                \
 *  @hsp:  pointer to _name                                             \
 *  @key:  key                                                          \
 *  @len:  key length                                                   \
 *  @dtor: optional destructor                                          \
 *                                                                      \
 * ret:                                                                 \
 *  @success: 0                                                         \
 *  @failure: -1 and errno set                                          \
 */                                                                     \
PUBLIC _link int                                                        \
_name ## _rm(struct _name *hsp,                                         \
             const _ktype key,                                          \
             size_t len,                                                \
             void (*dtor)(struct _name ## _kvp *))                      \
{                                                                       \
        struct _name ## _slink **p = NULL;                              \
        struct _name ## _kvp *kp = NULL;                                \
        struct _name ## _slink *tmp = NULL;                             \
        ctl_hash_size_t hash = 0;                                       \
        ctl_hash_size_t i = 0;                                          \
                                                                        \
        CTL_HASH_SLIST_OK(_name, hsp);                                  \
                                                                        \
        hash = _name ## _do_hash(hsp, key, len);                        \
        i = hash & (hsp->cap - 1);                                      \
                                                                        \
        for (p = &hsp->arr[i]; *p != NULL; p = &(*p)->next) {           \
                kp = &(*p)->kvp;                                        \
                if (hsp->cmp(kp->kv_key, kp->kv_len, key, len) == 0)    \
                        break;                                          \
        }                                                               \
                                                                        \
        if (*p == NULL)                                                 \
                return 0;                                               \
                                                                        \
        tmp = *p;                                                       \
        *p = tmp->next;                                                 \
        if (dtor != NULL)                                               \
                dtor(&tmp->kvp);                                        \
        free(tmp);                                                      \
        tmp = NULL;                                                     \
        hsp->len--;                                                     \
                                                                        \
        if (hsp->cap == CTL_HASH_INIT_CAP)                              \
                return 0;                                               \
                                                                        \
        if (hsp->len != (hsp->cap >> 1))                                \
                return 0;                                               \
                                                                        \
        return _name ## _resize(hsp, hsp->cap >> 1);                    \
}                                                                       \

/**
 * define a new dynamic hash map with singly linked chaining and pointer key:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type of array elements
 *  @_vtype: value type of array elements
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_PTR_KEY_DEF(_link, _ktype, _vtype, _name)        \
                                                                        \
CTL_HASH_SLIST_DECL(_ktype, _vtype, _name)                              \
CTL_HASH_SLIST_PTR_HASH_DEF(_link, _ktype, _name)                       \
CTL_DO_HASH_SLIST_DEF(_link, _ktype, _vtype, _name)

/**
 * define a new dynamic hash map with singly linked chaining:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type of array elements
 *  @_vtype: value type of array elements
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_NOT_PTR_KEY_DEF(_link, _ktype, _vtype, _name)    \
                                                                        \
CTL_HASH_SLIST_DECL(_ktype, _vtype, _name)                              \
CTL_HASH_SLIST_HASH_DEF(_link, _ktype, _name)                           \
CTL_DO_HASH_SLIST_DEF(_link, _ktype, _vtype, _name)

#endif /* #ifndef CTL_HASH_SLIST_H */
