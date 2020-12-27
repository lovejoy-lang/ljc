#include "common.h"

/// Which way an operator groups parentheses when chained
/// (if possible to chain at all).
enum Associativity {
	ASSOC_LEFT,
	ASSOC_RIGHT,
	ASSOC_NEITHER
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
	Associativity assoc;
	Fixity fixity;
	byte name[8];  ///< No operators may be longer than 8 bytes.
	struct Operator *_next;  ///< @private
};
unqualify(struct, Operator);

/// All known operators, may grow while parsing.
/// @note Must be stored in order of operator name length first,
///       and operator fixity second (in order: PREFIX, POSTFIX, INFIX).
struct OperatorTable {
	Operator *operators;
	usize count;
};
unqualify(struct, OperatorTable);

/// Default operators that do not need to be declared in source.
OperatorTable DefaultOperators();

/// Construct a new operator, and add it to the operator table.
u0 new_operator(OperatorTable *, byte *, u16, Associativity, Fixity);

/// Get an operator given name and fixity, will return specific operator
/// with priority to fixity, in order of: PREFIX, POSTFIX, INFIX.
/// @note May return NULL when no such operator is found.
Operator *get_operator(OperatorTable *, byte *, Fixity);

inline
bool is_prefix(Operator* op)
{ return (op->fixity & PREFIX) != 0; }

inline
bool is_infix(Operator* op)
{ return (op->fixity & INFIX) != 0; }

inline
bool is_postfix(Operator* op)
{ return (op->fixity & POSTFIX) != 0; }
