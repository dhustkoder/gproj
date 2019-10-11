#ifndef GPROJ_UTILS_H_
#define GPROJ_UTILS_H_

#define STATIC_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define STATIC_ASSERT(ident, cond) \
	struct static_assert_##ident {\
	u8 fake_array[(cond) ? 1 : -1]; \
}

#endif

