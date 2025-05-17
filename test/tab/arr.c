ARR_TEST(char *, strvec, strcmp)
CMP_DEF(intvec, int)
ARR_TEST(int, intvec, intvec_cmp)
CMP_DEF(floatvec, float)
ARR_TEST(float, floatvec, floatvec_cmp)

void
test(void)
{
	strvec_test()
	intvec_test()
	floatvec_test()
}
