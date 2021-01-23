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
#include <string.h>
#include <assert.h>

/* Misc macros */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* Version number */
#define V_MAJOR 0
#define V_MINOR 1
#define V_PATCH 0

#define VERSION "v" STR(V_MAJOR) "." STR(V_MINOR) "." STR(V_PATCH)

#define REALLOC_FACTOR 1.5

/* Syntax helpers */
#define loop while (1)
#define unless(cond) if (!(cond))
#define until(cond) while (!(cond))
#define newtype(NT, T) typedef struct _##NT { T value; } NT
#define newarray(NT, T) typedef struct _##NT { \
	usize len; \
	usize cap; \
	T (*value);  \
} NT
#define newslice(NT, T) typedef struct _##NT { \
	usize len; \
	T (*value); \
} NT
#define unqualify(D, T) typedef D T T
#define nil NULL

#define UNUSED(x) (void)(x)

#define print(...) printf(__VA_ARGS__)
#define println(lit, ...) printf(lit "\n", ## __VA_ARGS__)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define eprint(...) eprintf(__VA_ARGS__)
#define eprintln(lit, ...) fprintf(stderr, lit "\n", ## __VA_ARGS__)

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

/// Such that `sizeof(umin) == 1`.
typedef unsigned char umin;
/// Such that `sizeof(imin) == 1`.
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
newslice(string, byte);
/// Imutable warpper for UCS-4/UTF-32 encoded runic string (runes are mutable).
newslice(runic, rune);

/* Common Functions */
extern bool is_zero(imax);
extern bool is_zerof(f64);
extern bool is_zeroed(imax *, usize);
/// Zero a block of memory.
/// @param[out] blk Pointer to start of block.
/// @param[in] width How many bytes to zero.
/// e.g., for an array `width = lenght * sizeof(elem)`.
extern u0 zero(u0 *blk, usize width);
extern u0 *emalloc(usize, usize);
/// Push element to array.
/// @param[in,out] arr Pointer to the dynamic array, cast to (u0 *).
/// @param[in] element Pointer to element to be pushed,  cast to (u0 *).
/// @param[in] width The `sizeof(T)` where `T` is the type of the element
///                  that is being pushed.
/// @returns How much capacity increased.
extern usize push(u0 *arr, const u0 *element, usize width);
/// Pops/removes element from top of the stack.
/// @returns Pointer to popped element.
extern u0 *pop(u0 *array, usize width);
extern i32 eputs(const byte *);
/// Compare two strings for equality.
extern bool string_eq(string, string);

/* Common Macros */

/// Unwraps pointer/value in sizing wrapper struct.
#define UNWRAP(STRUCTURE) (STRUCTURE).value
/// Initialise sizing wrapper with literal.
#define INIT(TYPE, ...) { \
	.len = sizeof((TYPE[])__VA_ARGS__), \
	.value = (TYPE[])__VA_ARGS__ \
}
/// Initialise sizing wrapper with of string literal.
#define STRING(...) { \
	.len = sizeof((byte[]){ __VA_ARGS__ }) - 1, \
	.value = (byte[]){ __VA_ARGS__ } \
}

/// Empty slice of certain type.
#define SEMPTY(TYPE) ((TYPE){ .len = 0, .value = NULL })
/// Empty array of certain type.
#define AEMPTY(TYPE) ((TYPE){ .len = 0, .cap = 0, .value = NULL })

/// Is array empty?
#define IS_EMPTY(ARR) ((ARR).len == 0)

/// Heap allocates a variable sized array.
#define AMAKE(TYPE, CAP) ((TYPE){ \
	.len = 0, \
	.cap = (CAP), \
	.value = emalloc((CAP), sizeof(TYPE)) \
})

/// Heap allocates a constant sized slice type.
#define SMAKE(TYPE, LEN) { \
	.len = (LEN), \
	.value = emalloc((LEN), sizeof(TYPE)) \
}

/// Take a slice/substring/view of sized type.
#define SLICE(OBJ, START, END) ((typeof(OBJ)){ \
	.len = (((isize)(END) < 0) ? (OBJ).len : 0) + (END) - (START), \
	.value = (OBJ).value + (START) \
})

/// Works like `SLICE`, but on a pointer instead of an array.
#define VIEW(PTR, START, END) { \
	.len = (END) - (START), \
	.value = (PTR) + (START) \
}

/// For-each loop, iterates across an array or slice.
/// For example:
/// ```c
/// newarray(IntArray, int);
/// IntArray xs = AMAKE(IntArray, 5);
///
/// int elem1 = 5;
/// int elem2 = 3;
/// push(&xs, &elem1, sizeof(int));
/// push(&xs, &elem2, sizeof(int));
///
/// FOR_EACH(x, xs) {
///     printf("%d\n", *x);
/// }
/// ```
/// Will print `5` then `3`.
#define FOR_EACH(ELEM, ELEMS) \
	for (typeof((ELEMS).value) ELEM = (ELEMS).value, _frst = ELEM; \
		(usize)(ELEM - _frst) < (ELEMS).len; \
		++ELEM)

#define foreach FOR_EACH

/* NOTES: */
/*
 * Read this: https://www.cprogramming.com/tutorial/unicode.html
 * by Jeff Bezanson, about modern unicode in C.
 *
 */
#endif
