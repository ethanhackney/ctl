/* AUTO-GENERATED! DO NOT MODIFY */
#include "/home/ethanhackney/code/ctl/lib/lib.h"
#include "/home/ethanhackney/code/ctl/tmp/arr.h"
CTL_ARR_TEST(uint64_t, u64_arr, u64_cmp)
CTL_ARR_TEST(uint32_t, u32_arr, u32_cmp)
CTL_ARR_TEST(uint16_t, u16_arr, u16_cmp)
CTL_ARR_TEST(uint8_t, u8_arr, u8_cmp)
CTL_ARR_TEST(int64_t, s64_arr, s64_cmp)
CTL_ARR_TEST(int32_t, s32_arr, s32_cmp)
CTL_ARR_TEST(int16_t, s16_arr, s16_cmp)
CTL_ARR_TEST(int8_t, s8_arr, s8_cmp)
CTL_ARR_TEST(double, double_arr, float_cmp)
CTL_ARR_TEST(float, float_arr, double_cmp)
CTL_ARR_TEST(char *, str_arr, strcmp)

void
arr_test(void)
{
	printf("CTL_ARR_DEF tests running...\n");
	u64_arr_test(NULL);
	printf("PASSED: u64_arr_test");
	u32_arr_test(NULL);
	printf("PASSED: u32_arr_test");
	u16_arr_test(NULL);
	printf("PASSED: u16_arr_test");
	u8_arr_test(NULL);
	printf("PASSED: u8_arr_test");
	s64_arr_test(NULL);
	printf("PASSED: s64_arr_test");
	s32_arr_test(NULL);
	printf("PASSED: s32_arr_test");
	s16_arr_test(NULL);
	printf("PASSED: s16_arr_test");
	s8_arr_test(NULL);
	printf("PASSED: s8_arr_test");
	double_arr_test(NULL);
	printf("PASSED: double_arr_test");
	float_arr_test(NULL);
	printf("PASSED: float_arr_test");
	str_arr_test(NULL);
	printf("PASSED: str_arr_test");
	printf("PASS\n");
}
