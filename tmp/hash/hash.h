#ifndef CTL_HASH_H
#define CTL_HASH_H

#include "../../lib/include/util.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
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
 * read block bit block:
 *
 * args:
 *  @src: source
 *
 * ret:
 *  @success: block as 64-bit
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_read_block(const uint8_t *src)
{
        ctl_hash_size_t dst = 0;

        memcpy(&dst, src, sizeof(dst));
        return dst;
}

/**
 * hash key 0:
 *
 * args:
 *  @k: key
 *  @h: hash
 *
 * ret:
 *  @success: hash of key
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_k0(ctl_hash_size_t k, ctl_hash_size_t h)
{
        k *= 0x87c37b91114253d5ULL;
        k = (k << 31) | (k >> (64 - 31));
        k *= 0x4cf5ad432745937fULL;
        h ^= k;
        return k;
}

/**
 * hash key 1:
 *
 * args:
 *  @k: key
 *  @h: hash
 *
 * ret:
 *  @success: hash of key
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_k1(ctl_hash_size_t k, ctl_hash_size_t h)
{
        k *= 0x4cf5ad432745937fULL;
        k = (k << 33) | (k >> (64 - 33));
        k *= 0x87c37b91114253d5ULL;
        h ^= k;
        return k;
}

/**
 * finalize hash:
 *
 * args:
 *  @h: hash
 *
 * ret:
 *  @success: finalized hash
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_final(ctl_hash_size_t h)
{
        h ^= h >> 33;
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 33;
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= h >> 33;
        return h;
}

/**
 * multiply by 5:
 *
 * args:
 *  @n: number
 *
 * ret:
 *  @success: n * 5
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash_mult_5(ctl_hash_size_t n)
{
        return (n << 2) + n;
}

/**
 * MurmurHash3 https://en.wikipedia.org/wiki/MurmurHash:
 *
 * args:
 *  @key:  pointer to key
 *  @len:  length of key
 *  @seed: seed
 *
 * ret:
 *  @success: hash of key
 *  @failure: does not
 */
static inline ctl_hash_size_t
ctl_hash(const void *key, ctl_hash_size_t len, ctl_hash_size_t seed)
{
        ctl_hash_size_t nblocks = 0;
        ctl_hash_size_t h0 = 0;
        ctl_hash_size_t h1 = 0;
        ctl_hash_size_t k0 = 0;
        ctl_hash_size_t k1 = 0;
        ctl_hash_size_t i = 0;
        size_t sz = 0;
        const uint8_t *blocks = NULL;
        const uint8_t *tail = NULL;

        dbug(key == NULL, "key == NULL");
        dbug(len == 0, "len == 0");
        dbug(seed == 0, "seed == 0");

        h0 = seed;
        h1 = seed;

        nblocks = len >> 4;
        blocks = (const uint8_t *)key;
        sz = sizeof(ctl_hash_size_t);

        for (i = 0; i < nblocks; i++) {
                k0 = ctl_hash_read_block(blocks + (sz * (i << 1)));
                k1 = ctl_hash_read_block(blocks + ((sz * (i << 1)) + 1));

                k0 = ctl_hash_k0(k0, h0);
                h0 = (h0 << 27) | (h0 >> (64 - 27));
                h0 += h1;
                h0 = ctl_hash_mult_5(h0) + 0x52dce729;

                k1 = ctl_hash_k1(k1, h1);
                h1 = (h1 << 31) | (h1 >> (64 - 31));
                h1 += h0;
                h1 = ctl_hash_mult_5(h1) + 0x38495ab5;
        }

        tail = (const uint8_t *)(blocks + (nblocks << 4));
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
                 k1 = ctl_hash_k1(k1, h1);
                 /* fallthrough */
        case 8: k0 ^= ((ctl_hash_size_t)tail[7]) << 56;
                 /* fallthrough */
        case 7: k0 ^= ((ctl_hash_size_t)tail[6]) << 48;
                 /* fallthrough */
        case 6: k0 ^= ((ctl_hash_size_t)tail[5]) << 40;
                 /* fallthrough */
        case 5: k0 ^= ((ctl_hash_size_t)tail[4]) << 32;
                 /* fallthrough */
        case 4: k0 ^= ((ctl_hash_size_t)tail[3]) << 24;
                 /* fallthrough */
        case 3: k0 ^= ((ctl_hash_size_t)tail[2]) << 16;
                 /* fallthrough */
        case 2: k0 ^= ((ctl_hash_size_t)tail[1]) <<  8;
                 /* fallthrough */
        case 1: k0 ^= ((ctl_hash_size_t)tail[0]) <<  0;
                 /* fallthrough */
                k0 = ctl_hash_k0(k0, h0);
        }

        h0 ^= len;
        h1 ^= len;

        h0 += h1;
        h1 += h0;

        h0 = ctl_hash_final(h0);
        h1 = ctl_hash_final(h1);

        return h0 + h1;
}
#else
static inline ctl_hash_size_t
ctl_hash(const void *key, ctl_hash_size_t len, ctl_hash_size_t seed)
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

        return h;
}
#endif /* #ifdef UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF */

#endif /* #ifndef CTL_HASH_H */
