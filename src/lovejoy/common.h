//! @file common.h
//! Defines basic macros and datatypes which are in
//! common through-out the whole project.

#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <stdlib.h>
#include <stdio.h>
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
#define V_PATCH 0

#define VERSION "v" STR(V_MAJOR) "." STR(V_MINOR) "." STR(V_PATCH)

/* Syntax helpers */
#define loop while (1)
#define unless(cond) if (!(cond))
#define until(cond) while (!(cond))
#define newtype(NT, T) typedef struct _##NT { const T value; } NT
#define newarray(NT, T) typedef struct _##NT { \
	const usize len; \
	T *const value; \
} NT
#define unqualify(D, T) typedef D T T
#define nil NULL

#define UNUSED(x) (void)(x)

#define print(...) printf(__VA_ARGS__)
#define println(lit, ...) printf(lit "\n", __VA_ARGS__)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define eprint(...) eprintf(__VA_ARGS__)
#define eprintln(lit, ...) fprintf(stderr, lit "\n", __VA_ARGS__)

/* Types */
/// Useful for resource counting etc.
newtype(atomic_t, int);

/// The type that occupies no space.
/// Thanks to Terry for this one.
typedef void u0;
#define UNIT ;

#define __UCHAR8__ char

#if (CHAR_BIT == 8)
	typedef   signed char i8;
	typedef unsigned char u8;

	#if (CHAR_MIN < 0)
		#undef  __UCHAR8__
		#define __UCHAR8__ unsigned char
	#endif
#else
	typedef  __int8_t i8;
	typedef __uint8_t u8;

	#undef  __UCHAR8__
	#define __UCHAR8__ __uint8_t;
#endif

#ifndef IMPLEMENTATION
	typedef __UCHAR8__ byte;
#else
	typedef __uint8_t byte;  ///< Don't use `char' when you want `byte'.
#endif

typedef unsigned char umin;
typedef   signed char imin;

typedef  __int16_t i16;
typedef __uint16_t u16;

typedef  __int32_t i32;
typedef __uint32_t u32;

/// Unicode codepoint (32 bits), don't use `char[4]',
/// and definitely do not use `wchar_t'.
typedef u32 rune;

#if (__LONG_WIDTH__ == 64)
	typedef   signed long i64;
	typedef unsigned long u64;
#elif (__LONG_LONG_WIDTH__ == 64)
	typedef   signed long long i64;
	typedef unsigned long long u64;
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

typedef  intmax_t imax;
typedef uintmax_t umax;

#ifdef __STDC_IEC_559__
	typedef  float f32;
	typedef double f64;
#endif

#define _LDOUBLE_BIT (__SIZEOF_LONG_DOUBLE__ * CHAR_BIT)

#if (_LDOUBLE_BIT == 80)
	typedef long double f80;
#elif (_LDOUBLE_BIT == 128)
	typedef long double f128;
#endif

/// Immutable wrapper for UTF-8 encoded string (bytes are mutable).
newarray(string, byte);
/// Imutable warpper for UCS-4/UTF-32 encoded runic string (runes are mutable).
newarray(runic, rune);

/* Common Functions */
extern bool is_zero(imax);
extern bool is_zerof(f64);
extern bool is_zeroed(imax *, usize);
extern u0 zero(u0 *, usize);
extern u0 *emalloc(usize, usize);
extern i32 eputs(const byte *);

/* Common Macros */

/// Unwraps pointer/value in sizing wrapper struct.
#define UNWRAP(STRUCTURE) (STRUCTURE).value
/// Initialise sizing wrapper with literal.
#define SIZED(TYPE, LITERAL) ((TYPE){ \
	.len = sizeof(LITERAL), \
	.value = (LITERAL) \
})

/// Empyt array of type.
#define EMPTY(TYPE) ((TYPE){ .len = 0, .value = NULL })
/// Is array empty?
#define IS_EMPTY(ARR) ((ARR).len == 0)

/// Heap allocates a constant sized type.
#define MAKE(TYPE, LEN) ((TYPE){ \
	.len = (LEN), \
	.value = emalloc(LEN, sizeof(TYPE)) \
})

/// Take a slice/substring/view of sized type.
#define SLICE(OBJ, START, END) ((typeof(OBJ)){ \
	.len = (((isize)(END) < 0) ? (OBJ).len : 0) + (END) - (START), \
	.value = (OBJ).value + (START) \
})
/* NOTES: */
/*
 * Read this: https://www.cprogramming.com/tutorial/unicode.html
 * by Jeff Bezanson, about modern unicode in C.
 *
 */

#endif
