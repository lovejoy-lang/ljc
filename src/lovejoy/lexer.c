#include "lexer.h"
#include <string.h>

#ifndef IMPLEMENTATION

LexerContext NewLexer = {
	.filename = "<stdin>",
	.lineptr = NULL,
	.lineno = 1,
	.last_token_type = TT_NONE
};

byte *lexeme_substring(const Lexeme *lexeme)
{
	usize span = lexeme_span(lexeme);
	byte *ss = (byte *)malloc((span + 1) * sizeof(byte));
	strncpy(ss, lexeme->start, span);
	ss[span + 1] = '\0';
	return ss;
}

u0 lexeme_free(Lexeme *lexeme)
{
	free((u0 *)lexeme);
}

Lexeme *peek(LexerContext *ctx, u16 count, const byte *source)
{
	LexerContext tmp_ctx = *ctx;

	Lexeme *peeked = NULL;
	for (u16 i = 0; i < count; ++i) {
		peeked = lex(&tmp_ctx, source);
		if (peeked == NULL) return NULL;
		source = peeked->end;
		if (i != count - 1)  // While not at last iteration:
			lexeme_free(peeked);
	}

	return peeked;
}

TokenType character_type(byte chr)
{
	// Spaces or null.
	switch (chr) {
		case '\n':
		case ';':
			return TT_TERM;
		case '\0':
		case '\t':
		case '\v':
		case '\f':
		case '\r':
		case 0xA0:
		case ' ':
			return TT_NONE;
	}

	if ((chr >= 'A' && chr <= 'Z')
	 || (chr >= 'a' && chr <= 'z')
	 || (chr == '_' || chr == '$')
	 || (chr >= 0xA1 ))
		return TT_IDENT;
	if (chr >= '0' && chr <= '9')
		return TT_NUMBER;

	if (chr == '"')  return TT_STRING;
	if (chr == 0x27) return TT_CHAR;

	if (chr == '(') return TT_LPAREN;
	if (chr == ')') return TT_RPAREN;
	if (chr == '{') return TT_LCURLY;
	if (chr == '}') return TT_RCURLY;
	if (chr == '[') return TT_LBRACKET;
	if (chr == ']') return TT_RBRACKET;

	if ((chr >= 0x01 && chr <= 0x1f)
	 || (chr >= 0x7f && chr <= 0x9f))
		return TT_NONE;  // Control characters.

	// Default is operator, since they're spread everywhere.
	return TT_OPERATOR;
}

/// @private
static const byte *skip_whitespace(const byte *source)
{
	while (1)
		switch (*source) {
		case '\0': return source;
		case '\t':
		case '\v':
		case '\f':
		case '\r':
		case 0xA0:
		case ' ':
			++source;
			break;
		default:
			return source;
		}
}

Lexeme *lex(LexerContext *ctx, const byte *source)
{
	if (ctx->lineptr == NULL)
		ctx->lineptr = source;
	if (*source == '\0') return NULL;

	source = skip_whitespace(source);
	if (*source == '\0') return NULL;

	// Look for comments
	if (*source == '-') {
		if (*(source + 1) == '-') {  // EOL comment.
			++source;
			until (*source++ == '\0' || *source == '\n');
		} else if (*(source + 1) == '*') {  // Multiline comment.
			// TODO: Allow for nested multiline comments.
			++source;
			until (*source == '\0'
			|| (*source++ == '*' && *source == '-')
			|| *source == '\0')
				if (*source == '\n') {
					++ctx->lineno;
					ctx->lineptr = source + 1;
				}
			if (*source != '\0') ++source;
		}
	}

	source = skip_whitespace(source);
	if (*source == '\0') return NULL;

	// Collect multiple terminators together.
	bool is_terminal = false;
	do switch (*source) {
		case '\n':
			++ctx->lineno;
			ctx->lineptr = source + 1;
			// fallthrough
		case ';':
			++source;
			is_terminal = true;
			break;
		default:
			if (is_terminal) --source;
			goto make_token;
	} while (is_terminal);

make_token:;
	TokenType tt = character_type(*source);
	if (tt == TT_NONE) return NULL;
	// Stop the lexer from returning many consecutive
	// terminator tokens.
	if (tt == TT_TERM && ctx->last_token_type == TT_TERM)
		return lex(ctx, source + 1);  // Tail recursion should be optimised.

	Lexeme *token = (Lexeme *)malloc(sizeof(Lexeme));
	token->type = tt;
	token->start = source;
	token->line = ctx->lineptr;
	token->lineno = ctx->lineno;

	if (tt == TT_TERM) {
		token->end = source + 1;
		goto return_token;
	}
	if (tt == TT_STRING) {
		// TODO: String parsing w/ escapes...
		// Maybe use repeated character parsing.
		token->end = source + 1;
		goto return_token;
	}
	if (tt == TT_CHAR) {
		// TODO.
		token->end = source + 1;
		goto return_token;
	}

	switch (tt) {
	case TT_LPAREN:
	case TT_RPAREN:
	case TT_LBRACKET:
	case TT_RBRACKET:
	case TT_LCURLY:
	case TT_RCURLY:
		token->end = source + 1;
		goto return_token;
	case TT_NUMBER:
		// TODO: Decimal point.
		// TODO: Scientific notation, e.g. 3.7e-11.
		// TODO: Hex exponent, e.g. 0xF1.D3p+4A
		// TODO: Radix notation, e.g. 0775r8 == 0o0775.
		// Mainly we need to accommodate for the
		// use of '.', '+' and '-' characters, which would
		// otherwise be considered TT_OPERATORs, as opposed
		// to being part of TT_NUMBER literals.
		while (tt == TT_NUMBER || tt == TT_IDENT)
			tt = character_type(*(++source));

		token->end = source;
		goto return_token;
	case TT_IDENT:
		while (tt == TT_NUMBER || tt == TT_IDENT)
			tt = character_type(*(++source));

		token->end = source;
		goto return_token;
	case TT_OPERATOR:
		// TODO: Check which operators exists, and try and
		// match against the longest operators first.
		// e.g.  2+-3 becomes 2 + (-3), but if an operator '+-' exists,
		// then it becomes 2 +- 3.
		token->end = source + 1;
		goto return_token;
	default:
		return NULL;
	}

return_token:;
	ctx->last_token_type = token->type;
	return token;
}

#endif