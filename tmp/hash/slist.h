#ifndef CTL_HASH_SLIST_H
#define CTL_HASH_SLIST_H

#include "hash.h"

/**
 * define a new dynamic hash map with singly linked chaining:
 *
 * args:
 *  @_link:  linkage of generated functions
 *  @_ktype: key type of array elements
 *  @_vtype: value type of array elements
 *  @_name:  name of generated struct and prefix of function names
 */
#define CTL_HASH_SLIST_DEF(_link, _ktype, _vtype, _name)                \
                                                                        \
/* key-value pair */                                                    \
struct _name ## _kvp {                                                  \
        ctl_map_size_t kv_klen; /* public: key length */                \
        _ktype         kv_key;  /* public: key */                       \
        _vtype         kv_val;  /* public: value */                     \
};                                                                      \
                                                                        \
/* hash link */                                                         \
struct _name ## _slink {                                                \
        struct _name ## _slink *next; /* private: next on list */       \
        struct _name ## _kvp    kvp;  /* private: key-value pair */     \
        ctl_map_size_t          hash; /* private: saved hash */         \
};                                                                      \

/* hash map */
struct _name {
};

#endif /* #ifndef CTL_HASH_SLIST_H */
