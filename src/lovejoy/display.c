#include "display.h"
#include <string.h>
#include <lovejoy/utf.h>

#ifndef IMPLEMENTATION


string display_tt(TokenType tt)
{
	const byte *repr;
	switch (tt) {
		case TT_NUMBER:   repr =        "number"; break;
		case TT_STRING:   repr =        "string"; break;
		case TT_CHAR:     repr =     "character"; break;
		case TT_OPERATOR: repr =      "operator"; break;
		case TT_IDENT:    repr =    "identifier"; break;
		case TT_LPAREN:   repr =    "left-paren"; break;
		case TT_RPAREN:   repr =   "right-paren"; break;
		case TT_LBRACKET: repr =  "left-bracket"; break;
		case TT_RBRACKET: repr = "right-bracket"; break;
		case TT_LCURLY:   repr =    "left-brace"; break;
		case TT_RCURLY:   repr =   "right-brace"; break;
		case TT_TERM:     repr =    "terminator"; break;
		case TT_NONE:     repr =    "none-token"; break;
		default:          repr =   "false-token"; break;
	}
	return wrap_string(repr);
}

string display_lexeme(const Lexeme *lexeme)
{
	string tt = display_tt(lexeme->type);
	byte *repr;
	usize len;

	if (lexeme->type != TT_TERM) {
		string substr = lexeme_substring(lexeme);
		repr = emalloc(substr.len + tt.len + 16, sizeof(byte));
		len = sprintf(repr, ":%s ‘%s’", UNWRAP(tt), UNWRAP(substr));
		free(UNWRAP(substr));
	} else {
		repr = emalloc(tt.len + 16, sizeof(byte));
		len = sprintf(repr, ":%s", UNWRAP(tt));
	}

	repr[len] = '\0';
	return wrap_string(repr);
}

#endif
