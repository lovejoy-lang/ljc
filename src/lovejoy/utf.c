#include "utf.h"

static const rune OffsetsFromUTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const u8 TrailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

#ifndef IMPLEMENTATION

/* Conversions. */

usize utf_seqlen(string s)
{
    return TrailingBytesForUTF8[(usize)(*s.value)] + 1;
}

runic utf8_to_ucs4(runic dest, string src)
{
	byte *src_end = src.value + src.len;
	byte *sptr = src.value;

	usize i;
	for (i = 0; i < dest.len - 1; ++i) {
		u8 nb = TrailingBytesForUTF8[(usize)(*sptr)];
		if (sptr + nb >= src_end)
			break;

		rune ch = 0;
		switch (nb) {
			case 3: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 2: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 1: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 0: ch += *sptr++;
		}
		ch -= OffsetsFromUTF8[(usize)nb];
		dest.value[i] = ch;
	}

	dest.value[i] = 0; // Always NUL-terminate, despite known length.
	return SLICE(runic, dest, 0, i);
}

string ucs4_to_utf8(string dest, runic src)
{
	byte *dest_end = dest.value + dest.len;
	byte *dptr = dest.value;

	for (usize i = 0; i < src.len; ++i) {
		rune ch = src.value[i];

		if (ch < 0x80) {
			if (dptr >= dest_end)
				break;
			*dptr++ = (byte)ch;
		} else if (ch < 0x800) {
			if (dptr >= dest_end - 1)
				break;
			*dptr++ = (ch >> 6) | 0xC0;
			*dptr++ = (ch & 0x3F) | 0x80;
		} else if (ch < 0x10000) {
			if (dptr >= dest_end - 2)
				break;
			*dptr++ = (ch >> 12) | 0xE0;
			*dptr++ = ((ch >> 6) & 0x3F) | 0x80;
			*dptr++ = (ch & 0x3F) | 0x80;
		} else if (ch < 0x110000) {
			if (dptr >= dest_end - 3)
				break;
			*dptr++ = (ch >> 18) | 0xF0;
			*dptr++ = ((ch >> 12) & 0x3F) | 0x80;
			*dptr++ = ((ch >> 6) & 0x3F) | 0x80;
			*dptr++ = (ch & 0x3F) | 0x80;
		}
	}

	if (dptr < dest_end)
		*dptr = '\0';
	return SLICE(string, dest, 0, dptr - dest.value);
}

string rune_to_utf8(string dest, rune ch)
{
	byte *const dptr = dest.value;
	zero(dptr, dest.len * sizeof(byte));

	if (ch < 0x80) {
		dptr[0] = (byte)ch;
		return SLICE(string, dest, 0, 1);
	}
	if (ch < 0x800) {
		dptr[0] = (ch >> 6) | 0xC0;
		dptr[1] = (ch & 0x3F) | 0x80;
		return SLICE(string, dest, 0, 2);
	}
	if (ch < 0x10000) {
		dptr[0] = (ch >> 12) | 0xE0;
		dptr[1] = ((ch >> 6) & 0x3F) | 0x80;
		dptr[2] = (ch & 0x3F) | 0x80;
		return SLICE(string, dest, 0, 3);
	}
	if (ch < 0x110000) {
		dptr[0] = (ch >> 18) | 0xF0;
		dptr[1] = ((ch >> 12) & 0x3F) | 0x80;
		dptr[2] = ((ch >> 6) & 0x3F) | 0x80;
		dptr[3] = (ch & 0x3F) | 0x80;
		return SLICE(string, dest, 0, 4);
	}
	return SLICE(string, dest, 0, 0);
}

/* Moving through stirngs. */

usize byte_offset(string s, usize cn)
{
	usize offs = 0;

	while (cn-- > 0 && offs < s.len)
		UNUSED(is_utf(s.value[++offs]) || is_utf(s.value[++offs])
		    || is_utf(s.value[++offs]) || ++offs);

	return offs;
}

usize char_num(string s, usize offset)
{
	usize cn = 0, offs = 0;

	while (offs < offset && offs < s.len) {
		UNUSED(is_utf(s.value[++offs]) || is_utf(s.value[++offs])
		    || is_utf(s.value[++offs]) || ++offs);
		++cn;
	}

	return cn;
}

rune read_rune(string s, usize *i)
{
	if (*i >= s.len) return 0;

	rune ch = 0;
	usize len = 0;

	do {
		ch <<= 6;
		ch += s.value[(*i)++];
		++len;
	} while (*i < s.len && !is_utf(s.value[*i]));
	ch -= OffsetsFromUTF8[len - 1];

	return ch;
}

u0 next_rune(string str, usize *i)
{
	byte *s = str.value;
	UNUSED(is_utf(s[++(*i)]) || is_utf(s[++(*i)])
	    || is_utf(s[++(*i)]) || ++(*i));
}

u0 prev_rune(string str, usize *i)
{
	byte *s = str.value;
	UNUSED(is_utf(s[--(*i)]) || is_utf(s[--(*i)])
	    || is_utf(s[--(*i)]) || --(*i));
}

/* Unicode Escapes. */

