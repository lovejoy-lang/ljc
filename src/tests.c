//! @file tests.c
//! File for testing parts of the compiler
//! and internal libraries, &c.

#include <lovejoy/common.h>
#include <lovejoy/utf.h>
#include <lovejoy/lexer.h>
#include <lovejoy/display.h>

#include <stdio.h>
#include <locale.h>

#define TEST(DOES) do { \
	println("\n[###] Test Case: %s", DOES); \
	println(  "      ↓↓↓↓ ↓↓↓↓"); \
} while (false); if (true)

u0 utf8_ucs4_conversions(byte *cstring)
{
	// UTF-8 string.
	string s = wrap_string(cstring);
	println("s = \"%s\"", UNWRAP(s));
	FOR_EACH(c, s) println("byte: 0x%X", *c);
	println("s.len = %zu", s.len);
	// Convert to UCS-4.
	println("[***] Convert to UCS-4.");
	runic ucs4 = SMAKE(rune, s.len + 1);
	ucs4 = utf8_to_ucs4(ucs4, s);
	println("ucs4 = \"%ls\"", (wchar_t *)UNWRAP(ucs4));
	FOR_EACH(c, ucs4) println("rune: U+%08X = '%lc'", *c, *c);
	println("ucs4.len = %zu", ucs4.len);
	// And back again.
	println("[***] Convert to UTF-8.");
	string utf8 = SMAKE(byte, 4 * ucs4.len + 4);
	utf8 = ucs4_to_utf8(utf8, ucs4);
	println("utf8 = \"%s\"", UNWRAP(utf8));
	FOR_EACH(c, utf8) println("byte: 0x%X", *c);
	println("utf8.len = %zu", utf8.len);
}


#ifndef IMPLEMENTATION

i32 main(i32 argc, const byte **argv)
{
	UNUSED(argc); UNUSED(argv);
	byte *locale; UNUSED(locale);
    locale = setlocale(LC_ALL, "");
	println("Locale is UTF-8?  %s.",
		is_locale_utf8(locale) ? "true" : "false");


	/* UTF-8 <--> UCS-4, conversions. */
	TEST("Converts between UTF-8 and UCS-4.") {
		println("=== Chinese Characters ===");
		utf8_ucs4_conversions("你好");
		println("\n=== Latin-1/ASCII Characters ===");
		utf8_ucs4_conversions("AaBb");
		println("\n=== Look-alike Characters ===");
		utf8_ucs4_conversions("maña");  // U+00F1 = ñ.
		utf8_ucs4_conversions("maña");  // n + U+0303 = ñ.
		// The second string has one extra 'code-point', but both
		// have the exact same number of human distinguishable 'graphemes'.
	}

	/* UTF-8 Escapes */
	TEST("Unescaped ASCII/UTF-8 strings to UTF-8") {
		println("=== Unescaping Strings ===");
		string unescaped = SMAKE(byte, 128);
		string escaped = STRING("Hello, \\U1F30E.");
		println("escaped.len = %zu", escaped.len);
		unescaped = utf8_unescape(unescaped, escaped);
		println("unescaped.len = %zu", unescaped.len);
		println("\"%s\" --> \"%s\"", escaped.value, unescaped.value);
	}

	TEST("Escaped UTF-8 strings to escaped ASCII") {
		println("=== Escaped Strings ===");
		string escaped = SMAKE(byte, 128);
		string unescaped = STRING("Hello, 🌎.");
		println("unescaped.len = %zu", unescaped.len);
		escaped = utf8_escape(escaped, unescaped, false);
		println("escaped.len = %zu", escaped.len);
		println("\"%s\" --> \"%s\"", unescaped.value, escaped.value);
	}

	return EXIT_SUCCESS;
}

#endif
