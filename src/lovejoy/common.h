#include <stdlib.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define V_MAJOR 0
#define V_MINOR 1
#define V_TINY  0

#define VERSION "v" STR(V_MAJOR) "." STR(V_MINOR) "." STR(V_TINY)

typedef   signed char i8;
typedef unsigned char u8;
typedef   signed short i16;
typedef unsigned short u16;
typedef   signed int i32;
typedef unsigned int u32;

#if (_MIPS_SZLONG == 64)
typedef   signed long i64;
typedef unsigned long u64;
#endif

