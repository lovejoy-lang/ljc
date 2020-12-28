//! @file tests.c
//! File for testing parts of the compiler
//! and internal libraries, &c.

#include <lovejoy/common.h>
#include <lovejoy/utf.h>
#include <lovejoy/lexer.h>
#include <lovejoy/display.h>

#include <stdio.h>
#include <locale.h>

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

i32 main(i32 argc, const byte **argv)
{
	UNUSED(argc); UNUSED(argv);
	byte *locale; UNUSED(locale);
    locale = setlocale(LC_ALL, "");

	println("=== Chinese Characters ===");
	utf8_ucs4_conversions("你好");
	println("\n=== Latin-1/ASCII Characters ===");
	utf8_ucs4_conversions("AaBb");

	return EXIT_SUCCESS;
}
