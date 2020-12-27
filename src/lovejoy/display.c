#include "display.h"
#include <string.h>

#ifndef IMPLEMENTATION

byte *display_tt(TokenType tt)
{
	byte *repr = (byte *)malloc(16 * sizeof(byte));
	switch (tt) {
		case TT_NUMBER:   strcpy(repr,        "number"); break;
		case TT_STRING:   strcpy(repr,        "string"); break;
		case TT_CHAR:     strcpy(repr,     "character"); break;
		case TT_OPERATOR: strcpy(repr,      "operator"); break;
		case TT_IDENT:    strcpy(repr,    "identifier"); break;
		case TT_LPAREN:   strcpy(repr,    "left-paren"); break;
		case TT_RPAREN:   strcpy(repr,   "right-paren"); break;
		case TT_LBRACKET: strcpy(repr,  "left-bracket"); break;
		case TT_RBRACKET: strcpy(repr, "right-bracket"); break;
		case TT_LCURLY:   strcpy(repr,    "left-brace"); break;
		case TT_RCURLY:   strcpy(repr,   "right-brace"); break;
		case TT_TERM:     strcpy(repr,    "terminator"); break;
		case TT_NONE:     strcpy(repr,    "none-token"); break;
		default:          strcpy(repr,   "false-token"); break;
	}
	return repr;
}

byte *display_lexeme(const Lexeme *lexeme)
{
	usize span = lexeme_span(lexeme);
	byte *tt = display_tt(lexeme->type);
	usize tt_len = strlen(tt);

	byte *repr = malloc((span + tt_len + 8) * sizeof(byte));
	byte *start = repr;

	*repr++ = '(';
	*repr++ = ':';
	strcpy(repr, tt);
    free(tt);
	repr += tt_len;

	if (lexeme->type != TT_TERM) {
		*repr++ = ' ';
		*repr++ = '\'';
		strncpy(repr, lexeme->start, span);
		repr += span;
		*repr++ = '\'';
	}
	*repr++ = ')';
	*repr = '\0';

	return start;
}

#endif
