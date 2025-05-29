/* AUTO-GENERATED! DO NOT MODIFY */

#include "/home/ethanhackney/code/ctl/lib/include/util.h"
#include "/home/ethanhackney/code/ctl/tmp/map/hash/slist.h"
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

HASH_SLIST_TEST(uint64_t, uint64_t, u64_2_u64_map, u64_cmp)

void
hash_slist_test(void)
{
	srand((unsigned int)time(NULL));
	printf("CTL_HASH_SLIST_DEF tests running...\n");

	u64_2_u64_map_test(1024, NULL);
	printf("CTL_HASH_SLIST_TEST_PASSED: u64_2_u64_map_test\n");

}
