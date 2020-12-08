#pragma once

#include "common.h"
#include "lexer.h"

/// String representation of a token type.
/// @note Allocates memory.
byte *display_tt(TokenType);

/// String representation of a lexeme.
/// @note Allocates memory.
byte *display_lexeme(const Lexeme *);
