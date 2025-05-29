#include "../../../../../tmp/map/hash/slist.h"
#include "../../../../../lib/include/util.h"
#include <string.h>
#include <stdio.h>

/**
 * define a new array test:
 *
 * args:
 *  @_type:  type of elements
 *  @_name:  struct name
 *  @_never: value that cannot exist in array
 *  @_cmp:   comparison function
 *   ret:
 *    < 0 if arg0 < arg1
 *    = 0 if arg0 = arg1
 *    > 0 if arg0 > arg1
 */
#define HASH_SLIST_TEST(_ktype, _vtype, _name, _cmp)    \
                                                        \
CTL_HASH_SLIST_NOT_PTR_KEY_DEF(, _ktype, _vtype, _name) \
                                                        \
static void                                             \
_name ## _test(size_t size,                             \
               void (*dtor)(struct _name ## _kvp *))    \
{                                                       \
}

#include "do/slist.c"

static int
str_cmp(const char *a, size_t alen, const char *b, size_t blen)
{
        size_t n = 0;

        n = MIN(alen, blen);
        return strncmp(a, b, n);
}

CTL_HASH_SLIST_PTR_KEY_DEF(, char *, int, str2intmap)

int
main(int argc, char **argv)
{
        struct str2intmap map = {0};
        struct str2intmap_kvp *kp = NULL;
        char buf[BUFSIZ] = "";
        char *dup = NULL;
        FILE *fp = NULL;

        hash_slist_test();

        fp = fopen("slist.c", "r");
        if (fp == NULL)
                die("fopen");

        if (str2intmap_init(&map, 0, str_cmp) < 0)
                die("str2intmap_init");

        while (fgets(buf, BUFSIZ, fp) != NULL) {
                dup = strdup(buf);
                if (dup == NULL)
                        die("strdup");

                kp = str2intmap_set_or_get(&map, dup, strlen(dup), 1);
                if (kp == NULL)
                        die("str2intmap_set_or_get");

                kp = str2intmap_get(&map, dup, strlen(dup));
                if (kp == NULL)
                        die("str2intmap_get");
                if (kp->kv_val != 1)
                        die("str2intmap_get");

                if (str2intmap_rm(&map, dup, strlen(dup), NULL) < 0)
                        die("str2intmap_rm");
                kp = str2intmap_get(&map, dup, strlen(dup));
                if (kp != NULL)
                        die("str2intmap_rm did not remove");

                free(dup);
                dup = NULL;
        }
        if (ferror(fp))
                die("fgets");

        if (argc == 1)
                printf("CTL_HASH_SLIST_PASSED: str2intmap_test\n");

        printf("CTL_HASH_SLIST_PASSED: strarr_test\n");
        str2intmap_free(&map, NULL);
        fclose(fp);
}
