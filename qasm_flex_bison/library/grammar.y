%define parse.error verbose

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { qasm_data* data }

%locations

%code requires {
    #include "qasm_ast.hpp"
    #include "qasm_data.hpp"
    typedef void* yyscan_t;
}

%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <vector>
    #include <string>
    #include "qasm_ast.hpp"
    #include "qasm_data.hpp"
    typedef void* yyscan_t;
%}

%union {
    int ival;
    double dval;
    char* sval;
    std::vector<double>* vecdval;
    compiler::NumericalIdentifiers* idval;
    compiler::Qubits* qval;
    compiler::Bits* bval;
    compiler::Operation* oval;
    compiler::OperationsCluster* ocval;
}

%{
    int yylex(YYSTYPE* lvalp, YYLTYPE* yylloc, yyscan_t scanner);
    void yyerror(YYLTYPE* yylloc, yyscan_t scanner, qasm_data* data, const char* x);
%}

%token QASM_VERSION
%token <sval> NAME 
%token <ival> INTEGER
%token <dval> FLOAT
%token COMMA_SEPARATOR PARALLEL_SEPARATOR BRA KET DOT SBRA SKET CBRA CKET NEWLINE WS COLON QUOTE COMMENT
%token <sval> ROTATIONS AXIS
%token QUBITS
%token <sval> SINGLE_QUBIT_GATES TWO_QUBIT_GATES CR CRK TOFFOLI
%token <sval> CDASH NOT_TOKEN
%token <sval> MAPKEY PREP MEASURE MEASUREPARITY MEASUREALL
%token <sval> WAIT DISPLAY RESET_AVERAGING LOAD_STATE QUOTED_STRING
%token <sval> ERROR_MODEL_KEY ERROR_MODEL
%token QBITHEAD BITHEAD

%start qasm-file

%%

//# Describe the general structure of a qasm file
qasm-file : qasm_version NEWLINE qubit-register body 
            {
              data->qasm_representation.getSubCircuits() = data->subcircuits_object;
            }
          | qasm_version NEWLINE comment qubit-register body 
            {
              data->qasm_representation.getSubCircuits() = data->subcircuits_object;
            }
          | qasm_version NEWLINE qubit-register 
            {
              data->qasm_representation.getSubCircuits() = data->subcircuits_object;
            }
          | qasm_version NEWLINE comment qubit-register 
            {
              data->qasm_representation.getSubCircuits() = data->subcircuits_object;
            }
    ;
qasm_version : QASM_VERSION WS FLOAT
               {
                  data->qasm_representation.versionNumber($3);
               }
             | comment QASM_VERSION WS FLOAT
               {
                  data->qasm_representation.versionNumber($4);
               } 
    ;
body : bodyline 
     | body bodyline
    ;
bodyline : statement
         | WS statement
         | NEWLINE
    ;
statement  : qasm-line 
           | subcircuit-definition
           | COMMENT
    ;
comment : COMMENT
        | comment COMMENT
    ;
subcircuit-definition : DOT NAME
                        { 
                            data->subcircuits_object.addSubCircuit( compiler::SubCircuit ($2, data->subcircuits_object.numberOfSubCircuits(), @$.first_line) ); 
                        }
                      | DOT NAME BRA INTEGER KET
                        {
                            data->subcircuits_object.addSubCircuit( compiler::SubCircuit ($2, data->subcircuits_object.numberOfSubCircuits(), @$.first_line) ); 
                            data->subcircuits_object.lastSubCircuit().numberIterations($4); 
                        }
    ;
qasm-line : map-operation
          | error-model
          | measureall-operation
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops, @$.first_line );
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | measure-parity-operation
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops, @$.first_line );
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | regular-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops, @$.first_line );
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | binary-controlled-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops, @$.first_line );
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | parallel-operations
            {
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( $1 );
            }
          | special-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops, @$.first_line );
                data->subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
    ;

//# We define the convenience strings, texts, numbers here....
%type <dval> int-or-float;
int-or-float : FLOAT | INTEGER {$$ = static_cast<double> ($1);}
;
%type <idval> indices numerical-identifiers numerical-identifier-list numerical-identifier-range;
indices : SBRA numerical-identifiers SKET {} 
    ;
