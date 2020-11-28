#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Misc macros */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* Version number */
#define V_MAJOR 0
#define V_MINOR 1
#define V_TINY  0

#define VERSION "v" STR(V_MAJOR) "." STR(V_MINOR) "." STR(V_TINY)

/* Syntax helpers */
#define newtype(NT, T) typedef struct __##NT { T value; } NT
#define record(S) typedef struct S S

/* Types */
newtype(atomic_t, int);

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

#ifdef __SIZEOF_INT128__
	typedef  __int128_t i128;
	typedef __uint128_t u128;
#endif

typedef ptrdiff_t isize;
typedef    size_t usize;

typedef  intptr_t iptr;
typedef uintptr_t uptr;

typedef  float f32;
typedef double f64;
