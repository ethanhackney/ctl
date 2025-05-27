#include "tmp/map/hash/slist.h"
#include "lib/include/util.h"
#include <string.h>
#include <stdio.h>

CTL_HASH_SLIST_PTR_KEY_DEF(, char *, int, str2intmap)
CTL_HASH_SLIST_NOT_PTR_KEY_DEF(, int, int, int2intmap)

static int
str_cmp(const char *a, size_t alen, const char *b, size_t blen)
{
        size_t n = 0;

        n = MIN(alen, blen);
        return strncmp(a, b, n);
}

int
main(void)
{
        struct str2intmap map = {0};
        struct str2intmap_kvp *kp = NULL;
        char buf[BUFSIZ] = "";
        char *dup = NULL;

        if (str2intmap_init(NULL, 0, str_cmp) < 0)
                die("str2intmap_init");

        while (fgets(buf, BUFSIZ, stdin) != NULL) {
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

        str2intmap_free(&map, NULL);
}
