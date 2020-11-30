//! @file bin.c
//! Interactions with the main executable.
//! This will *usually* be how most programs are compiled.

#include <lovejoy/common.h>
#include <stdio.h>

/**
 * Handle command-line arguments,
 * compile files with given options.
 */
i32 main(i32 argc, byte **argv)
{
	puts("ljc - Lovejoy Compiler, " VERSION ".");

	for (i32 i = 1; i < argc; ++i)
		if (*argv[i] == '-')
			printf("Option: %s\n", argv[i]);
		else
			printf("Source file: %s\n", argv[i]);

	return EXIT_SUCCESS;
}
