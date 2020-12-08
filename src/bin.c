//! @file bin.c
//! Interactions with the main executable.
//! This will *usually* be how most programs are compiled.

#include <lovejoy/common.h>
#include <lovejoy/lexer.h>
#include <lovejoy/display.h>

#include <stdio.h>

/**
 * Compiles source code to binary executable.
 */
byte *compile_source(const byte *, const byte *);

/**
 * Handle command-line arguments,
 * compile files with given options.
 */
i32 main(i32 argc, const byte **argv)
{
	fputs("ljc - Lovejoy Compiler, " VERSION ".\n", stderr);

	for (i32 i = 1; i < argc; ++i)
		if (*argv[i] == '-') {
			fprintf(stderr, "Option: %s\n", argv[i]);
		} else {
			fprintf(stderr, "Source file: %s\n", argv[i]);

			FILE *f = fopen(argv[i], "rb");
			if (f == NULL)  return 1;

			byte *source;
			usize source_len;

			fseek(f, 0, SEEK_END);
			source_len = ftell(f);
			rewind(f);
			source = calloc(1, source_len + 1);
			fread(source, source_len, 1, f);

			byte *bin = compile_source(argv[i], source);
			fclose(f);
			free(source);
		}

	return EXIT_SUCCESS;
}

#ifndef IMPLEMENTATION

byte *compile_source(const byte *name, const byte *source)
{
	fprintf(stderr, "Compiling: `%s'.\n", name);

	LexerContext lexer_ctx = NewLexer;
	lexer_ctx.filename = name;

	Lexeme *lexeme = NULL;
	while (NULL != (lexeme = lex(&lexer_ctx, source))) {
		source = lexeme->end;

		// Print fo debugging
		fprintf(stderr, "lexem[%lu] -> %s\n",
			lexeme->lineno,
			display_lexeme(lexeme));
	}

	return NULL;
}

#endif