usize read_escape(string src, rune *dest)
{
	rune ch;
	byte *sptr = src.value;
	byte digs[8 + 1] = "\0\0\0\0\0\0\0\0";
	usize dno = 0, i = 1;

	ch = (rune)sptr[0];  // Take the literal character.

	switch (sptr[0]) {
	case 'n': { ch = U'\n'; break; }
	case 't': { ch = U'\t'; break; }
	case 'r': { ch = U'\r'; break; }
	case 'b': { ch = U'\b'; break; }
	case 'f': { ch = U'\f'; break; }
	case 'v': { ch = U'\v'; break; }
	case 'a': { ch = U'\a'; break; }
	case 'x':  // e.g. \x41 = 'A'
		while (hex_digit(sptr[i]) && dno < 2)
			digs[dno++] = sptr[i++];
		break;
	case 'u':  // e.g. \u6CB3 = '河'
		while (hex_digit(sptr[i]) && dno < 4)
			digs[dno++] = sptr[i++];
		break;
	case 'U':  // e.g. \U0001F920 = '🤠'
		while (hex_digit(sptr[i]) && dno < 8)
			digs[dno++] = sptr[i++];
		break;
	default:
		break;
	}

	if (dno > 0)
		ch = strtol(digs, nil, 16);

	*dest = ch;
	return i;
}

string escape_rune(string dest, rune ch)
{
	byte *buf = dest.value;
	usize len = dest.len;

	switch (ch) {
	case U'\n': return SLICE(string, dest, 0, snprintf(buf, len, "\\n"));
	case U'\t': return SLICE(string, dest, 0, snprintf(buf, len, "\\t"));
	case U'\r': return SLICE(string, dest, 0, snprintf(buf, len, "\\r"));
	case U'\b': return SLICE(string, dest, 0, snprintf(buf, len, "\\b"));
	case U'\f': return SLICE(string, dest, 0, snprintf(buf, len, "\\f"));
	case U'\v': return SLICE(string, dest, 0, snprintf(buf, len, "\\v"));
	case U'\a': return SLICE(string, dest, 0, snprintf(buf, len, "\\a"));
	case U'\\': return SLICE(string, dest, 0, snprintf(buf, len, "\\\\"));
	}

	if (ch < 32 || ch == 0x7F)
		return SLICE(string, dest, 0, snprintf(buf, len, "\\x%hhX", (u8)ch));
	else if (ch > 0xFFFF)
		return SLICE(string, dest, 0, snprintf(buf, len, "\\U%X",  (u32)ch));
	else if (ch >= 0x80 && ch <= 0xFFFF)
		return SLICE(string, dest, 0, snprintf(buf, len, "\\u%hX", (u16)ch));

	return SLICE(string, dest, 0, snprintf(buf, len, "%c", (byte)ch));
}

string utf8_unescape(string dest, string src)
{
	rune ch;
	byte buf[4];
	string temp = { .len = 4, .value = buf };
	usize c = 0, i = 0, amt;

	while (i < src.len && c < dest.len) {
		if (src.value[i] == '\\') {
			++i;
			amt = read_escape(SLICE(string, src, i, -1), &ch);
		} else {
			ch = (rune)(src.value[i]);
			amt = 1;
		}

		i += amt;
		temp = rune_to_utf8(temp, ch);
		amt = temp.len;

		if (amt > dest.len - c)
			break;

		memcpy(dest.value + c, temp.value, amt);
		c += amt;
	}

	if (c < dest.len)  // Try to NUL-terminate.
		dest.value[c] = '\0';

	return SLICE(string, dest, 0, c);
}


string utf8_escape(string dest, string src, bool escape_quotes)
{
	byte *buf = dest.value;
	usize c = 0, i = 0;

	while (c < dest.len && i < src.len) {
		string slice = { .value = buf, .len = dest.len - c };

		if (escape_quotes && src.value[i++] == '"')
			slice = SLICE(string, slice, 0, snprintf(buf, slice.len, "\\\""));
		else
			slice = escape_rune(slice, read_rune(src, &i));

		c   += slice.len;
		buf += slice.len;
	}

	if (c < dest.len)
		*buf = '\0';

	return SLICE(string, dest, 0, c);
}

/* Standard function replacements. */

string utf_strchr(string s, rune ch, usize *i)
{
	usize offs = 0, last_offs = 0;
	rune c;

	*i = 0;
	until (offs >= s.len) {
		c = read_rune(s, &offs);
		if (c == ch)
			return SLICE(string, s, last_offs, -1);

		last_offs = offs;
		++(*i);
	}

	return SEMPTY(string);
}

usize utf_strlen(string s)
{
	usize i = 0, count = 0;

	until (read_rune(s, &i) == 0)
		++count;

	return count;
}


/// This code based on the `libutf8` function.
bool is_locale_utf8(byte *locale)
{
	const byte* cp = locale;

	for (; *cp != '\0' && *cp != '@' && *cp != '+' && *cp != ','; cp++) {
		if (*cp == '.') {
			const byte* encoding = ++cp;
			for (; *cp != '\0' && *cp != '@' && *cp != '+' && *cp != ','; cp++)
				;
			if ((cp-encoding == 5 && !strncmp(encoding, "UTF-8", 5))
				|| (cp-encoding == 4 && !strncmp(encoding, "utf8", 4)))
				return true;  // It's UTF-8.
			break;
		}
	}

	return false;  // :(
}

/* Utility. */

bool octal_digit(byte c)
{
	return (c >= '0' && c <= '7');
}

bool hex_digit(byte c)
{
	return ((c >= '0' && c <= '9')
	     || (c >= 'A' && c <= 'F')
	     || (c >= 'a' && c <= 'f'));
}

#endif
