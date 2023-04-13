%define api.pure full
%define api.prefix {cqasm_v1_}
%locations

%code requires {
    /**
     * \file
     * Parser, generated by Bison.
     */
    #include <memory>
    #include <cstdio>
    #include <cstdint>
    #include "cqasm-annotations.hpp"
    #include "v1/cqasm-ast.hpp"
    #include "v1/cqasm-parse-helper.hpp"
    using namespace cqasm::v1::ast;
    typedef void* yyscan_t;

    #define YYSTYPE CQASM_V1_STYPE
    #define YYLTYPE CQASM_V1_LTYPE

}

%code {
    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper, const char* msg);
}

%code top {

    /**
     * Attaches a source location annotation object to the given node pointer.
     */
    #define ADD_SOURCE_LOCATION(v)                              \
        v->set_annotation(cqasm::annotations::SourceLocation(   \
            helper.filename,                                    \
            yyloc.first_line,                                   \
            yyloc.first_column,                                 \
            yyloc.last_line,                                    \
            yyloc.last_column))

    /**
     * Constructs an empty, new node of type T and places it into v. v is almost
     * always $$ (the result location), but it can't be made part of the
     * definition because preprocessing happens after bison's substitution.
     */
    #define NEW(v, T)           \
        v = new T();            \
        ADD_SOURCE_LOCATION(v)

    /**
     * Moves previously constructed node s into t, where s is almost always one
     * of bison's input locations ($1, $2, etc) and t is almost always $$. The
     * purpose of this over just assigning t = s is that it extends the source
     * location annotation of the object to the extents of whatever rule is
     * being matched.
     */
    #define FROM(t, s)                                                                  \
        t = s;                                                                          \
        {                                                                               \
            auto *loc = t->get_annotation_ptr<cqasm::annotations::SourceLocation>();    \
            if (!loc) {                                                                 \
                ADD_SOURCE_LOCATION(t);                                                 \
            } else {                                                                    \
                loc->expand_to_include(yyloc.first_line, yyloc.first_column);           \
                loc->expand_to_include(yyloc.last_line, yyloc.last_column);             \
            }                                                                           \
        }

}

%param { yyscan_t scanner }
%parse-param { cqasm::v1::parser::ParseHelper &helper }

/* YYSTYPE union */
%union {
    char           	*str;
    IntegerLiteral  *ilit;
    FloatLiteral    *flit;
    MatrixLiteral   *mat;
    StringBuilder   *strb;
    StringLiteral   *slit;
    JsonLiteral     *jlit;
    Identifier      *idnt;
    FunctionCall    *func;
    Index           *indx;
    UnaryOp         *unop;
    BinaryOp        *biop;
    TernaryCond     *tcnd;
    Expression      *expr;
    ExpressionList  *expl;
    IndexItem       *idxi;
    IndexRange      *idxr;
    IndexEntry      *idxe;
    IndexList       *idxl;
    AnnotationData  *adat;
    Instruction     *inst;
    Bundle          *bun;
    Mapping         *map;
    Variables       *vars;
    Subcircuit      *sub;
    Assignment      *asgn;
    IfElse          *ifel;
    ForLoop         *forl;
    ForeachLoop     *fore;
    WhileLoop       *whil;
    RepeatUntilLoop *repu;
    BreakStatement  *brk;
    ContinueStatement *cont;
    Statement       *stmt;
    StatementList   *stms;
    Version         *vers;
    Program         *prog;
};

