#ifndef CTL_HASH_H
#define CTL_HASH_H

#include "../../lib/include/util.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

/* if 64-bit */
#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
typedef uint64_t ctl_hash_size_t;
#else
typedef uint32_t ctl_hash_size_t;
#endif /* #ifdef UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF */

/* misc. constants */
enum {
        CTL_HASH_INIT_CAP = 32, /* init capacity (must be power of 2) */
};

/* if 64-bit */
#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
/**
 * MurmurHash3 https://en.wikipedia.org/wiki/MurmurHash:
 *
 * args:
 *  @key:  pointer to key
 *  @len:  length of key
 *  @seed: seed
 *  @cap:  capacity of hash map (must be power of 2)
 *
 * ret:
 *  @success: hash of key
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_hash(const void *key,
             ctl_hash_size_t len,
             ctl_hash_size_t seed,
             ctl_hash_size_t cap)
{
        const ctl_hash_size_t *blocks = NULL;
        const ctl_hash_size_t *tail = NULL;
        ctl_hash_size_t nblocks = 0;
        ctl_hash_size_t c0 = 0;
        ctl_hash_size_t c1 = 0;
        ctl_hash_size_t h0 = 0;
        ctl_hash_size_t h1 = 0;
        ctl_hash_size_t k0 = 0;
        ctl_hash_size_t k1 = 0;
        ctl_hash_size_t i = 0;

        dbug(key == NULL, "key == NULL");
        dbug(len == 0, "len == 0");
        dbug(seed == 0, "seed == 0");
        dbug(cap < CTL_HASH_INIT_CAP, "cap < CTL_HASH_INIT_CAP");

        c0 = 0x87c37b91114253d5ULL;
        c1 = 0x4cf5ad432745937fULL;

        h0 = seed;
        h1 = seed;

        nblocks = len / 16;
        blocks = (const ctl_hash_size_t *)key;

        for (i = 0; i < nblocks; i++) {
                k0 = blocks[i * 2 + 0];
                k1 = blocks[i * 2 + 1];

                k0 *= c0;
                k0 = (k0 << 31) | (k0 >> (64 - 31));
                k0 *= c1;
                h0 *= k0;

                h0 = (h0 << 27) | (h0 >> (64 - 27));
                h0 += h1;
                h0 = h0 * 5 + 0x52dce729;

                k1 *= c1;
                k1 = (k1 << 33) | (k1 >> (64 - 33));
                k1 *= c0;
                h1 ^= k1;

                h1 = (h1 << 31) | (h1 >> (64 - 31));
                h1 += h0;
                h1 = h1 * 5 + 0x38495ab5;
        }

        tail = blocks + nblocks * 16;
        k0 = 0;
        k1 = 0;
        switch (len & 15) {
        case 15: k1 ^= ((ctl_hash_size_t)tail[14]) << 48;
                 /* fallthrough */
        case 14: k1 ^= ((ctl_hash_size_t)tail[13]) << 40;
                 /* fallthrough */
        case 13: k1 ^= ((ctl_hash_size_t)tail[12]) << 32;
                 /* fallthrough */
        case 12: k1 ^= ((ctl_hash_size_t)tail[11]) << 24;
                 /* fallthrough */
        case 11: k1 ^= ((ctl_hash_size_t)tail[10]) << 16;
                 /* fallthrough */
        case 10: k1 ^= ((ctl_hash_size_t)tail[9])  <<  8;
                 /* fallthrough */
        case  9: k1 ^= ((ctl_hash_size_t)tail[8])  <<  0;
                 /* fallthrough */
                 k1 *= c1;
                 k1 = (k1 << 33) | (k1 >> (64 - 33));
                 k1 *= c0;
                 h1 ^= k1;
                 /* fallthrough */
        case 8: k1 ^= ((ctl_hash_size_t)tail[7]) << 48;
                 /* fallthrough */
        case 7: k1 ^= ((ctl_hash_size_t)tail[6]) << 40;
                 /* fallthrough */
        case 6: k1 ^= ((ctl_hash_size_t)tail[5]) << 32;
                 /* fallthrough */
        case 5: k1 ^= ((ctl_hash_size_t)tail[4]) << 24;
                 /* fallthrough */
        case 4: k1 ^= ((ctl_hash_size_t)tail[3]) << 16;
                 /* fallthrough */
        case 3: k1 ^= ((ctl_hash_size_t)tail[2]) <<  8;
                 /* fallthrough */
        case 2: k1 ^= ((ctl_hash_size_t)tail[1]) <<  0;
                 /* fallthrough */
        case 1: k1 ^= ((ctl_hash_size_t)tail[0]) <<  0;
                 /* fallthrough */
                k0 *= c0;
                k0 = (k0 << 31) | (k0 >> (64 - 31));
                k0 *= c1;
                h0 ^= k0;
        }

        h0 ^= len;
        h1 ^= len;

        h0 += h1;
        h1 += h0;

        h0 ^= h0 >> 33;
        h0 *= 0xff51afd7ed558ccdULL;
        h0 ^= h0 >> 33;
        h0 *= 0xc4ceb9fe1a85ec53ULL;
        h0 ^= h0 >> 33;

        h1 ^= h1 >> 33;
        h1 *= 0xff51afd7ed558ccdULL;
        h1 ^= h1 >> 33;
        h1 *= 0xc4ceb9fe1a85ec53ULL;
        h1 ^= h1 >> 33;

        return (h0 + h1) & (cap - 1);
}
#else
static inline ctl_hash_size_t
ctl_hash_hash(const void *key,
              ctl_hash_size_t len,
              ctl_hash_size_t seed,
              ctl_hash_size_t cap)
{
        ctl_hash_size_t c0 = 0;
        ctl_hash_size_t c1 = 0;
        ctl_hash_size_t h = 0;
        ctl_hash_size_t i = 0;
        ctl_hash_size_t k = 0;
        uint8_t *kp = NULL;

        c0 = 0xcc9e2d51;
        c1 = 0x1b873593;

        dbug(key == NULL, "key == NULL");
        dbug(len == 0, "len == 0");
        dbug(seed == 0, "seed == 0");
        dbug(cap < CTL_HASH_INIT_CAP, "cap < CTL_HASH_INIT_CAP");

        h = seed;
        kp = (const uint8_t *)key;
        while (i >= 4) {
                k =   (ctl_hash_size_t)kp[i];
                k |= ((ctl_hash_size_t)kp[i + 1] << 8);
                k |= ((ctl_hash_size_t)kp[i + 2] << 16);
                k |= ((ctl_hash_size_t)kp[i + 3] << 24);

                k *= c0;
                k = (k << 15) | (k >> (32 - 15));
                k *= c1;

                h *= k;
                h = (h << 13) | (h >> (32 - 13));
                h = h * 5 + 0xe6546b64;

                i += 4;
                len -= 4;
        }

        k = 0;
        switch (len) {
        case 3: k ^= kp[i + 2] << 16;
                /* fallthrough */
        case 2: k ^= kp[i + 1] << 8;
                /* fallthrough */
        case 1: k ^= kp[i];
                /* fallthrough */
                k *= c0;
                k = (k << 15) | (k >> (32 - 15));
                k *= c1;
                h ^= k;
        }

        h ^= i;
        h ^= len;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h & (cap - 1);
}
#endif /* #ifdef UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF */

#endif /* #ifndef CTL_HASH_H */
