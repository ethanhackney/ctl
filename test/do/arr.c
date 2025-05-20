/* AUTO-GENERATED! DO NOT MODIFY */

#include "/home/ethanhackney/code/ctl/lib/include/util.h"
#include "/home/ethanhackney/code/ctl/tmp/arr.h"
#include <time.h>
#include <stdlib.h>

struct arr_test_struct {
	int a;
	int b;
	int c;
};

int
arr_test_struct_cmp(const struct arr_test_struct a, const struct arr_test_struct b)
{
	return s64_cmp(a.a, b.a);
}

union arr_test_union {
	int a;
	float b;
	long c;
};

int
arr_test_union_cmp(const union arr_test_union a, const union arr_test_union b)
{
	return s64_cmp(a.a, b.a);
}

ARR_TEST(uint64_t, u64_arr, u64_cmp)
ARR_TEST(uint32_t, u32_arr, u32_cmp)
ARR_TEST(uint16_t, u16_arr, u16_cmp)
ARR_TEST(uint8_t, u8_arr, u8_cmp)
ARR_TEST(int64_t, s64_arr, s64_cmp)
ARR_TEST(int32_t, s32_arr, s32_cmp)
ARR_TEST(int16_t, s16_arr, s16_cmp)
ARR_TEST(int8_t, s8_arr, s8_cmp)
ARR_TEST(struct arr_test_struct, arr_test_struct_arr, arr_test_struct_cmp)
ARR_TEST(union arr_test_union, arr_test_union_arr, arr_test_union_cmp)

void
arr_test(void)
{
	srand(time(NULL));
	u64_arr_test((uint64_t)0, 1024, NULL);

	u32_arr_test((uint32_t)0, 1024, NULL);

	u16_arr_test((uint16_t)0, 1024, NULL);

	u8_arr_test((uint8_t)0, 128, NULL);

	s64_arr_test((int64_t)0, 1024, NULL);

	s32_arr_test((int32_t)0, 1024, NULL);

	s16_arr_test((int16_t)0, 1024, NULL);

	s8_arr_test((int8_t)0, 128, NULL);

	arr_test_struct_arr_test((struct arr_test_struct){0}, 1024, NULL);

	arr_test_union_arr_test((union arr_test_union){0}, 1024, NULL);

	printf("CTL_ARR_TEST_PASSED\n");
}
