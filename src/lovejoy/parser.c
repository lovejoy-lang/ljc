#include "parser.h"

ParserContext NewParser()
{
	const LexerContext lexer = NewLexer();  //< Likely to be replaced.
	ParserContext ctx = {
		.lexer_ctx = lexer,
		.inside_parentheses = false,
		.last_expr = nil
	};

	return ctx;
}

HIR_Expr *parse_expr(ParserContext *ctx, const string source)
{
	const byte *stream = UNWRAP(source);
	Lexeme *token = nil;  //< The current token.

	token = lex(&ctx->lexer_ctx, stream);
	stream = token->end;

	UNUSED(token);
	return nil;
}
