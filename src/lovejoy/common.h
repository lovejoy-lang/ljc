//! @file common.h
//! Defines basic macros and datatypes which are in
//! common through-out the whole project.

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

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
/// Useful for resource counting etc.
newtype(atomic_t, int);

#define __uchar8 char

#if (CHAR_BIT == 8)
	typedef   signed char i8;
	typedef unsigned char u8;

	#if (CHAR_MIN < 0)
		#undef  __uchar8
		#define __uchar8 unsigned char
	#endif
#else
	typedef  __int8_t i8;
	typedef __uint8_t u8;

	#undef  __uchar8
	#define __uchar8 __uint8_t;
#endif

/// Don't use `char' when you want `byte'.
typedef __uchar8 byte;

typedef  __int16_t i16;
typedef __uint16_t u16;

typedef  __int32_t i32;
typedef __uint32_t u32;

/// Unicode codepoint (32 bits), don't use `char',
/// and definitely do not use `wchar_t'.
typedef u32 rune;

#if (_MIPS_SZLONG == 64)
	typedef   signed long i64;
	typedef unsigned long u64;
#else
	typedef  __int64_t i64;
	typedef __uint64_t u64;
#endif

#ifdef __SIZEOF_INT128__
	typedef  __int128_t i128;
	typedef __uint128_t u128;
#endif

typedef ptrdiff_t isize;
typedef    size_t usize; ///< Use for storing array indices or object sizes.

typedef  intptr_t iptr;
typedef uintptr_t uptr; ///< Large enough to store a pointer, like (void *).

typedef  float f32;
typedef double f64;


/* NOTES: */
/*
 * Read this: https://www.cprogramming.com/tutorial/unicode.html
 * by Jeff Bezanson, about modern unicode in C.
 *
 */