/* Typenames for nonterminals */
%type <ilit> IntegerLiteral
%type <flit> FloatLiteral
%type <mat>  MatrixRows MatrixLiteral
%type <strb> StringBuilder
%type <slit> StringLiteral
%type <jlit> JsonLiteral
%type <idnt> Identifier
%type <func> FunctionCall
%type <indx> Index
%type <indx> IndexNP
%type <unop> UnaryOp
%type <unop> UnaryOpNP
%type <biop> BinaryOp
%type <biop> BinaryOpNP
%type <tcnd> TernaryOp
%type <tcnd> TernaryOpNP
%type <expr> Expression
%type <expr> ExpressionNP
%type <expl> ExpressionList
%type <expl> ExpressionListNP
%type <idxi> IndexItem
%type <idxr> IndexRange
%type <idxe> IndexEntry
%type <idxl> IndexList
%type <adat> AnnotationName AnnotationData
%type <inst> Instruction AnnotInstr
%type <bun>  SLParInstrList CBParInstrList
%type <map>  Mapping
%type <vars> Variable VariableBody
%type <sub>  Subcircuit
%type <asgn> Assignment OptAssignment
%type <ifel> IfElse
%type <forl> ForLoop
%type <fore> ForeachLoop
%type <whil> WhileLoop
%type <repu> RepeatUntilLoop
%type <brk>  Break
%type <cont> Continue
%type <stmt> Statement AnnotStatement
%type <stms> StatementList Statements SubStatements
%type <vers> Version
%type <prog> Program

/* FIXME: no %destructor for the new-allocated nodes above, so the parser may
    leak, especially when it recovers from an error. */

/* Whitespace management */
%token NEWLINE

/* Version statement */
%token VERSION

/* Keywords */
%token QUBITS
%token MAP
%token VAR
%token CDASH
%token COND

/* cQASM 1.2 keywords */
%token IF
%token ELSE
%token FOR
%token FOREACH
%token WHILE
%token REPEAT
%token UNTIL
%token CONTINUE
%token BREAK
%token SET

/* Numeric literals */
%token <str> INT_LITERAL
%token <str> FLOAT_LITERAL
%token BAD_NUMBER

/* String and JSON literals */
%token STRING_OPEN STRING_CLOSE
%token JSON_OPEN JSON_CLOSE
%token <str> STRBUILD_APPEND STRBUILD_ESCAPE

/* Identifiers */
%token <str> IDENTIFIER

/* Multi-character operators */
%token POWER
%token INT_DIV
%token LOGIC_OR
%token LOGIC_AND
%token LOGIC_XOR
%token CMP_GE
%token CMP_LE
%token CMP_EQ
%token CMP_NE
%token SHL
%token ARITH_SHR
%token LOGIC_SHR
%token ELLIPSIS

/* Error marker tokens */
%token BAD_CHARACTER END_OF_FILE

/* Associativity rules for static expressions. The lowest precedence level
comes first. */
%left ',' ':'                                /* SIMD/SGMQ indexation */
%left ELLIPSIS                               /* Foreach range */
%right '?'                                   /* Ternary conditional */
%left LOGIC_OR                               /* Logical OR */
%left LOGIC_XOR                              /* Logical XOR */
%left LOGIC_AND                              /* Logical AND */
%left '|'                                    /* Bitwise OR */
%left '^'                                    /* Bitwise XOR */
%left '&'                                    /* Bitwise AND */
%left CMP_EQ CMP_NE                          /* Equalities */
%left '>' '<' CMP_GE CMP_LE                  /* Inequalities */
%left SHL ARITH_SHR LOGIC_SHR                /* Bit shifts */
%left '+' '-'                                /* Addition/subtraction */
%left '*' '/' INT_DIV '%'                    /* Multiplication/division */
%right POWER                                 /* Power */
%right UMINUS UCOMP UNOT                     /* Negation */
%left '(' '['                                /* Function call, indexation */

/* In a single-line parallel statement, possibly containing only a single gate,
annotations apply to the gate, not the bundle. Therefore '@' has greater
priority than '|' */
%left '@'

/* Misc. Yacc directives */
%define parse.error verbose
%start Root

%%

/* One or more newlines. */
Newline         : Newline NEWLINE
                | NEWLINE
                ;

/* Zero or more newlines. */
OptNewline      : Newline
                |
                ;

/* Integer literals. */
IntegerLiteral  : INT_LITERAL                                                   { NEW($$, IntegerLiteral); $$->value = std::strtol($1, nullptr, 0); std::free($1); }
                ;