numerical-identifiers : numerical-identifier-list
                      | numerical-identifier-range
                      | numerical-identifiers COMMA_SEPARATOR numerical-identifier-list
                      | numerical-identifiers COMMA_SEPARATOR numerical-identifier-range
    ;
numerical-identifier-list : INTEGER {data->buffer_indices.addToVector($1);}
    ;
numerical-identifier-range : INTEGER COLON INTEGER 
                             {
                                data->buffer_indices.addToVector($1,$3); 
                             }
    ;
qubit-register : QUBITS WS INTEGER 
                 {
                    data->qasm_representation.qubitRegister($3);
                 } 
    ;
%type <vecdval> error-model-args;
error-model-args : int-or-float
                   {
                     std::vector<double> arguments;
                     arguments.push_back($1);
                     $$ = new std::vector<double> (arguments);
                   }
                 | error-model-args COMMA_SEPARATOR int-or-float
                   {
                     auto argument_iterated = $1;
                     argument_iterated->push_back($3);
                     $$ = new std::vector<double> (*(argument_iterated));
                   }
    ;
error-model : ERROR_MODEL_KEY WS ERROR_MODEL COMMA_SEPARATOR error-model-args
              {
                  data->qasm_representation.setErrorModel( std::string($3), *($5) );
              }  
    ;

//# We define the syntax for selecting the qubits/bits, either by a range or a list
%type <qval> qubit;
qubit : qubit-nomap {$$=$1;}
      | NAME
        {
            data->buffer_indices = data->qasm_representation.getMappedIndices( std::string($1), true, @$.first_line );
            $$ = new compiler::Qubits (data->buffer_indices);
            data->buffer_indices.clear();
        } 
    ;
%type <qval> qubit-nomap;
qubit-nomap : QBITHEAD indices 
              {
                 data->buffer_indices.removeDuplicates();
                 $$ = new compiler::Qubits (data->buffer_indices);
                 data->buffer_indices.clear();
              }
    ;

//## Next we define a classical bit, which is required to perform control gate operations
%type <bval> bit;
bit :  bit-nomap 
    |  NAME
       {
           data->buffer_indices = data->qasm_representation.getMappedIndices( std::string($1), false, @$.first_line );
           $$ = new compiler::Bits (data->buffer_indices);
           data->buffer_indices.clear();
       }
    ;
%type <bval> bit-nomap;
bit-nomap : BITHEAD indices
            {
                 data->buffer_indices.removeDuplicates();
                 $$ = new compiler::Bits (data->buffer_indices);
                 data->buffer_indices.clear();
            } 
    ;

//# Define the single qubit operation line
%type <oval> single-qubit-operation;
single-qubit-operation : single-qubit-gate WS qubit 
                         {
                            $$ = new compiler::Operation(data->buffer_string, *($3) );
                         }
                       | prep_measure-ops WS qubit 
                         {
                            $$ = new compiler::Operation(data->buffer_string, *($3) );
                         }
                       | single-qubit-gate WS qubit COMMA_SEPARATOR matrix-arguments
                         {
                            compiler::Operation U_op(data->buffer_string, *($3) );
                            U_op.setUMatrixElements(*($5));
                            $$ = new compiler::Operation(U_op);
                         }
    ;
%type <vecdval> matrix-arguments;
matrix-arguments : SBRA int-or-float COMMA_SEPARATOR int-or-float
                   COMMA_SEPARATOR int-or-float COMMA_SEPARATOR int-or-float
                   COMMA_SEPARATOR int-or-float COMMA_SEPARATOR int-or-float
                   COMMA_SEPARATOR int-or-float COMMA_SEPARATOR int-or-float SKET
                   {
                       std::vector<double> arguments = {$2, $4, $6, $8, $10, $12, $14, $16};
                       $$ = new std::vector<double> (arguments);
                   }
    ;
%type <oval> single-qubit-operation-args;
single-qubit-operation-args : parameterized-single-qubit-gate WS qubit COMMA_SEPARATOR int-or-float 
                              {
                                  $$ = new compiler::Operation(data->buffer_string, *($3) ,$5);
                              }
    ;
