#include "operators.h"

#ifndef IMPLEMENTATION

static const string OP_DISAM  = STRING("::");
static const string OP_CONCAT = STRING("++");
static const string OP_RSHIFT = STRING(">>");
static const string OP_LSHIFT = STRING("<<");
static const string OP_MAPTO  = STRING("->");
static const string OP_CMPGE  = STRING(">=");
static const string OP_CMPLE  = STRING("<=");
static const string OP_CMPEQ  = STRING("==");
static const string OP_CMPNE  = STRING("/=");
static const string OP_MATCH  = STRING("=>");
static const string OP_FIELD  = STRING(".");
static const string OP_TIMES  = STRING("*");
static const string OP_RATIO  = STRING("/");
static const string OP_MOD    = STRING("%");
static const string OP_PLUS   = STRING("+");
static const string OP_MINUS  = STRING("-");
static const string OP_COLON  = STRING(":");
static const string OP_CMPGT  = STRING(">");
static const string OP_CMPLT  = STRING("<");
static const string OP_EQUAL  = STRING("=");
static const string OP_COMMA  = STRING(",");

OperatorTable DefaultOperators()
{
	OperatorTable table = AEMPTY(OperatorTable);

	// Two bytes.
	new_operator(&table, OP_DISAM, 190, ASSOC_LEFT,    INFIX);  // ::
	new_operator(&table, OP_CONCAT, 60, ASSOC_LEFT,    INFIX);  // ++
	new_operator(&table, OP_RSHIFT, 50, ASSOC_LEFT,    INFIX);  // >>
	new_operator(&table, OP_LSHIFT, 50, ASSOC_LEFT,    INFIX);  // <<
	new_operator(&table, OP_MAPTO,  40, ASSOC_RIGHT,   INFIX);  // ->
	new_operator(&table, OP_CMPGE,  35, ASSOC_LEFT,    INFIX);  // >=
	new_operator(&table, OP_CMPLE,  35, ASSOC_LEFT,    INFIX);  // <=
	new_operator(&table, OP_CMPEQ,  30, ASSOC_LEFT,    INFIX);  // ==
	new_operator(&table, OP_CMPNE,  30, ASSOC_LEFT,    INFIX);  // /=
	new_operator(&table, OP_MATCH,  15, ASSOC_NEITHER, INFIX);  // =>

	// One byte.
	new_operator(&table, OP_FIELD, 200, ASSOC_LEFT,    INFIX);  // .
	new_operator(&table, OP_TIMES,  70, ASSOC_LEFT,    INFIX);  // *
	new_operator(&table, OP_RATIO,  70, ASSOC_LEFT,    INFIX);  // /
	new_operator(&table, OP_MOD,    70, ASSOC_LEFT,    INFIX);  // %
	new_operator(&table, OP_PLUS,   60, ASSOC_LEFT,    INFIX);  // +
	new_operator(&table, OP_MINUS,  60, ASSOC_LEFT,    INFIX);  // -
	new_operator(&table, OP_COLON,  35, ASSOC_NEITHER, INFIX);  // :
	new_operator(&table, OP_CMPGT,  35, ASSOC_LEFT,    INFIX);  // >
	new_operator(&table, OP_CMPLT,  35, ASSOC_LEFT,    INFIX);  // <
	new_operator(&table, OP_EQUAL,  20, ASSOC_RIGHT,   INFIX);  // =
	new_operator(&table, OP_COMMA,  10, ASSOC_RIGHT,   INFIX);  // ,

	return table;
}

u0 new_operator(OperatorTable *table, const string name,
                u16 prec, Associativity assoc, Fixity fixity)
{
	Operator op = {
		.precedence = prec,
		.associativity = assoc,
		.fixity = fixity,
		.name = name
	};

	// TODO: Return a lexical error, i.e. error propagation.
	assert(name.len <= 8);

	// TODO: Search through operators, inserting in order of name-length.
	// Operators with same name, but different fixity, are in order of:
	//   PREFIX -> POSTFIX -> INFIX.

	// For now, naive push. (TODO: `insert(..)').
	push(table, &op, sizeof(Operator));

	return UNIT;
}

#endif
