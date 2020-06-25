%define api.pure full
%locations

%code requires {
    #include <memory>
    #include <cstdio>
    #include <cstdint>
    #include "cqasm-ast.hpp"
    #include "cqasm-parse-helper.hpp"
    using namespace cqasm::ast;
    typedef void* yyscan_t;
}

%code {
    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, cqasm::parser::ParseHelper &helper, const char* msg);
}

%code top {
    #define ADD_SOURCE_LOCATION(v)                          \
        v->set_annotation(cqasm::parser::SourceLocation(    \
            helper.filename,                                \
            yyloc.first_line,                               \
            yyloc.first_column,                             \
            yyloc.last_line,                                \
            yyloc.last_column))


    #define NEW(v, T)           \
        v = new T();            \
        ADD_SOURCE_LOCATION(v)

    #define FROM(t, s)                                                          \
        t = s;                                                                  \
        {                                                                       \
            auto *loc = t->get_annotation_ptr<cqasm::parser::SourceLocation>(); \
            if (!loc) {                                                         \
                ADD_SOURCE_LOCATION(t);                                         \
            } else {                                                            \
                loc->expand_to_include(yyloc.first_line, yyloc.first_column);   \
                loc->expand_to_include(yyloc.last_line, yyloc.last_column);     \
            }                                                                   \
        }

}

%param { yyscan_t scanner }
%parse-param { cqasm::parser::ParseHelper &helper }

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
    Subcircuit      *sub;
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
%type <unop> UnaryOp
%type <biop> BinaryOp
%type <expr> Expression
%type <expl> ExpressionList
%type <idxi> IndexItem
%type <idxr> IndexRange
%type <idxe> IndexEntry
%type <idxl> IndexList
%type <adat> AnnotationName AnnotationData
%type <inst> Instruction AnnotInstr
%type <bun>  SLParInstrList CBParInstrList
%type <map>  Mapping
%type <sub>  Subcircuit
%type <stmt> Statement AnnotStatement
%type <stms> StatementList
%type <vers> Version
%type <prog> Program

/* Whitespace management */
%token NEWLINE

/* Version statement */
%token VERSION

/* Keywords */
%token QUBITS
%token MAP
%token CDASH

/* Numeric literals */
%token <str> INT_LITERAL
%token <str> FLOAT_LITERAL
%token BAD_NUMBER

/* String and JSON literals */
%token STRING_OPEN STRING_CLOSE
%token JSON_OPEN JSON_CLOSE
%token <str> STRBUILD_APPEND STRBUILD_ESCAPE

/* Matrix literals */
%token MAT_OPEN MAT_CLOSE

/* Identifiers */
%token <str> IDENTIFIER

/* Multi-character operators */
%token POWER

/* Error marker tokens */
%token BAD_CHARACTER END_OF_FILE

/* Associativity rules for static expressions. The lowest precedence level
comes first. NOTE: expression precedence must match the values in
operators.[ch]pp for correct pretty-printing! */
%left ',' ':'                                /* SIMD/SGMQ indexation */
%left '+' '-'                                /* Addition/subtraction */
%left '*' '/'                                /* Multiplication/division */
%left POWER                                  /* Power */
%right UMINUS                                /* Negation */
%left '(' '['                                /* Function call, indexation */

/* In a single-line parallel statement, possibly containing only a single gate,
annotations apply to the gate, not the bundle. Therefore '@' has greater
priority than '|' */
%left '|'
%left '@'
%nonassoc BUNDLE

/* Misc. Yacc directives */
%error-verbose
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
FunctionCall    : Identifier '(' ExpressionList ')' %prec '('                   { NEW($$, FunctionCall); $$->name.set_raw($1); $$->arguments.set_raw($3); }
                ;

/* Array/register indexation. */
Index           : Expression '[' IndexList ']'                                  { NEW($$, Index); $$->expr.set_raw($1); $$->indices.set_raw($3); }
                ;

/* Math operations, evaluated by the parser. */
UnaryOp         : '-' Expression %prec UMINUS                                   { NEW($$, Negate); $$->expr.set_raw($2); }
                ;

