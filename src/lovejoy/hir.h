//! @file hir.h
//! High-Level Intermediate Representation.

#pragma once

#include "lexer.h"

/// Location within source-code.
struct Site {
	byte *source;
	usize begin_offs;
	usize end_offs;
	usize lineno;
	byte *line;
};
unqualify(struct, Site);

enum NodeKind {
	CALL_NODE = 0,
	BLOCK_NODE,
	RETURN_NODE,
	FEILD_NODE,
	DECLARATION_NODE,
	SUBSCRIPT_NODE,
	DISAMB_NODE,  ///< e.g. `a`, `::b`, `A::B::c`, `std::xyz`, etc.
	MODULE_NODE,
	IMPORT_NODE,
	CAST_NODE,
	SIGNATURE_NODE,
	LITERAL_NODE
};
unqualify(enum, NodeKind);

struct HIR_Literal;      unqualify(struct, HIR_Literal);
struct HIR_FunctionCall; unqualify(struct, HIR_FunctionCall);
struct HIR_ScopeBlock;   unqualify(struct, HIR_ScopeBlock);
struct HIR_Return;       unqualify(struct, HIR_Return);
struct HIR_Branch;       unqualify(struct, HIR_Branch);
struct HIR_FieldAccess;  unqualify(struct, HIR_FeildAccess);
struct HIR_Subscript;    unqualify(struct, HIR_Subscript);
struct HIR_Disamb;       unqualify(struct, HIR_Disamb);
struct HIR_Declaration;  unqualify(struct, HIR_Declaration);
struct HIR_Module;       unqualify(struct, HIR_Module);
struct HIR_Import;       unqualify(struct, HIR_Import);
struct HIR_Signature;    unqualify(struct, HIR_Signature);
struct HIR_Cast;         unqualify(struct, HIR_Cast);

/// Most general/top-level sort of AST entry.
struct HIR_Expr {
	Site site;
	NodeKind kind;
	union {
		HIR_FunctionCall *call_expr;
		HIR_ScopeBlock *block_expr;
		HIR_Return *return_expr;
		HIR_Branch *jump_expr;
		HIR_FeildAccess *field_expr;
		HIR_Subscript *subscipt_expr;
		HIR_Disamb *disamb_expr;
		HIR_Declaration *declaration_expr;
		HIR_Module *module_expr;
		HIR_Import *import_expr;
		HIR_Signature *signature_expr;
		HIR_Cast *cast_expr;
		HIR_Literal *literal_expr;
	};
};
unqualify(struct, HIR_Expr);

/// Array literals holding number of nodes.
newarray(HIR_Array, HIR_Expr);
newarray(HIR_Tuple, HIR_Expr);

enum LiteralKind {
	ARRAY_LITERAL = 0,
	TUPLE_LITERAL,
	STRING_LITERAL,
	INTEGER_LITERAL,
	FLOATING_LITERAL,
	BYTE_LITEAL,
	BOOL_LITERAL,
	CODEPOINT_LITERAL
};
unqualify(enum, LiteralKind);

struct HIR_Literal {
	LiteralKind kind;
	union {
		HIR_Array array_literal;
		HIR_Tuple tuple_literal;
		symbol string_literal;
		u128 integer_literal;  ///< 128-bit integers are largest supported by LJ.
		symbol floating_literal;  ///< May have suffix (e.g. 5.29f).
		byte byte_literal;
		bool bool_literal;
		rune codepoint_literal;
	};
};