map-operation : MAPKEY WS qubit-nomap COMMA_SEPARATOR NAME 
                {
                    data->qasm_representation.addMappings(std::string($5), $3->getSelectedQubits(), true );
                }
              | MAPKEY WS bit-nomap COMMA_SEPARATOR NAME
                {
                    data->qasm_representation.addMappings(std::string($5), $3->getSelectedBits(), false );
                }
    ;

//## Define the single qubit operations/gates
%type <sval> single-qubit-gate;
single-qubit-gate : AXIS {data->buffer_string = std::string($1);} | SINGLE_QUBIT_GATES {data->buffer_string = std::string($1);} 
    ;
parameterized-single-qubit-gate : ROTATIONS {data->buffer_string = std::string($1);}
    ;

//# This is to define the state preparation/measurement
prep_measure-ops : PREP {data->buffer_string = std::string($1);} | MEASURE {data->buffer_string = std::string($1);}
    ;
%type <oval> measure-parity-operation;
measure-parity-operation : measure-parity-command WS qubit COMMA_SEPARATOR AXIS COMMA_SEPARATOR qubit COMMA_SEPARATOR AXIS
                           {
                               $$ = new compiler::Operation( data->buffer_string, *($3) , std::string($5,1) , *($7) , std::string($9,1) ) ;
                           }
    ;
measure-parity-command : MEASUREPARITY {data->buffer_string = std::string($1);}
%type <oval> measureall-operation;
measureall-operation : MEASUREALL {$$ = new compiler::Operation(std::string($1,11));}
    ;

//# Qubit-controlled operations
%type <oval> two-qubit-operation;
two-qubit-operation : two-qubit-gates WS qubit COMMA_SEPARATOR qubit
                      {
                          $$ = new compiler::Operation( data->buffer_string, *($3) , *($5) );
                      }
    ;
%type <oval> two-qubit-operation-args;
two-qubit-operation-args : two-qubit-gate-args WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR FLOAT
                           {
                              $$ = new compiler::Operation( data->buffer_string, *($3) , *($5), ($7) );
                           }
                         | two-qubit-gate-args WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
                           {
                              $$ = new compiler::Operation( data->buffer_string, *($3) , *($5), ($7) );
                           }
    ;

//## Define the two qubit gates
two-qubit-gates : TWO_QUBIT_GATES {data->buffer_string = std::string($1);}
    ;
two-qubit-gate-args : CR {data->buffer_string = std::string($1);}
                    | CRK {data->buffer_string = std::string($1);}
    ;

//## Define the toffoli gate
%type <oval> toffoli-operation;
toffoli-operation : toffoli-gate WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                    {
                       $$ = new compiler::Operation( data->buffer_string, *($3) , *($5) , *($7) );
                    }
    ;
toffoli-gate : TOFFOLI {data->buffer_string = std::string($1);}
    ;

//## Define a superset of all general "normal" operations
%type <oval> regular-operations;
regular-operations : single-qubit-operation 
                   | single-qubit-operation-args
                   | two-qubit-operation 
                   | two-qubit-operation-args
                   | toffoli-operation 
    ;
%type <oval> binary-controlled-operations;
binary-controlled-operations : bit-single-qubit-operation 
                                   | bit-single-qubit-operation-args 
                                   | bit-two-qubit-operation 
                                   | bit-two-qubit-operation-args 
                                   | bit-toffoli-operation 
                                   | negate-binary-operation
    ;
%type <oval> bit-single-qubit-operation;    
bit-single-qubit-operation : CDASH single-qubit-gate WS bit COMMA_SEPARATOR qubit
                             {
                                compiler::Operation* bit_single_qubit_operation = new compiler::Operation(data->buffer_string, *($6) );
                                bit_single_qubit_operation -> setControlBits( *($4) );
                                $$ = bit_single_qubit_operation;
                             }
    ;
%type <oval> bit-single-qubit-operation-args;    
bit-single-qubit-operation-args : CDASH parameterized-single-qubit-gate WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR int-or-float
                                  {
                                      compiler::Operation* bit_single_qubit_operation_args = new compiler::Operation(data->buffer_string, *($6) , $8);
                                      bit_single_qubit_operation_args -> setControlBits( *($4) );
                                      $$ = bit_single_qubit_operation_args;
                                  }
    ;