BinaryOp        : Expression POWER Expression                                   { NEW($$, Power);    $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '*' Expression                                     { NEW($$, Multiply); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '/' Expression                                     { NEW($$, Divide);   $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '+' Expression                                     { NEW($$, Add);      $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
                | Expression '-' Expression                                     { NEW($$, Subtract); $$->lhs.set_raw($1); $$->rhs.set_raw($3); }
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
                | '(' Expression ')'                                            { FROM($$, $2); }
                | error                                                         { NEW($$, ErroneousExpression); }
                ;

/* List of one or more expressions. */
ExpressionList  : ExpressionList ',' Expression                                 { FROM($$, $1); $$->items.add_raw($3); }
                | Expression %prec ','                                          { NEW($$, ExpressionList); $$->items.add_raw($1); }
                ;

/* Indexation modes. */
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

/* The information caried by an annotation or pragma statement. */
AnnotationName  : Identifier '.' Identifier                                     { NEW($$, AnnotationData); $$->interface.set_raw($1); $$->operation.set_raw($1); }
                ;

AnnotationData  : AnnotationName                                                { FROM($$, $1); $$->operands.set_raw(new ExpressionList()); }
                | AnnotationName '(' ')'                                        { FROM($$, $1); $$->operands.set_raw(new ExpressionList()); }
                | AnnotationName '(' ExpressionList ')'                         { FROM($$, $1); $$->operands.set_raw($3); }
                ;

/* Instructions. Note that this is NOT directly a statement grammatically;
they are always part of a bundle. */
Instruction     : Identifier                                                    { NEW($$, Instruction); $$->name.set_raw($1); $$->operands.set_raw(new ExpressionList()); }
                | Identifier ExpressionList                                     { NEW($$, Instruction); $$->name.set_raw($1); $$->operands.set_raw($2); }
                | CDASH Identifier Expression                                   { NEW($$, Instruction); $$->name.set_raw($2); $$->condition.set_raw($3); $$->operands.set_raw(new ExpressionList()); }
                | CDASH Identifier Expression ',' ExpressionList                { NEW($$, Instruction); $$->name.set_raw($2); $$->condition.set_raw($3); $$->operands.set_raw($5); }
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

/* Subcircuit header statement. */
Subcircuit      : '.' Identifier                                                { NEW($$, Subcircuit); $$->name.set_raw($2); }
                | '.' Identifier '(' Expression ')'                             { NEW($$, Subcircuit); $$->name.set_raw($2); $$->iterations.set_raw($4); }
                ;

/* Any of the supported statements. */
Statement       : Mapping                                                       { FROM($$, $1); }
                | Subcircuit                                                    { FROM($$, $1); }
                | SLParInstrList                                                { FROM($$, $1); }
                | '{' OptNewline CBParInstrList OptNewline '}'                  { FROM($$, $3); }
                | error                                                         { NEW($$, ErroneousStatement); }
                ;

/* Statement with annotations attached to it. */
AnnotStatement  : AnnotStatement '@' AnnotationData                             { FROM($$, $1); $$->annotations.add_raw($3); }
                | Statement                                                     { FROM($$, $1); }
                ;

/* List of one or more statements. */
StatementList   : StatementList Newline AnnotStatement                          { FROM($$, $1); $$->items.add_raw($3); }
                | AnnotStatement                                                { NEW($$, StatementList); $$->items.add_raw($1); }
                ;

/* Version. */
Version         : Version '.' IntegerLiteral                                    { FROM($$, $1); $$->items.push_back($3->value); delete $3; }
                | IntegerLiteral                                                { NEW($$, Version); $$->items.push_back($1->value); delete $1; }
                ;

/* Program. */
Program         : OptNewline VERSION Version Newline
                    QUBITS Expression Newline
                    StatementList OptNewline                                    { NEW($$, Program); $$->version.set_raw($3); $$->num_qubits.set_raw($6); $$->statements.set_raw($8); }
                | OptNewline VERSION Version Newline
                    QUBITS Expression OptNewline                                { NEW($$, Program); $$->version.set_raw($3); $$->num_qubits.set_raw($6); $$->statements.set_raw(new StatementList()); }
                ;

/* Toplevel. */
Root            : Program                                                       { helper.result.root.set_raw($1); }
                | error                                                         { helper.result.root.set_raw(new ErroneousProgram()); }
                ;

%%

void yyerror(YYLTYPE* yyllocp, yyscan_t unused, cqasm::parser::ParseHelper &helper, const char* msg) {
    (void)unused;
    std::ostringstream sb;
    sb << helper.filename
       << ":"  << yyllocp->first_line
       << ":"  << yyllocp->first_column
       << ": " << msg;
    helper.push_error(sb.str());
}
