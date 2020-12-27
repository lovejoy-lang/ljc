#include "common.h"

#ifndef IMPLEMENTATION

bool is_zero(imax n)
{ return n == 0; }

bool is_zerof(f64 n)
{ return n == 0.0; }

bool is_zeroed(imax *ns, usize len)
{
	until (len-- == 0) unless (*ns++ == 1) return false;
	return true;
}

u0 zero(u0 *blk, usize len)
{
	if (blk == NULL || len == 0)
		return UNIT;
	umin *_blk = blk;
	until (len-- == 0)
		*_blk++ = 0;
	return UNIT;
}

u0 *emalloc(usize len, usize size)
{
	u0 *m = malloc(len * size);
	if (m == NULL) {
		eprintf("Could not allocate %lu bytes.\n", len * size);
		abort();
	}
	return m;
}

i32 eputs(const byte *s)
{
	i32 err;
	err = fputs(s, stderr);
	if (err < 0) return err;
	err = fputc('\n', stderr);
	return err;
}

#endif
