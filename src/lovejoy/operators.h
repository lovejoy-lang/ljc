#include "common.h"

/// Which way an operator groups parentheses when chained
/// (if possible to chain at all).
enum Associativity {
	ASSOC_NEITHER = 0, ///< Operator may not be chained w/o parentheses.
	ASSOC_LEFT,
	ASSOC_RIGHT,
};
unqualify(enum, Associativity);

/// On what side of an expression may an operator be used.
enum Fixity {
	PREFIX  = 1 << 0,
	INFIX   = 1 << 1,
	POSTFIX = 1 << 2
};
unqualify(enum, Fixity);

/// Mathematical/notational properties of an operator.
struct Operator {
	u16 precedence;
	Associativity associativity;
	Fixity fixity;
	string name;  ///< No operators should be longer than 8 bytes.
};
unqualify(struct, Operator);

/// All known operators, may grow while parsing.
/// @note Must be stored in order of operator name length first,
///       and operator fixity second (in order: PREFIX, POSTFIX, INFIX).
newarray(OperatorTable, Operator);

/// Default operators that do not need to be declared in source.
OperatorTable DefaultOperators();

/// Construct a new operator, and add it to the operator table.
/// Will over-write any old operator that has the same name and fixity.
u0 new_operator(OperatorTable *, string, u16, Associativity, Fixity);

/// Get an operator given name and fixity, will return specific operator
/// with priority to fixity, in order of: PREFIX, POSTFIX, INFIX.
/// @note May return `nil` when no such operator is found.
Operator *get_operator(OperatorTable, string, Fixity);

inline
bool is_prefix(Operator* op)
{ return (op->fixity & PREFIX) != 0; }

inline
bool is_infix(Operator* op)
{ return (op->fixity & INFIX) != 0; }

inline
bool is_postfix(Operator* op)
{ return (op->fixity & POSTFIX) != 0; }
