parser grammar cqasm_parser;

options {
	tokenVocab = cqasm_lexer;
}

@parser::preinclude {
#include "v1x/cqasm-ast.hpp"
#include "v1x/cqasm-parse-helper.hpp"
using namespace cqasm::ast;
using namespace cqasm::v1x::ast;
}

// Actual grammar start.
root: program;

program: version statement*;

version: VERSION INTEGER_LITERAL (DOT INTEGER_LITERAL)?;

statement: qubits_statement | map_statement | var_statement | instruction;

qubits_statement: QUBITS INTEGER_LITERAL;

map_statement: MAP IDENTIFIER EQUAL expression;

var_statement: VAR IDENTIFIER COLON IDENTIFIER;

instruction: IDENTIFIER expression_list;

expression_list: expression (COMMA expression)?;

expression:
    INTEGER_LITERAL
    | FLOAT_LITERAL
    | IDENTIFIER
    | array_elements;

array_elements: IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET;
array_indices: array_index (COMMA array_index)?;
array_index: expression (COLON expression)?;

/*
* Things we are leaving out at the moment:
* - Function calls.
* - Matrix literals, string literals.
* - Multiple variable declarations in the same line.
* - Operators: unary, binary, ternary.
* - Semicolons don't start a new statement.
*/
