#include "lib.h"
#include "arr.h"
#include <time.h>

/**
 * compare arguments:
 *
 * args:
 *  @app: pointer to first string
 *  @bpp: pointer to second string
 */
static int
arg_cmp(const void *app, const void *bpp)
{
        const char *a = *(const char **)app;
        const char *b = *(const char **)bpp;

        return strcmp(a, b);
}

ARR_DEF(, char *, strvec)
ARR_DEF(, int, intvec)

int
main(int argc, char **argv)
{
        struct strvec args = {0};
        size_t idx = 0;
        size_t i = 0;
        char **p = NULL;
        char *v = NULL;
        int ret = 0;

        (void)arg_cmp;

        srand(time(NULL));

        if (strvec_init(&args, 0, 1, "ethan") < 0)
                die("main: strvec_init");

        if (strvec_addv(&args, 0, argv, argc) < 0)
                die("main: strvec_addv");

        for (i = 0; i < args.len; i++) {
                if (strvec_find(&args, args.arr[i], strcmp) == args.len)
                        die("main: strvec_find");
        }
        if (strvec_find(&args, "1", strcmp) != args.len)
                die("main: strvec_find");

        strvec_clear(&args);

        for (p = argv; *p != NULL; p++) {
                if (strvec_bin_add(&args, *p, strcmp) < 0)
                        die("main: strvec_bin_add");
        }

        for (i = 0; i < args.len - 1; i++) {
                if (strcmp(args.arr[i], args.arr[i + 1]) > 0)
                        die("main: args not sorted");
        }

        for (i = 0; i < args.len; i++) {
                if (strvec_bin_find(&args, args.arr[i], strcmp) == args.len)
                        die("main: strvec_bin_find");
        }
        if (strvec_bin_find(&args, "1", strcmp) != args.len)
                die("main: strvec_bin_find");

        for (;;) {
                if (args.len == 0)
                        break;

                idx = rand() % args.len;
                ret = strvec_rm(&args, idx, &v);

                if (ret < 0)
                        die("main: strvec_rm");
                if (ret > 0)
                        break;
        }

        strvec_free(&args, NULL);
}
