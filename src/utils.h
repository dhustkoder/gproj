#ifndef GPROJ_UTILS_H_
#define GPROJ_UTILS_H_

#define STATIC_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define STATIC_ASSERT(ident, cond)  \
	struct static_assert_##ident {  \
	u8 fake_array[(cond) ? 1 : -1]; \
}



#ifdef _MSC_VER
#include <windows.h>
// fucking MS please start supporting C99 properly!
#define restrict __restrict
#endif


#endif

