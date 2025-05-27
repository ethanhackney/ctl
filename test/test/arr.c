#ifndef CTL_DBUG
#define CTL_DBUG
#endif /* #ifndef CTL_DBUG */

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
#define ARR_TEST(_type, _name, _cmp)                                            \
CTL_ARR_DEF(, _type, _name)                                                     \
/**                                                                             \
 * do test on _name:                                                            \
 *                                                                              \
 * args:                                                                        \
 *  @data: test data                                                            \
 *  @n:    number of elements in data                                           \
 *  @nil:  nil value (for testing failed searches)                              \
 *  @dtor: optional destructor                                                  \
 */                                                                             \
PUBLIC void                                                                     \
_name ## _do_test(_type *data,                                                  \
                  size_t n,                                                     \
                  _type nil,                                                    \
                  void (*dtor)(_type))                                          \
{                                                                               \
        struct _name arr = {0};                                                 \
        size_t idx = 0;                                                         \
        size_t i = 0;                                                           \
        _type rm[5];                                                            \
        _type *p = NULL;                                                        \
        _type v;                                                                \
        int ret = 0;                                                            \
                                                                                \
        if (_name ## _init(&arr, 0) < 0)                                        \
                die("%s_init", TO_STR(_name));                                  \
                                                                                \
        if (_name ## _addv(&arr, 0, data, n) < 0)                               \
                die("%s_addv", TO_STR(_name));                                  \
                                                                                \
        if (_name ## _len(&arr) != n)                                           \
                die("%s_addv did not update length");                           \
                                                                                \
        CTL_ARR_FOR_EACH(&arr, p) {                                             \
                i = _name ## _find(&arr, *p, _cmp);                             \
                if (i == _name ## _len(&arr))                                   \
                        die("%s_find", TO_STR(_name));                          \
        }                                                                       \
        if (_name ## _find(&arr, nil, _cmp) != _name ## _len(&arr))             \
                die("%s_find", TO_STR(_name));                                  \
                                                                                \
        if (_name ## _rmv(&arr, 0, rm, 5, dtor) < 0)                            \
                die("%s_rmv", TO_STR(_name));                                   \
                                                                                \
        if (_name ## _rmv(&arr, 0, NULL, _name ## _len(&arr), dtor) < 0)        \
                die("%s_rmv", TO_STR(_name));                                   \
                                                                                \
        if (_name ## _addv(&arr, 0, data, n) < 0)                               \
                die("%s_addv", TO_STR(_name));                                  \
                                                                                \
        _name ## _sort(&arr, _cmp);                                             \
        for (i = 0; i < _name ## _len(&arr) - 1; i++) {                         \
                if (_cmp(arr.arr[i], arr.arr[i + 1]) > 0)                       \
                        die("arr not sorted after %s_sort", TO_STR(_name));     \
        }                                                                       \
                                                                                \
        for (p = data; (size_t)p < (size_t)data + n; p++) {                     \
                if (_name ## _bin_add(&arr, *p, _cmp) < 0)                      \
                        die("%s_bin_add", TO_STR(_name));                       \
        }                                                                       \
                                                                                \
        for (i = 0; i < _name ## _len(&arr) - 1; i++) {                         \
                if (_cmp(arr.arr[i], arr.arr[i + 1]) > 0)                       \
                        die("arr not sorted after %s_bin_add", TO_STR(_name));  \
        }                                                                       \
                                                                                \
        CTL_ARR_FOR_EACH(&arr, p) {                                             \
                idx = _name ## _bin_find(&arr, *p, _cmp);                       \
                if (idx == _name ## _len(&arr))                                 \
                        die("%s_bin_find", TO_STR(_name));                      \
        }                                                                       \
        if (_name ## _bin_find(&arr, nil, _cmp) != _name ## _len(&arr))         \
                die("%s_bin_find", TO_STR(_name));                              \
                                                                                \
        for (;;) {                                                              \
                idx = (size_t)rand() % _name ## _len(&arr);                     \
                ret = _name ## _rm(&arr, idx, &v, dtor);                        \
                                                                                \
                if (ret < 0)                                                    \
                        die("%s_rm", TO_STR(_name));                            \
                else if (ret > 0)                                               \
                        break;                                                  \
                else if (_name ## _len(&arr) == 0)                              \
                        break;                                                  \
        }                                                                       \
                                                                                \
        _name ## _free(&arr, NULL);                                             \
}                                                                               \
                                                                                \
/**                                                                             \
 * test _name:                                                                  \
 *                                                                              \
 * args:                                                                        \
 *  @nil:  nil value                                                            \
 *  @n:    number of elements in data                                           \
 *  @dtor: optional destructor                                                  \
 */                                                                             \
PUBLIC void                                                                     \
_name ## _test(_type nil, size_t n, void (*dtor)(_type))                        \
{                                                                               \
        _type data[n];                                                          \
        size_t i = 0;                                                           \
                                                                                \
again:                                                                          \
        buf_rand(data, sizeof(data));                                           \
        for (i = 0; i < n; i++) {                                               \
                if (_cmp(data[i], nil) == 0)                                    \
                        goto again;                                             \
        }                                                                       \
        _name ## _do_test(data, n, nil, dtor);                                  \
}

#include "do/list/arr.c"

int
main(void)
{
        arr_test();
}