/* Floating point literals. */
FloatLiteral    : FLOAT_LITERAL                                                 { NEW($$, FloatLiteral); $$->value = std::strtod($1, nullptr); std::free($1); }
                ;

/* Matrix syntax. */
MatrixRows      : MatrixRows Newline ExpressionList                             { FROM($$, $1); $$->rows.add_raw($3); }
                | ExpressionList                                                { NEW($$, MatrixLiteral); $$->rows.add_raw($1); }
                ;

MatrixLiteral   : '[' OptNewline MatrixRows OptNewline ']'                      { FROM($$, $3); }
                | '[' OptNewline ']'                                            { NEW($$, MatrixLiteral); $$->rows.add_raw(new ExpressionList()); }
                ;

/* String builder. This accumulates JSON/String data, mostly
character-by-character. */
StringBuilder   : StringBuilder STRBUILD_APPEND                                 { FROM($$, $1); $$->push_string(std::string($2)); std::free($2); }
                | StringBuilder STRBUILD_ESCAPE                                 { FROM($$, $1); $$->push_escape(std::string($2)); std::free($2); }
                |                                                               { NEW($$, StringBuilder); }
                ;

/* String literal. */
StringLiteral   : STRING_OPEN StringBuilder STRING_CLOSE                        { NEW($$, StringLiteral); $$->value = $2->stream.str(); delete $2; }
                ;

/* JSON literal. */
JsonLiteral     : JSON_OPEN StringBuilder JSON_CLOSE                            { NEW($$, JsonLiteral); $$->value = $2->stream.str(); delete $2; }
                ;

/* Identifiers. */
Identifier      : IDENTIFIER                                                    { NEW($$, Identifier); $$->name = std::string($1); std::free($1); }
                ;

/* Function calls. */
FunctionCall    : Identifier '(' ')' %prec '('                                  { NEW($$, FunctionCall); $$->name.set_raw($1); $$->arguments.set_raw(new ExpressionList()); }
                | Identifier '(' ExpressionList ')' %prec '('                   { NEW($$, FunctionCall); $$->name.set_raw($1); $$->arguments.set_raw($3); }
                ;

/* Array/register indexation. */
IndexItem       : Expression                                                    { NEW($$, IndexItem); $$->index.set_raw($1); }
                ;

IndexRange      : Expression ':' Expression                                     { NEW($$, IndexRange); $$->first.set_raw($1); $$->last.set_raw($3); }
                ;

IndexEntry      : IndexItem                                                     { FROM($$, $1); }
                | IndexRange                                                    { FROM($$, $1); }
                ;

IndexList       : IndexList ',' IndexEntry                                      { FROM($$, $1); $$->items.add_raw($3); }
                | IndexEntry                                                    { NEW($$, IndexList); $$->items.add_raw($1); }
                ;

Index           : Expression '[' IndexList ']'                                  { NEW($$, Index); $$->expr.set_raw($1); $$->indices.set_raw($3); }
                ;

IndexNP         : ExpressionNP '[' IndexList ']'                                { NEW($$, Index); $$->expr.set_raw($1); $$->indices.set_raw($3); }
                ;

/* Operators. */
UnaryOp         : '-' Expression %prec UMINUS                                   { NEW($$, Negate); $$->expr.set_raw($2); }
                | '~' Expression %prec UCOMP                                    { NEW($$, BitwiseNot); $$->expr.set_raw($2); }
                | '!' Expression %prec UNOT                                     { NEW($$, LogicalNot); $$->expr.set_raw($2); }
                ;

