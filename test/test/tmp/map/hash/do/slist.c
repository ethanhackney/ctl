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

HASH_SLIST_TEST(uint64_t, uint64_t, u64_2_u64_map, u64_key_cmp)
HASH_SLIST_TEST(uint64_t, uint32_t, u64_2_u32_map, u64_key_cmp)
HASH_SLIST_TEST(uint64_t, uint16_t, u64_2_u16_map, u64_key_cmp)
HASH_SLIST_TEST(uint64_t, uint8_t, u64_2_u8_map, u64_key_cmp)
HASH_SLIST_TEST(int64_t, int64_t, s64_2_s64_map, s64_key_cmp)
HASH_SLIST_TEST(int64_t, int32_t, s64_2_s32_map, s64_key_cmp)
HASH_SLIST_TEST(int64_t, int16_t, s64_2_s16_map, s64_key_cmp)
HASH_SLIST_TEST(int64_t, int8_t, s64_2_s8_map, s64_key_cmp)

void
hash_slist_test(void)
{
	srand((unsigned int)time(NULL));
	u64_2_u64_map_test(1024, NULL);

	u64_2_u32_map_test(1024, NULL);

	u64_2_u16_map_test(1024, NULL);

	u64_2_u8_map_test(1024, NULL);

	s64_2_s64_map_test(1024, NULL);

	s64_2_s32_map_test(1024, NULL);

	s64_2_s16_map_test(1024, NULL);

	s64_2_s8_map_test(1024, NULL);

}
