# [DRAFT] Language Specification

## Syntax

[...]

### Keywords

Reserved keywords are: `data`, `jump`, `null`, `mut`,
`match`, `return`.
Other language constructs (e.g. `if`, `while`, etc.) are implemented
through macros, `data` definitions, etc.


### GADTs

Lovejoy does not have `stuct`s/`record`s, `union`s nor `enum`s.
Instead it takes a completely generalised approach, using the `data`
keyword.


## Built-ins

### Types

### Integers

| Signed | Unsigned |
|--------|----------|
| N/A    | `u0` (unit/void) |
| `s8`   | `u8`     |
| `s16`  | `u16`    |
| `s32`  | `u32`    |
| `s64`  | `u64`    |
| `s128` | `u128`   |

### Arrays and pointers

#### Raw Pointer
```ada
*byte_pointer: u8
byte_pointer: &u8
-- In C:
-- uint8_t *byte_pointer;
```

#### Nullable Pointer

Exact same as a raw pointer, but whose value may be `null`.
```ada
maybe_a_byte?: u8
maybe_a_byte: u8?
```

This is because the `?` operator is used to dereference a nullable pointer, if the pointer were null, it forces the current function to immediately return null, otherwise it will just attempt to dereference it normally.
e.g.

```ada
my_func(): Object? = {
	maybe_o?: Object = may_return_null_pointer();
	o: Object = maybe_o?  -- dereference.
	o.x = 3
	return o
}

main(argv[][]: u8): u0 = {
	obj: Object? = my_func();
	not_null: &Object = match obj {
		null => exit(1)
		o => o
	}

	print(*not_null)
	exit(0)
}

```

#### Statically Sized Array
```ada
byte_array[20]: u8
byte_array: [u8; 20]
-- In C:
-- uint8_t byte_array[20]

-- Statically known size, not stored at runtime.
byte_array.size      -->  0
byte_array.capacity  --> 20
```

#### Dynamic Arrays / Slices
```ada
dyn_byte_array[]: u8
dyn_byte_array: &[u8]  -- Automatically a fat pointer!
```

Dynamic arrays and arrays of unknown size/capacity
have no distinction.  All arrays (dynamic or not) can
be treated the same, i.e. you may push and pop them, but
you may not push more elements to a fixed capacity array
than has been stated in the type.

**NOTE:** you will never see a variable with type of [u8],
since its size is not known at compile time, and thus
cannot be stored on the stack, it must always be a reference
to a [u8] on the heap, i.e. &[u8], which is fat, and stores
size and capacity information with it. e.g.
```ada
SliceRef := data($A) SliceRef(*ptr: A, capacity: usize, size: usize)
```
So, `&[u8]` is essentially the same as a `SliceRef(u8)`.

```ada
byte_array_slice[]: u8
byte_array_slice: &[u8]
byte_array_slice = byte_array[2:5]
```
This is a slice, it does not create a new array,
it is just a view into the old one. It stores a pointer
to the second element of byte_array, a length, and a capacity (of 3).
pushing to a slice will work if length < capacity. This will also
modify the original array.  It is, in most cases, not smart.

#### Sizes of Such Types

```ada
sizeof(&[u8]) == 2 * sizeof(usize) + sizeof(&u8)  -- Fat pointer.
sizeof([u8; 6]) == 6 * sizeof(u8)  -- Not a pointer.
sizeof(&[u8; 6]) == sizeof(&u8)  -- Skinny pointer, size of any other pointer.
sizoof([u8]) -- ERROR! [u8] has no known size...
```

### Special Types

#### Unit
The unit type (`Unit`/`u0`) defined as
```ada
Unit := data { Unit }
u0: Type := Unit
```
such that `sizeof(u0) = 0`, `u0` has one value (`Unit`), since `u0` stores
zero bits of information, it may only have one value (`2^0 = 1` values).

#### Bottom

The bottom type (or the *never type*), is the type of something that
never finishes evaluating.  It has no value, naturally.

Defined as
```ada
! := data {}
```
