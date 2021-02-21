#pragma once

#include "common.h"
#include "lexer.h"

/// String representation of a token type.
/// @note Allocates memory.
string display_tt(TokenType);

/// String representation of a lexeme.
/// @note Allocates memory.
string display_lexeme(const Lexeme *);