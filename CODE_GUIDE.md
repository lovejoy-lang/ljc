# Pointers
Try to use `const` wherever you can.

# Types

Use types in `lovejoy/common.h`.
DO NOT use types like `int` or `unsigned long`, use more immdeiate names
like `i32` or `u64`, etc.

# Comments

Use regular code comments to help convey what your code is doing.
Use markdown in your comments (if you need formatting to convey your message).

## Doc-comments

Try to describe all data-types, data-fields and functions with
a doc-comment.

Inline doc-comments are written as `///`, e.g.:
```c
/// Documenting this *struct* below
struct Hello {
	/// Holds greeting message.
	byte *message;
};
```

Block doc-comments are written as `/** ... */`, e.g.:
```c
/**
 * # Documenting the below function.
 * Some *long* description.
 */
u16 my_function(f64 foo)
{
	//[...]
}
```

## File comments

Try to describe the general purpose of the file you are writing with file
comments, e.g. (inline):
```c
//! This file does so and so...

#include <stdlib.c>
//[...]
```

e.g. (block):
```c
/*!
 * This file does a lot of things.
 * Blah Blah.
 */

#include "lovejoy/common.h"
//[...]
```

# Structs

Prefer `typdef`s, e.g.

```c
struct Example {
	u8 a, b, c;
	f64 f;
};

record(Example);

// ^ Expands to
typedef struct Example Example;
```

or, without the macro

```c
typedef struct __MyList {
	i32 value;
	struct __MyList *next;
} MyList;
```
