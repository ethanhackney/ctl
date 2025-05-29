/* AUTO-GENERATED! DO NOT MODIFY */

#include "/home/ethanhackney/code/ctl/lib/include/util.h"
#include "/home/ethanhackney/code/ctl/tmp/list/arr.h"
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
	srand((unsigned int)time(NULL));
	printf("CTL_ARR_DEF tests running...\n");

	u64_arr_test((uint64_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: u64_arr_test\n");

	u32_arr_test((uint32_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: u32_arr_test\n");

	u16_arr_test((uint16_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: u16_arr_test\n");

	u8_arr_test((uint8_t)0, 128, NULL);
	printf("CTL_ARR_TEST_PASSED: u8_arr_test\n");

	s64_arr_test((int64_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: s64_arr_test\n");

	s32_arr_test((int32_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: s32_arr_test\n");

	s16_arr_test((int16_t)0, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: s16_arr_test\n");

	s8_arr_test((int8_t)0, 128, NULL);
	printf("CTL_ARR_TEST_PASSED: s8_arr_test\n");

	arr_test_struct_arr_test((struct arr_test_struct){0}, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: arr_test_struct_arr_test\n");

	arr_test_union_arr_test((union arr_test_union){0}, 4096, NULL);
	printf("CTL_ARR_TEST_PASSED: arr_test_union_arr_test\n");

}
