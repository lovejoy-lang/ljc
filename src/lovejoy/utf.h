#include "common.h"
#include <string.h>

#pragma once

/// Is `c` the start of a UTF-8 sequence?
#define is_utf(c) (((c) & 0xC0) != 0x80)

/// Wrap C-string to internal `string` slice.
/// Length does _not_ count the NUL-terminator.
/// @param[in] c NUL-terminated C-string to be wrapped
/// @returns UTF-8 string slice.
static inline
string to_string(const byte *c)
	{ return ((string){ .len = strlen(c), .value = (byte *)c }); }

/* Conversions. */

/// Convert UTF-8 to UCS-4 (4-byte wide characters)
/// No error checking is done, _must_ be valid UTF-8
/// and `dest` must be large enough. If `dest.len >= src.len + 1`,
/// then there will always be enough space.
/// @param[out] dest Empty runic structure.
/// @param[in] src UTF-8 encoded string.
/// @returns Slice of converted runic `dest` with correct length.
runic utf8_to_ucs4(runic dest, string src);

/// Convert UCS-4 to UTF-8, will try to NUL-terminate, if there is space.
/// No error checking is done. `dest.len >= 4 * src.len + 4`
/// To ensure a fit.  Ensure enough space in `dest` string.
/// @param[out] dest Empty string structure.
/// @param[in] src UCS-4/UTF-32 encoded string.
/// @returns Slice of `dest` with converted UTF-8 bytes
///          with correct length.
string ucs4_to_utf8(string dest, runic src);

/// Single UCS-4 rune to UTF-8 string.
/// `dest` should allocate 4 bytes, or 5 if it is desired to NUL-terminate.
/// @param[out] dest Empty string structure to hold UTF-8 bytes.
/// @param[in] ch Single UCS-4 character / rune.
/// @returns Slice of `dest` with correct length.
string rune_to_utf8(string dest, rune ch);

/* Moving through stirngs. */

/// Character number to byte offset.
/// Given the n-th character/rune in a string, how many
/// bytes is that from the start of a UTF-8 string.
usize byte_offset(string, usize);

/// Byte offset to character number.
/// Given a byte-offset from the start of a UTF-8 string,
/// return how many characters/runes precede it.
usize char_num(string, usize);

/// Step through a string, one rune/character at a time.
/// Given the previous index, and updating the index to the
/// begining of the next character.
/// @param[in] s String to step through.
/// @param[in,out] i Index with value of current position in the string,
///                  updating to the next start-of-character byte.
/// @returns The character (rune).
rune read_rune(string s, usize *i);

/// Update byte-index to move next rune, skipping it.
u0 next_rune(string, usize *);

/// Update byte-index to move to previous rune.
u0 prev_rune(string, usize *);

/* Unicode Escapes. */

/// Take an unescaped UTF-8 string, where the start of the string is
/// pointing to the character right after the backslash. If this character
/// is a `U`, then max eight (8) hexadecimal digits are expected to succeed
/// it, otherwise a `u` is expected, where max four (4) hexdecimal digits
/// are to succeed it.  The resulting rune is stored in `dest`.
/// @param[in] src String pointing to char after backslash.
/// @param[out] dest Pointer to location where resulting rune is to be stored.
/// @returns How many bytes read as part of parsing (including `u`/`U`).
///          Returns `0` if escape is invalid.
usize read_escape(string src, rune *dest);

/// Given a rune, convert it to an ASCII escape sequence.
/// @param[out] dest Empty string, should be large enough for minimum
///                  4 bytes, plus 1 byte for the NUL-terminator.
/// @param[in] ch The UCS-4 rune to convert from.
/// @returns Slice of `dest` with correct length.
string escape_rune(string dest, rune ch);

/// Convert a string containing ASCII escape sequences to
/// a proper UTF-8 string.
/// @param[out] dest Empty string structure.
/// @param[in] src String containg escapes.
/// @returns Slice of `dest` with correct length.
string utf8_unescape(string dest, string src);

/// Convert a string containing UTF-8 to ASCII with
/// escape sequences.
/// @param[out] dest Empty string structure.
/// @param[in] src String containg valid UTF-8.
/// @param[in] escape_quotes If true, quotation-marks will have
///                          backslashes prepended too.
/// @returns Slice of `dest` string with correct length.
string utf8_escape(string dest, string src, bool escape_quotes);

/* UTF-8 oriented standard function replacements. */

/// Find first occurrence of character `ch` in string `c`.
/// @param[in] s String to search through.
/// @param[in] ch Rune/character to find.
/// @param[out] i Pointer to be set to index of character in string.
/// @returns Slice of `s` string, starting at first occurence.
///          Points to nil with zero (0) length if no such
///          character is found.
string utf_strchr(string s, rune ch, usize *i);

/// Counts number of _characters_ (runes) in a UTF-8 string.
/// _Not_ the number of bytes, which is `s.len`.
usize utf_strlen(string s);

/// Returns length of next UTF-8 sequence.
usize utf_seqlen(string);

/// Give the C-string returned by `setlocale`, determine
/// whether the current locale speaks UTF-8.
bool is_locale_utf8(byte *locale);

/* Utility. */
bool octal_digit(byte c);
bool hex_digit(byte c);
