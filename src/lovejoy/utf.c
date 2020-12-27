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

	usize i = 0;
	while (i < dest.len - 1) {
		byte nb = trailing_bytes_for_UTF8[(usize)(*sptr)];
		if (sptr + nb >= src_end)
			goto done_utf8_to_ucs4;

		rune ch = 0;
		switch (nb) {
			case 3: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 2: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 1: ch += *sptr++; ch <<= 6; /* fallthrough */
			case 0: ch += *sptr++;
		}
		ch -= offsets_from_UTF8[(usize)nb];
		dest.value[i++] = ch;
	}

done_utf8_to_ucs4:
	dest.value[i] = 0; // Null-terminate anyway, despite known length.
	return SLICE(dest, 0, i);
}

#endif
