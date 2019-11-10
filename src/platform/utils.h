#ifndef GPROJ_UTILS_H_
#define GPROJ_UTILS_H_
#include <assert.h>
#include <stddef.h>


#define STATIC_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define INDEX_OF(array, ptr) (ptr - (&array[0]))
#define OFFSETOF(type, memb) offsetof(type, memb)
#define STATIC_ASSERT(ident, cond)  \
	struct static_assert_##ident {  \
	u8 fake_array[(cond) ? 1 : -1]; \
}



#ifdef _MSC_VER
// fucking MS please start supporting C99 properly!
#define restrict __restrict
#endif


#define MACR_STRINGFY(x) #x
#define INVALID_CODE_PATH assert(0 && "INVALID CODE PATH AT " __FILE__ ":" MACR_STRINGFY(__LINE__))


#endif