%type <oval> bit-two-qubit-operation;    
bit-two-qubit-operation : CDASH two-qubit-gates WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                          {
                              compiler::Operation* bit_two_qubit_operation = new compiler::Operation( data->buffer_string, *($6) , *($8) );
                              bit_two_qubit_operation -> setControlBits( *($4) );
                              $$ = bit_two_qubit_operation;
                          }
    ;
%type <oval> bit-two-qubit-operation-args;    
bit-two-qubit-operation-args : CDASH two-qubit-gate-args WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
                               {
                                  compiler::Operation* bit_two_qubit_operation_args = new compiler::Operation( data->buffer_string, *($6) , *($8), ($10) );
                                  bit_two_qubit_operation_args -> setControlBits( *($4) );
                                  $$ = bit_two_qubit_operation_args;
                               }
    ;
%type <oval> bit-toffoli-operation;    
bit-toffoli-operation : CDASH toffoli-gate WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                        {
                            compiler::Operation* bit_toffoli_operation = new compiler::Operation( data->buffer_string, *($6), *($8), *($10) );
                            bit_toffoli_operation -> setControlBits( *($4) );
                            $$ = bit_toffoli_operation;
                        }
    ;
%type <oval> negate-binary-operation;    
negate-binary-operation : NOT_TOKEN WS bit
                          {
                              compiler::Operation* negate_binary_operation = new compiler::Operation( std::string($1,3) );
                              negate_binary_operation -> setControlBits( *($3) );
                              $$ = negate_binary_operation;
                          }
    ;

//# Parallel execution
%type <ocval> parallel-operations parallelizable-ops;
parallel-operations : CBRA parallelizable-ops CKET { $$ = $2; }
                    | CBRA WS parallelizable-ops CKET { $$ = $3; }
                    | CBRA parallelizable-ops WS CKET { $$ = $2; }
                    | CBRA WS parallelizable-ops WS CKET { $$ = $3; }
    ;
%type <oval> all-valid-operations;
all-valid-operations : regular-operations
                     | binary-controlled-operations 
    ;
parallelizable-ops : all-valid-operations
                     {
                        compiler::OperationsCluster* parallel_ops = new compiler::OperationsCluster( $1, @$.first_line );
                        $$ = parallel_ops;
                     }
                   | parallelizable-ops PARALLEL_SEPARATOR all-valid-operations
                     {
                        $1 -> addParallelOperation( $3 );
                        $$ = $1;
                     }
    ;

//# Special operations
%type <oval> special-operations display-operation wait-operation reset-averaging-operation load-state-operation;
special-operations : display-operation | wait-operation | reset-averaging-operation | load-state-operation
    ;
display-operation : DISPLAY NEWLINE
                    {
                        $$ = new compiler::Operation( std::string($1) );
                    }
                  | DISPLAY WS
                    {
                        $$ = new compiler::Operation( std::string($1) );
                    }
                  | DISPLAY WS bit
                    {
                        $$ = new compiler::Operation( std::string($1), *($3) );
                    }
    ;
wait-operation : WAIT WS INTEGER
                 {
                     $$ = new compiler::Operation( std::string($1,4), ($3) );
                 }
    ;
reset-averaging-operation : RESET_AVERAGING WS
                            {
                                $$ = new compiler::Operation( std::string($1,15) );
                            }
                          | RESET_AVERAGING NEWLINE
                            {
                                $$ = new compiler::Operation( std::string($1,15) );
                            }
                          | RESET_AVERAGING WS qubit
                            {
                                $$ = new compiler::Operation( std::string($1,15), *($3) );
                            }

    ;
load-state-operation : LOAD_STATE QUOTED_STRING
                            {
                                $$ = new compiler::Operation( std::string($1), std::string($2) );
                            }
    ;

%%
void yyerror(YYLTYPE* yylloc, yyscan_t scanner, qasm_data* data, const char* x)
{
    std::string base_error_message(x);
    std::string entire_error_message = base_error_message + " | Line: " + std::to_string(yylloc->first_line);
    throw std::runtime_error(entire_error_message);
}
