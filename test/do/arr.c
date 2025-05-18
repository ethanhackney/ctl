/* AUTO-GENERATED! DO NOT MODIFY */
CTL_CMP_DEF(u64arr, uint64_t)
CTL_ARR_TEST(uint64_t, u64arr, u64arr_cmp)
CTL_CMP_DEF(u32arr, uint32_t)
CTL_ARR_TEST(uint32_t, u32arr, u32arr_cmp)
CTL_CMP_DEF(u16arr, uint16_t)
CTL_ARR_TEST(uint16_t, u16arr, u16arr_cmp)
CTL_CMP_DEF(u8arr, uint8_t)
CTL_ARR_TEST(uint8_t, u8arr, u8arr_cmp)
CTL_CMP_DEF(s64arr, int64_t)
CTL_ARR_TEST(int64_t, s64arr, s64arr_cmp)
CTL_CMP_DEF(s32arr, int32_t)
CTL_ARR_TEST(int32_t, s32arr, s32arr_cmp)
CTL_CMP_DEF(s16arr, int16_t)
CTL_ARR_TEST(int16_t, s16arr, s16arr_cmp)
CTL_CMP_DEF(s8arr, int8_t)
CTL_ARR_TEST(int8_t, s8arr, s8arr_cmp)
CTL_CMP_DEF(doublearr, double)
CTL_ARR_TEST(double, doublearr, doublearr_cmp)
CTL_CMP_DEF(floatarr, float)
CTL_ARR_TEST(float, floatarr, floatarr_cmp)
CTL_ARR_TEST(char *, strarr, strcmp)

void
test(void)
{
	printf("CTL_ARR_DEF tests running...\n");
	u64arr_test(NULL);
	printf("PASSED: _test");
	u32arr_test(NULL);
	printf("PASSED: _test");
	u16arr_test(NULL);
	printf("PASSED: _test");
	u8arr_test(NULL);
	printf("PASSED: _test");
	s64arr_test(NULL);
	printf("PASSED: _test");
	s32arr_test(NULL);
	printf("PASSED: _test");
	s16arr_test(NULL);
	printf("PASSED: _test");
	s8arr_test(NULL);
	printf("PASSED: _test");
	doublearr_test(NULL);
	printf("PASSED: _test");
	floatarr_test(NULL);
	printf("PASSED: _test");
	strarr_test(NULL);
	printf("PASSED: _test");
	printf("PASS\n");
}
