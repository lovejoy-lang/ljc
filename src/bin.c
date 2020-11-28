#include <stdio.h>

#include "lovejoy/common.h"

i32 main(i32 argc, char **argv)
{
	puts("ljc - Lovejoy Compiler, " VERSION ".");

	for (i32 i = 1; i < argc; ++i)
		if (*argv[i] == '-')
			printf("Option: %s\n", argv[i]);
		else
			printf("Source file: %s\n", argv[i]);

	return EXIT_SUCCESS;
}
