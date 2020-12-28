#include "utf.h"

static const rune offsets_from_UTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const byte trailing_bytes_for_UTF8[256] = {
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

usize utf_seqlen(string s)
{
    return trailing_bytes_for_UTF8[(usize)(*s.value)] + 1;
}

runic utf8_to_ucs4(runic dest, string src)
{
	byte *src_end = src.value + src.len;
	byte *sptr = src.value;

	usize i;
	for (i = 0; i < dest.len - 1; ++i) {
		byte nb = trailing_bytes_for_UTF8[(usize)(*sptr)];
		if (sptr + nb >= src_end)
			break;

		rune ch = 0;
		switch (nb) {
			case 3: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 2: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 1: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 0: ch += *sptr++;
		}
		ch -= offsets_from_UTF8[(usize)nb];
		dest.value[i] = ch;
	}

	dest.value[i] = 0; // Always null-terminate, despite known length.
	return SLICE(dest, 0, i);
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
	return SLICE(dest, 0, dptr - dest.value);
}

#endif
