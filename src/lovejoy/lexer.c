#include "lexer.h"
#include <string.h>
#include <lovejoy/utf.h>

#ifndef IMPLEMENTATION

LexerContext NewLexer()
{
	const OperatorTable table = DefaultOperators();
	LexerContext ctx = {
		.filename = "<stdin>",
		.operator_table = table,
		.lineptr = nil,
		.lineno = 1,
		.last_token_type = TT_NONE
	};

	return ctx;
}

string lexeme_substring(const Lexeme *lexeme)
{
	usize span = lexeme_span(lexeme);
	byte *ss = emalloc(span + 1, sizeof(byte));
	strncpy(ss, lexeme->start, span);
	ss[span] = '\0';
	return wrap_string(ss);
}

u0 lexeme_free(Lexeme *lexeme)
{
	free((u0 *)lexeme);
}

Lexeme *peek(LexerContext *ctx, u16 count, const byte *source)
{
	LexerContext tmp_ctx = *ctx;

	Lexeme *peeked = nil;
	for (u16 i = 0; i < count; ++i) {
		peeked = lex(&tmp_ctx, source);
		if (peeked == nil) return nil;
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

	switch (chr) {
	case '"':  return TT_STRING;
	case 0x27: return TT_CHAR;
	case '(':  return TT_LPAREN;
	case ')':  return TT_RPAREN;
	case '{':  return TT_LCURLY;
	case '}':  return TT_RCURLY;
	case '[':  return TT_LBRACKET;
	case ']':  return TT_RBRACKET;
	}

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
	if (ctx->lineptr == nil)
		ctx->lineptr = source;
	if (*source == '\0') return nil;

	source = skip_whitespace(source);
	if (*source == '\0') return nil;

	// Look for comments.
	if (*source == '-') {  // All comments start with '-', (-{-,*}).
		if (*(source + 1) == '-') {  // EOL comment.
			++source;
			until (*source++ == '\0' || *source == '\n');
		} else if (*(source + 1) == '*') {  // Multiline comment.
			// TODO: Allow for nested multiline comments.
			source += 2; // Skip the '-' and '*'.
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
	if (*source == '\0') return nil;

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
	if (tt == TT_NONE) return nil;
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
		// Find the EOS.
		usize eos = 1;
		until (source[eos++] == '"') {
			if (source[eos - 1] == '\\'
			&& (source[eos] == '"' || source[eos] == '\\'))
				++eos;
		}
		token->end = source + eos;
		goto return_token;
	}
	if (tt == TT_CHAR) {
		// A 'char' is one rune.
		usize width = 0;
		next_rune(wrap_string(source), &width);
		token->end = source + width;
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
	case TT_OPERATOR:;  // Check for known operators:
		// Check which operators exists, and try and
		// match against the longest operators first.
		// e.g.  2+-3 becomes 2 + (-3), but if an operator '+-' exists,
		// then it becomes 2 +- 3.
		OperatorTable operators = ctx->operator_table;
		foreach (op, operators) {
			string op_substr = VIEW((byte *)source, 0, op->name.len);
			if (string_eq(op->name, op_substr)) {
				token->end = source + op_substr.len;
				goto return_token;
			} // Otherwise continue searching.
		}
		// No operator was found...
		if (tt == TT_IDENT) {  // Hence, it's just a regular identifier.
			while (tt == TT_NUMBER || tt == TT_IDENT)
				tt = character_type(*(++source));  // Consolidate characters.

			token->end = source;
			goto return_token;
		}

		// Otherwise, if we have an operator symbol, but it was not covered by
		// the previous check for known operators, then it does not exist.
		// Hence, we throw an error.
		// TODO: Throw proper lexer error.
		eprintln("Error: Operator does not exist.");
		return nil;
	default:
		return nil;
	}

return_token:;
	ctx->last_token_type = token->type;
	return token;
}

#endif
