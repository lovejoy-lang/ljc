#pragma once

#include "common.h"
#include "operators.h"

/// Token Type (TT), classifies each lexeme.
enum TokenType {
	TT_NUMBER = 0,  ///< Literal.
	TT_STRING,      ///< Literal.
	TT_CHAR,        ///< Literal.
	TT_OPERATOR,
	TT_IDENT,
	TT_LPAREN,
	TT_RPAREN,
	TT_LBRACKET,
	TT_RBRACKET,
	TT_LCURLY,
	TT_RCURLY,
	TT_TERM,  ///< Statement terminator.
	TT_NONE   ///< Whitespace, &c.
};
unqualify(enum, TokenType);

/// Lexer environment.
struct LexerContext {
	const byte *filename;
	const OperatorTable operator_table;  ///< Lexer will not add operators.
	const byte *lineptr;  ///< Start of current line pointer.
	usize lineno;
	TokenType last_token_type;
};
unqualify(struct, LexerContext);

/// Most basic/initialized lexer context.
LexerContext NewLexer();

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

static inline
usize lexeme_span(const Lexeme *lexeme)
	{ return lexeme->end - lexeme->start; }

/// Line column number that token starts at.
static inline
usize lexeme_col(const Lexeme *lexeme)
	{ return lexeme->start - lexeme->line + 1; }

/// Get stand-alone substring of lexeme.
/// @note Allocates memory.
string lexeme_substring(const Lexeme *);

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
/// @param source Source code, offset by end of current token.
/// @note Allocates memory.
Lexeme *peek(LexerContext *, u16 count, const byte *source);

/// Free a lexeme from memory.
u0 lexeme_free(Lexeme *);
