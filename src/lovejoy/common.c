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

usize push(u0 *array, const u0 *element, usize width)
{
	if (element == NULL) return 0;

	newarray(ByteArray, umin);
	ByteArray *dyn = (ByteArray *)array;
	umin *elem = (umin *)element;

	usize old_cap = dyn->cap;
	usize new_cap = old_cap;
	if (dyn->len < dyn->cap)
		goto do_push;

	// Otherwise, we need to reallocate for more space.
	new_cap = (usize)(dyn->cap * REALLOC_FACTOR + 1);
	dyn->value = (umin *)realloc(dyn->value, new_cap * width);
	if (dyn->value == NULL) {
		eprintln("Failed to realloc %lu bytes.", new_cap * width);
		abort();
	}
	dyn->cap = new_cap;

do_push:
	memcpy(dyn->value + width * dyn->len++, elem, width);
	return new_cap - old_cap;
}

u0 *pop(u0 *array, usize width)
{
	newarray(ByteArray, umin);
	ByteArray *dyn = (ByteArray *)array;

	return (u0 *)(dyn->value + --dyn->len * width);
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
