#include "common.h"
#include "assert.h"

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

u0 zero(u0 *blk, usize width)
{
	if (blk == NULL || width == 0)
		return UNIT;
	umin *_blk = blk;
	until (width-- == 0)
		*_blk++ = 0;
	return UNIT;
}

u0 *emalloc(usize len, usize size)
{
	usize bytes = len * size;
	u0 *m = malloc(bytes);
	if (m == nil) {
		eprintf("Could not allocate %lu bytes.\n", bytes);
		abort();
	}
	zero(m, bytes);
	return m;
}

usize push(u0 *arr, const u0 *element, usize width)
{
	if (element == nil) return 0;

	newarray(ByteArray, umin);
	ByteArray *dyn = (ByteArray *)arr;
	umin *elem = (umin *)element;

	usize old_cap = dyn->cap;
	usize new_cap = old_cap;

	unless (dyn->len < dyn->cap) {
		new_cap = (usize)(dyn->cap * REALLOC_FACTOR + 1);
		dyn->value = (umin *)realloc(dyn->value, new_cap * width);
		if (dyn->value == nil) {
			eprintln("Failed to realloc %lu bytes.", new_cap * width);
			abort();
		}
		dyn->cap = new_cap;
	}

	memcpy(dyn->value + width * dyn->len++, elem, width);
	return new_cap - old_cap;
}

u0 *pop(u0 *array, usize width)
{
	newarray(ByteArray, umin);
	ByteArray *dyn = (ByteArray *)array;

	assert(dyn->len > 0);

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
