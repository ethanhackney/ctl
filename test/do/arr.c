/* AUTO-GENERATED! DO NOT MODIFY */
#include "/home/ethanhackney/code/ctl/lib/lib.h"
#include "/home/ethanhackney/code/ctl/tmp/arr.h"
#include <time.h>
#include <stdlib.h>
ARR_TEST(uint64_t, u64_arr, u64_cmp)
ARR_TEST(uint32_t, u32_arr, u32_cmp)
ARR_TEST(uint16_t, u16_arr, u16_cmp)
ARR_TEST(uint8_t, u8_arr, u8_cmp)
ARR_TEST(int64_t, s64_arr, s64_cmp)
ARR_TEST(int32_t, s32_arr, s32_cmp)
ARR_TEST(int16_t, s16_arr, s16_cmp)
ARR_TEST(int8_t, s8_arr, s8_cmp)

void
arr_test(void)
{
	srand(time(NULL));
	printf("CTL_ARR_DEF tests running...\n");
	u64_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: u64_arr_test\n");
	u32_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: u32_arr_test\n");
	u16_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: u16_arr_test\n");
	u8_arr_test(0, 128, NULL);
	printf("CTL_ARR_TEST_PASSED: u8_arr_test\n");
	s64_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: s64_arr_test\n");
	s32_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: s32_arr_test\n");
	s16_arr_test(0, 1024, NULL);
	printf("CTL_ARR_TEST_PASSED: s16_arr_test\n");
	s8_arr_test(0, 128, NULL);
	printf("CTL_ARR_TEST_PASSED: s8_arr_test\n");
	printf("CTL_ARR_TEST_PASSED\n");
}
