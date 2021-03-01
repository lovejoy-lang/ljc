//! @file bin.c
//! Interactions with the main executable.
//! This will *usually* be how most programs are compiled.

#include <lovejoy/common.h>
#include <lovejoy/utf.h>
#include <lovejoy/lexer.h>
#include <lovejoy/display.h>

#include <stdio.h>
#include <locale.h>

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
	byte *locale; UNUSED(locale);
    locale = setlocale(LC_ALL, "");

	eputs("ljc - Lovejoy Compiler, " VERSION ".");

	for (i32 i = 1; i < argc; ++i)
		if (*argv[i] == '-') {
			eprintf("Option: %s\n", argv[i]);
		} else {
			eprintf("Source file: %s\n", argv[i]);

			FILE *f = fopen(argv[i], "rb");
			if (f == nil) {
				eprintln("Could not open `%s' for reading.", argv[i]);
				return 1;
			}

			byte *source;
			usize source_len;

			fseek(f, 0, SEEK_END);
			source_len = ftell(f);
			rewind(f);
			source = (byte *)calloc(source_len + 1, sizeof(byte));
			fread(source, source_len, 1, f);

			byte *bin __attribute__((unused));
			bin = compile_source(argv[i], source);
			fclose(f);
			free(source);
		}

	return EXIT_SUCCESS;
}

#ifndef IMPLEMENTATION

byte *compile_source(const byte *name, const byte *source)
{
	eprintln("Compiling: `%s'.", name);

	LexerContext lexer_ctx = NewLexer();
	lexer_ctx.filename = name;

	Lexeme *lexeme = nil;
	until (nil == (lexeme = lex(&lexer_ctx, source))) {
		source = lexeme->end;

		// Print fo debugging
		eprintln("(lexem %02lu:%02lu :%S ‘%S’)",
			lexeme->lineno,
			lexeme_col(lexeme),
			display_tt(lexeme->type),
			(lexeme->type != TT_TERM)
				? lexeme_substring(lexeme)
				: STR(";"));

		lexeme_free(lexeme);
	}
	eputs("EOT - End of Token Stream");

	// Not producing executables yet.
	return nil;
}

#endif