BinaryOp        : Expression POWER     Expression                               { NEW($$, Power);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '*'       Expression                               { NEW($$, Multiply);        $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '/'       Expression                               { NEW($$, Divide);          $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression INT_DIV   Expression                               { NEW($$, IntDivide);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '%'       Expression                               { NEW($$, Modulo);          $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '+'       Expression                               { NEW($$, Add);             $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '-'       Expression                               { NEW($$, Subtract);        $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression SHL       Expression                               { NEW($$, ShiftLeft);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression ARITH_SHR Expression                               { NEW($$, ShiftRightArith); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression LOGIC_SHR Expression                               { NEW($$, ShiftRightLogic); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression CMP_EQ    Expression                               { NEW($$, CmpEq);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression CMP_NE    Expression                               { NEW($$, CmpNe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '>'       Expression                               { NEW($$, CmpGt);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression CMP_GE    Expression                               { NEW($$, CmpGe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '<'       Expression                               { NEW($$, CmpLt);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression CMP_LE    Expression                               { NEW($$, CmpLe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '&'       Expression                               { NEW($$, BitwiseAnd);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '^'       Expression                               { NEW($$, BitwiseXor);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '|'       Expression                               { NEW($$, BitwiseOr);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression LOGIC_AND Expression                               { NEW($$, LogicalAnd);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression LOGIC_XOR Expression                               { NEW($$, LogicalXor);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression LOGIC_OR  Expression                               { NEW($$, LogicalOr);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                ;

TernaryOp       : Expression '?' Expression ':' Expression                      { NEW($$, TernaryCond);     $$->cond.set_raw($1); $$->if_true.set_raw($3); $$->if_false.set_raw($5); }
                ;

/* Operators with "no pipe". This is used to disambiguate between the | */
/* operator and bundle notation. */
UnaryOpNP       : '-' ExpressionNP %prec UMINUS                                 { NEW($$, Negate); $$->expr.set_raw($2); }
                | '~' ExpressionNP %prec UCOMP                                  { NEW($$, BitwiseNot); $$->expr.set_raw($2); }
                | '!' ExpressionNP %prec UNOT                                   { NEW($$, LogicalNot); $$->expr.set_raw($2); }
                ;

BinaryOpNP      : ExpressionNP POWER     ExpressionNP                           { NEW($$, Power);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '*'       ExpressionNP                           { NEW($$, Multiply);        $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '/'       ExpressionNP                           { NEW($$, Divide);          $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP INT_DIV   ExpressionNP                           { NEW($$, IntDivide);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '%'       ExpressionNP                           { NEW($$, Modulo);          $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '+'       ExpressionNP                           { NEW($$, Add);             $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '-'       ExpressionNP                           { NEW($$, Subtract);        $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP SHL       ExpressionNP                           { NEW($$, ShiftLeft);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP ARITH_SHR ExpressionNP                           { NEW($$, ShiftRightArith); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP LOGIC_SHR ExpressionNP                           { NEW($$, ShiftRightLogic); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP CMP_EQ    ExpressionNP                           { NEW($$, CmpEq);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP CMP_NE    ExpressionNP                           { NEW($$, CmpNe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '>'       ExpressionNP                           { NEW($$, CmpGt);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP CMP_GE    ExpressionNP                           { NEW($$, CmpGe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '<'       ExpressionNP                           { NEW($$, CmpLt);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP CMP_LE    ExpressionNP                           { NEW($$, CmpLe);           $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '&'       ExpressionNP                           { NEW($$, BitwiseAnd);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP '^'       ExpressionNP                           { NEW($$, BitwiseXor);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP LOGIC_AND ExpressionNP                           { NEW($$, LogicalAnd);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP LOGIC_XOR ExpressionNP                           { NEW($$, LogicalXor);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | ExpressionNP LOGIC_OR  ExpressionNP                           { NEW($$, LogicalOr);       $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                ;

TernaryOpNP     : ExpressionNP '?' ExpressionNP ':' ExpressionNP                { NEW($$, TernaryCond);     $$->cond.set_raw($1); $$->if_true.set_raw($3); $$->if_false.set_raw($5); }
                ;

/* Supported types of expressions. */
Expression      : IntegerLiteral                                                { FROM($$, $1); }
                | FloatLiteral                                                  { FROM($$, $1); }
                | MatrixLiteral                                                 { FROM($$, $1); }
                | StringLiteral                                                 { FROM($$, $1); }
                | JsonLiteral                                                   { FROM($$, $1); }
                | Identifier                                                    { FROM($$, $1); }
                | FunctionCall                                                  { FROM($$, $1); }
                | Index                                                         { FROM($$, $1); }
                | UnaryOp                                                       { FROM($$, $1); }
                | BinaryOp                                                      { FROM($$, $1); }
                | TernaryOp                                                     { FROM($$, $1); }
                | '(' Expression ')'                                            { FROM($$, $2); }
                | error                                                         { NEW($$, ErroneousExpression); }
                ;

ExpressionNP    : IntegerLiteral                                                { FROM($$, $1); }
                | FloatLiteral                                                  { FROM($$, $1); }
                | MatrixLiteral                                                 { FROM($$, $1); }
                | StringLiteral                                                 { FROM($$, $1); }
                | JsonLiteral                                                   { FROM($$, $1); }
                | Identifier                                                    { FROM($$, $1); }
                | FunctionCall                                                  { FROM($$, $1); }
                | IndexNP                                                       { FROM($$, $1); }
                | UnaryOpNP                                                     { FROM($$, $1); }
                | BinaryOpNP                                                    { FROM($$, $1); }
                | TernaryOpNP                                                   { FROM($$, $1); }
                | '(' Expression ')'                                            { FROM($$, $2); }
                | error                                                         { NEW($$, ErroneousExpression); }
                ;

/* List of one or more expressions. */
ExpressionList  : ExpressionList ',' Expression                                 { FROM($$, $1); $$->items.add_raw($3); }
                | Expression %prec ','                                          { NEW($$, ExpressionList); $$->items.add_raw($1); }
                ;

ExpressionListNP: ExpressionListNP ',' ExpressionNP                             { FROM($$, $1); $$->items.add_raw($3); }
                | ExpressionNP %prec ','                                        { NEW($$, ExpressionList); $$->items.add_raw($1); }
                ;

/* The information caried by an annotation or pragma statement. */
AnnotationName  : Identifier '.' Identifier                                     { NEW($$, AnnotationData); $$->interface.set_raw($1); $$->operation.set_raw($3); }
                ;

AnnotationData  : AnnotationName                                                { FROM($$, $1); $$->operands.set_raw(new ExpressionList()); }
                | AnnotationName '(' ')'                                        { FROM($$, $1); $$->operands.set_raw(new ExpressionList()); }
                | AnnotationName '(' ExpressionList ')'                         { FROM($$, $1); $$->operands.set_raw($3); }
                ;

/* Instructions. Note that this is NOT directly a statement grammatically;
they are always part of a bundle. */
Instruction     : Identifier                                                    {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw($1);
                                                                                    $$->operands.set_raw(new ExpressionList());
                                                                                }
                | Identifier ExpressionListNP                                   {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw($1);
                                                                                    $$->operands.set_raw($2);
                                                                                }
                | SET ExpressionNP '=' ExpressionNP                             {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw(new Identifier());
                                                                                    $$->name->name = "set";
                                                                                    $$->operands.set_raw(new ExpressionList());
                                                                                    $$->operands->items.add_raw($2);
                                                                                    $$->operands->items.add_raw($4);
                                                                                }
                | CDASH Identifier ExpressionNP                                 {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw($2);
                                                                                    $$->condition.set_raw($3);
                                                                                    $$->operands.set_raw(new ExpressionList());
                                                                                }
                | CDASH Identifier ExpressionNP ',' ExpressionListNP            {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw($2);
                                                                                    $$->condition.set_raw($3);
                                                                                    $$->operands.set_raw($5);
                                                                                }
                | COND '(' Expression ')' Identifier ExpressionListNP           {
                                                                                    NEW($$, Instruction);
                                                                                    $$->name.set_raw($5);
                                                                                    $$->condition.set_raw($3);
                                                                                    $$->operands.set_raw($6);
                                                                                }
                ;

/* Instructions are not statements (because there can be multiple bundled
instructions per statement) but can be annotated, so they need their own
annotation rule. */
AnnotInstr      : AnnotInstr '@' AnnotationData                                 { FROM($$, $1); $$->annotations.add_raw($3); }
                | Instruction                                                   { FROM($$, $1); }
                ;

/* Single-line bundling syntax. */
SLParInstrList  : SLParInstrList '|' AnnotInstr                                 { FROM($$, $1); $$->items.add_raw($3); }
                | AnnotInstr %prec '|'                                          { NEW($$, Bundle); $$->items.add_raw($1); }
                ;

/* Multi-line bundling syntax. */
CBParInstrList  : CBParInstrList Newline SLParInstrList                         { FROM($$, $1); $$->items.extend($3->items); delete $3; }
                | SLParInstrList                                                { FROM($$, $1); }
                ;

/* Map statement, aliasing some expression with an identifier. */
Mapping         : MAP Expression ',' Identifier                                 { NEW($$, Mapping); $$->expr.set_raw($2); $$->alias.set_raw($4); }
                | MAP Identifier '=' Expression                                 { NEW($$, Mapping); $$->alias.set_raw($2); $$->expr.set_raw($4); }
                ;

/* Variable declaration. */
VariableBody    : Identifier ':' Identifier                                     { NEW($$, Variables); $$->names.add_raw($1); $$->typ.set_raw($3); }
                | Identifier ',' VariableBody                                   { FROM($$, $3); $$->names.add_raw($1); }
                ;

Variable        : VAR VariableBody                                              { FROM($$, $2); }
                ;

/* Subcircuit header statement. */
Subcircuit      : '.' Identifier                                                { NEW($$, Subcircuit); $$->name.set_raw($2); }
                | '.' Identifier '(' Expression ')'                             { NEW($$, Subcircuit); $$->name.set_raw($2); $$->iterations.set_raw($4); }
                ;

/* cQASM 1.2 statements. */
SubStatements   : '{' OptNewline StatementList OptNewline '}'                   { FROM($$, $3); }
                | '{' OptNewline '}'                                            { NEW($$, StatementList); }
                ;

Assignment      : Expression '=' Expression                                     { NEW($$, Assignment); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                ;

OptAssignment   : Assignment                                                    { FROM($$, $1); }
                |                                                               { $$ = nullptr; }
                ;

IfElse          : IF '(' Expression ')' SubStatements                           {
                                                                                    NEW($$, IfElse);
                                                                                    $$->branches.add_raw(new IfElseBranch());
                                                                                    $$->branches[0]->condition.set_raw($3);
                                                                                    $$->branches[0]->body.set_raw($5);
                                                                                }
                | IF '(' Expression ')' SubStatements ELSE IfElse               {
                                                                                    FROM($$, $7);
                                                                                    $$->branches.add_raw(new IfElseBranch(), 0);
                                                                                    $$->branches[0]->condition.set_raw($3);
                                                                                    $$->branches[0]->body.set_raw($5);
                                                                                }
                | IF '(' Expression ')' SubStatements ELSE SubStatements        {
                                                                                    NEW($$, IfElse);
                                                                                    $$->branches.add_raw(new IfElseBranch());
                                                                                    $$->branches[0]->condition.set_raw($3);
                                                                                    $$->branches[0]->body.set_raw($5);
                                                                                    $$->otherwise.set_raw($7);
                                                                                }
                ;

ForLoop         : FOR '(' OptAssignment NEWLINE Expression NEWLINE
                    OptAssignment ')' SubStatements                             {
                                                                                    NEW($$, ForLoop);
                                                                                    if ($3) $$->initialize.set_raw($3);
                                                                                    $$->condition.set_raw($5);
                                                                                    if ($7) $$->update.set_raw($7);
                                                                                    $$->body.set_raw($9);
                                                                                }
                ;

ForeachLoop     : FOREACH '(' Expression '=' Expression ELLIPSIS Expression
                    ')' SubStatements                                           {
                                                                                    NEW($$, ForeachLoop);
                                                                                    $$->lhs.set_raw($3);
                                                                                    $$->frm.set_raw($5);
                                                                                    $$->to.set_raw($7);
                                                                                    $$->body.set_raw($9);
                                                                                }
                ;

WhileLoop       : WHILE '(' Expression ')' SubStatements                        { NEW($$, WhileLoop); $$->condition.set_raw($3); $$->body.set_raw($5); }
                ;

RepeatUntilLoop : REPEAT SubStatements UNTIL '(' Expression ')'                 { NEW($$, RepeatUntilLoop); $$->body.set_raw($2); $$->condition.set_raw($5); }
                ;

Continue        : CONTINUE                                                      { NEW($$, ContinueStatement); }
                ;

Break           : BREAK                                                         { NEW($$, BreakStatement); }
                ;

/* Any of the supported statements. */
Statement       : Mapping                                                       { FROM($$, $1); }
                | Variable                                                      { FROM($$, $1); }
                | Subcircuit                                                    { FROM($$, $1); }
                | SLParInstrList                                                { FROM($$, $1); }
                | '{' OptNewline CBParInstrList OptNewline '}'                  { FROM($$, $3); }
                | IfElse                                                        { FROM($$, $1); }
                | ForLoop                                                       { FROM($$, $1); }
                | ForeachLoop                                                   { FROM($$, $1); }
                | WhileLoop                                                     { FROM($$, $1); }
                | RepeatUntilLoop                                               { FROM($$, $1); }
                | Continue                                                      { FROM($$, $1); }
                | Break                                                         { FROM($$, $1); }
                | error                                                         { NEW($$, ErroneousStatement); }
                ;

/* FIXME: statement recovery fails even on simple things like
    "this is an invalid statement" */

/* Statement with annotations attached to it. */
AnnotStatement  : AnnotStatement '@' AnnotationData                             { FROM($$, $1); $$->annotations.add_raw($3); }
                | Statement                                                     { FROM($$, $1); }
                ;

/* List of one or more statements. */
StatementList   : StatementList Newline AnnotStatement                          { FROM($$, $1); $$->items.add_raw($3); }
                | AnnotStatement                                                { NEW($$, StatementList); $$->items.add_raw($1); }
                ;

/* List of zero or more statements preceded by a newline. */
Statements      : Newline StatementList OptNewline                              { FROM($$, $2); }
                | OptNewline                                                    { NEW($$, StatementList); }
                ;

/* Version. */
Version         : Version '.' IntegerLiteral                                    { FROM($$, $1); $$->items.push_back($3->value); delete $3; }
                | IntegerLiteral                                                { NEW($$, Version); $$->items.clear(); $$->items.push_back($1->value); delete $1; }
                ;

/* Program. */
Program         : OptNewline VERSION Version Newline
                    QUBITS Expression
                    Statements                                                  { NEW($$, Program); $$->version.set_raw($3); $$->num_qubits.set_raw($6); $$->statements.set_raw($7); }
                | OptNewline VERSION Version
                    Statements                                                  { NEW($$, Program); $$->version.set_raw($3); $$->statements.set_raw($4); }
                ;

/* Toplevel. */
Root            : Program                                                       { helper.result.root.set_raw($1); }
                | error                                                         { helper.result.root.set_raw(new ErroneousProgram()); }
                ;

%%

void yyerror(YYLTYPE* yyllocp, yyscan_t unused, cqasm::v1::parser::ParseHelper &helper, const char* msg) {
    (void)unused;
    std::ostringstream sb;
    sb << helper.filename
       << ":"  << yyllocp->first_line
       << ":"  << yyllocp->first_column
       << ": " << msg;
    helper.push_error(sb.str());
}
