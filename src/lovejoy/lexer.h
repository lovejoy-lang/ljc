#pragma once

#include "common.h"

/// Lexer environment.
struct LexerContext {
	const byte *filename;
	const byte *lineptr;  ///< Start of current line pointer.
	usize lineno;
};
unqualify(struct, LexerContext);

extern LexerContext NewLexer;

/// Token Type (TT), classifies each lexeme.
enum TokenType {
	TT_NUMBER,  ///< Literal.
	TT_STRING,  ///< Literal.
	TT_CHAR,    ///< Literal.
	TT_OPERATOR,
	TT_IDENT,
	TT_LPAREN,
	TT_RPAREN,
	TT_LBRACKET,
	TT_RBRACKET,
	TT_LCURLY,
	TT_RCURLY,
	TT_TERM,  ///< Statement terminator.
	TT_NONE ///< Whitespace, &c.
};
unqualify(enum, TokenType);

/// Lexeme, a substring of the source.
struct Lexeme {
	TokenType type;
	const byte *start;  ///< Pointer to start of token in source.
	const byte *end;    ///< Pointer to end of token in source.
	const byte *line;   ///< Pointer to start of line token is on.
	usize lineno;  ///< Line number og token.
};
unqualify(struct, Lexeme);

/// Number of bytes in token.
usize lexeme_span(const Lexeme *);

/// Get stand-alone substring of lexeme.
/// @note Allocates memory.
byte *lexeme_substring(const Lexeme *);

/// Type of token character probably belongs to.
TokenType character_type(byte);

/// Advances lexer, returns token.
/// Setting the source pointer to Lexeme#end
/// will let you step through the source token by token.
/// Look-ahead is accomplished by setting it to Lexeme#start,
/// or simply not changing the source pointer.
/// @note Allocates memory.
Lexeme *lex(LexerContext *, const byte *source);

/// N-token look-ahead.
/// @param count How many tokens too skip ahead to.
/// @note Allocates memory.
Lexeme *peek(LexerContext *, u16 count, const byte *source);

/// Free a lexeme from memory.
u0 lexeme_free(Lexeme *);
