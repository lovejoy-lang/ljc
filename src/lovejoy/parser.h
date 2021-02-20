//! @file parser.h
//! Incremental parsing of source, which in turns increments the lexer.

#pragma once

#include "lexer.h"
#include "hir.h"

/// Parser environment.
struct ParserContext {
	LexerContext lexer_ctx;
	bool inside_parentheses;
	HIR_Expr *last_expr;
};
unqualify(struct, ParserContext);

ParserContext NewParser();

HIR_Expr *parse_expr(ParserContext *, const string);

