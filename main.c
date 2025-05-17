#include "lib.h"
#include "arr.h"
#include <time.h>

/**
 * compare arguments:
 *
 * args:
 *  @app: pointer to first string
 *  @bpp: pointer to second string
 *
 * ret:
 *  < 0 if *app < *bpp
 *  = 0 if *app = *bpp
 *  > 0 if *app > *bpp
 */
static int
arg_cmp(const void *app, const void *bpp)
{
        const char *a = *(const char **)app;
        const char *b = *(const char **)bpp;

        return strcmp(a, b);
}

/* dynamic array of strings */
ARR_DEF(, char *, strvec)

int
main(int argc, char **argv)
{
        struct strvec args = {0};
        size_t idx = 0;
        char **p = NULL;
        char *v = NULL;
        int ret = 0;

        (void)arg_cmp;

        srand(time(NULL));

        if (strvec_init(&args, 0, 1, "ethan") < 0)
                die("main: strvec_init");

        if (strvec_addv(&args, 0, argv, argc) < 0)
                die("main: strvec_addv");

        if (strvec_rmv(&args, 0, NULL, argc) < 0)
                die("main: strvec_rmv");

        for (p = argv; *p != NULL; p++) {
                idx = rand() % args.len;
                if (strvec_add(&args, idx, *p) < 0)
                        die("main: strvec_add");
        }

        for (;;) {
                idx = rand() % args.len;
                ret = strvec_rm(&args, idx, &v);

                if (ret < 0)
                        die("main: strvec_rm");
                if (ret > 0)
                        break;

                puts(v);
                if (args.len == 0)
                        break;
        }

        strvec_free(&args, NULL);
}
