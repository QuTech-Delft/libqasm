%code requires { #include "qasm_ast.hpp" }

%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <vector>
    #include <string>
    #include "qasm_ast.hpp"
    int yylex(void);
    void yyerror (char const *);
    compiler::Bits bits_identified;
    compiler::NumericalIdentifiers buffer_indices;
    compiler::SubCircuits subcircuits_object;
    compiler::QasmRepresentation qasm_representation;
    std::string buffer_string;
%}


%union {
    int ival;
    double dval;
    char* sval;
    compiler::NumericalIdentifiers* idval;
    compiler::Qubits* qval;
    compiler::Bits* bval;
    compiler::Operation* oval;
    compiler::OperationsCluster* ocval;
}

%token <sval> NAME 
%token <ival> INTEGER
%token <dval> FLOAT
%token COMMA_SEPARATOR PARALLEL_SEPARATOR BRA KET DOT SBRA SKET CBRA CKET LS_SEP NEWLINE WS COLON COMMENT
%token <sval> ROTATIONS AXIS
%token QUBITS
%token <sval> SINGLE_QUBIT_GATES TWO_QUBIT_GATES CR TOFFOLI
%token <sval> CDASH NOT_TOKEN
%token <sval> MAPKEY PREP MEASURE MEASUREPARITY MEASUREALL
%token <sval> WAIT DISPLAY RESET_AVERAGING
%token QBITHEAD BITHEAD

%type <sval> single-qubit-gate;

%start qasm-file

%%

//# Describe the general structure of a qasm file
qasm-file : qubit-register line-separator circuits {qasm_representation.getSubCircuits() = subcircuits_object;}
          | comments qubit-register line-separator circuits {qasm_representation.getSubCircuits() = subcircuits_object;}
    ;
circuits : circuit 
           | circuits circuit
           | comments
           | circuits comments
    ;
circuit : subcircuit statements 
        | statements
        | WS subcircuit statements 
        | WS statements
    ;
subcircuit : DOT NAME { subcircuits_object.addSubCircuit( compiler::SubCircuit ($2,subcircuits_object.numberOfSubCircuits()) ); }
           | subcircuit BRA INTEGER KET { subcircuits_object.lastSubCircuit().numberIterations($3); }
    ;
statements : qasm-line 
           | subcircuit
           | qasm-line line-separator
           | subcircuit line-separator
           | statements line-separator qasm-line
           | statements line-separator subcircuit
    ;
comments : COMMENT 
         | comments COMMENT 
         | comments line-separator COMMENT 
         | comments COMMENT line-separator
         | comments line-separator
    ;
qasm-line : map-operation
          | measureall-operation
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | measure-parity-operation
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | regular-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | binary-controlled-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | parallel-operations
            {
                subcircuits_object.lastSubCircuit().addOperationsCluster( $1 );
            }
          | special-operations
            {
                compiler::Operation* serial_ops = $1;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | WS map-operation
          | WS measureall-operation
            {
                compiler::Operation* serial_ops = $2;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | WS measure-parity-operation
            {
                compiler::Operation* serial_ops = $2;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | WS regular-operations
            {
                compiler::Operation* serial_ops = $2;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | WS binary-controlled-operations
            {
                compiler::Operation* serial_ops = $2;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
          | WS parallel-operations
            {
                subcircuits_object.lastSubCircuit().addOperationsCluster( $2 );
            }
          | WS special-operations
            {
                compiler::Operation* serial_ops = $2;
                compiler::OperationsCluster* single_op_cluster = new compiler::OperationsCluster( serial_ops );
                subcircuits_object.lastSubCircuit().addOperationsCluster( single_op_cluster );
            }
    ;

//# We define the convenience strings, texts, numbers here....
%type <idval> indices numerical-identifiers numerical-identifier-list numerical-identifier-range;
line-separator : NEWLINE 
               | line-separator NEWLINE
    ;
indices : SBRA numerical-identifiers SKET {} 
    ;
numerical-identifiers : numerical-identifier-list {} 
                      | numerical-identifier-range {}
    ;
numerical-identifier-list : INTEGER {buffer_indices.addToVector($1);}
                          | numerical-identifier-list COMMA_SEPARATOR numerical-identifiers
    ;
numerical-identifier-range : INTEGER COLON INTEGER 
                             {
                                buffer_indices.addToVector($1,$3); 
                             }
                           | numerical-identifier-range COMMA_SEPARATOR numerical-identifiers
    ;


qubit-register : QUBITS WS INTEGER {qasm_representation.qubitRegister($3);}

//# We define the syntax for selecting the qubits/bits, either by a range or a list
%type <qval> qubit;
qubit : qubit-nomap {$$=$1;}
      | NAME
        {
            buffer_indices = qasm_representation.getMappedIndices( std::string($1), true );
            $$ = new compiler::Qubits (buffer_indices);
            buffer_indices.clear();
        } 
    ;
%type <qval> qubit-nomap;
qubit-nomap : QBITHEAD indices 
              {
                 buffer_indices.removeDuplicates();
                 $$ = new compiler::Qubits (buffer_indices);
                 buffer_indices.clear();
              }
    ;

//## Next we define a classical bit, which is required to perform control gate operations
%type <bval> bit;
bit :  bit-nomap 
    |  NAME
       {
           buffer_indices = qasm_representation.getMappedIndices( std::string($1), false );
           $$ = new compiler::Bits (buffer_indices);
           buffer_indices.clear();
       }
    ;
%type <bval> bit-nomap;
bit-nomap : BITHEAD indices
            {
                 buffer_indices.removeDuplicates();
                 $$ = new compiler::Bits (buffer_indices);
                 buffer_indices.clear();
            } 
    ;


//# Define the single qubit operation line
%type <oval> single-qubit-operation;
single-qubit-operation : single-qubit-gate WS qubit 
                         {
                            $$ = new compiler::Operation(buffer_string, *($3) );
                         }
                       | prep_measure-ops WS qubit 
                         {
                            $$ = new compiler::Operation(buffer_string, *($3) );
                         }
    ;
%type <oval> single-qubit-operation-args;
single-qubit-operation-args : parameterized-single-qubit-gate WS qubit COMMA_SEPARATOR FLOAT 
                              {
                                  $$ = new compiler::Operation(buffer_string, *($3) ,$5);
                              }
    ;
map-operation : MAPKEY WS qubit-nomap COMMA_SEPARATOR NAME 
                {
                    qasm_representation.addMappings(std::string($5), $3->getSelectedQubits(), true );
                }
              | MAPKEY WS bit-nomap COMMA_SEPARATOR NAME
                {
                    qasm_representation.addMappings(std::string($5), $3->getSelectedBits(), false );
                }
    ;
//## Define the single qubit operations/gates
single-qubit-gate : AXIS {buffer_string = std::string($1);} | SINGLE_QUBIT_GATES {buffer_string = std::string($1);} 
    ;
parameterized-single-qubit-gate : ROTATIONS {buffer_string = std::string($1);}
    ;
//# This is to define the state preparation/measurement
prep_measure-ops : PREP {buffer_string = std::string($1);} | MEASURE {buffer_string = std::string($1);}
    ;

%type <oval> measure-parity-operation;
measure-parity-operation : measure-parity-command WS qubit COMMA_SEPARATOR AXIS COMMA_SEPARATOR qubit COMMA_SEPARATOR AXIS
                           {
                               $$ = new compiler::Operation( buffer_string, *($3) , std::string($5,1) , *($7) , std::string($9,1) ) ;
                           }
    ;
measure-parity-command : MEASUREPARITY {buffer_string = std::string($1);}
%type <oval> measureall-operation;
measureall-operation : MEASUREALL {$$ = new compiler::Operation(std::string($1));}
    ;

//# Qubit-controlled operations
%type <oval> two-qubit-operation;
two-qubit-operation : two-qubit-gates WS qubit COMMA_SEPARATOR qubit
                      {
                          $$ = new compiler::Operation( buffer_string, *($3) , *($5) );
                      }
    ;
%type <oval> two-qubit-operation-args;
two-qubit-operation-args : two-qubit-gate-args WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
                           {
                              $$ = new compiler::Operation( buffer_string, *($3) , *($5), ($7) );
                           }
    ;
//## Define the two qubit gates
two-qubit-gates : TWO_QUBIT_GATES {buffer_string = std::string($1);}
    ;
two-qubit-gate-args : CR {buffer_string = std::string($1);}
    ;
//## Define the toffoli gate
%type <oval> toffoli-operation;
toffoli-operation : toffoli-gate WS qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                    {
                       $$ = new compiler::Operation( buffer_string, *($3) , *($5) , *($7) );
                    }
    ;
toffoli-gate : TOFFOLI {buffer_string = std::string($1);}
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
                                compiler::Operation* bit_single_qubit_operation = new compiler::Operation(buffer_string, *($6) );
                                bit_single_qubit_operation -> setControlBits( *($4) );
                                $$ = bit_single_qubit_operation;
                             }
    ;
%type <oval> bit-single-qubit-operation-args;    
bit-single-qubit-operation-args : CDASH parameterized-single-qubit-gate WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR FLOAT
                                  {
                                      compiler::Operation* bit_single_qubit_operation_args = new compiler::Operation(buffer_string, *($6) , $8);
                                      bit_single_qubit_operation_args -> setControlBits( *($4) );
                                      $$ = bit_single_qubit_operation_args;
                                  }
    ;
%type <oval> bit-two-qubit-operation;    
bit-two-qubit-operation : CDASH two-qubit-gates WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                          {
                              compiler::Operation* bit_two_qubit_operation = new compiler::Operation( buffer_string, *($6) , *($8) );
                              bit_two_qubit_operation -> setControlBits( *($4) );
                              $$ = bit_two_qubit_operation;
                          }
    ;
%type <oval> bit-two-qubit-operation-args;    
bit-two-qubit-operation-args : CDASH two-qubit-gate-args WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR INTEGER
                               {
                                  compiler::Operation* bit_two_qubit_operation_args = new compiler::Operation( buffer_string, *($6) , *($8), ($10) );
                                  bit_two_qubit_operation_args -> setControlBits( *($4) );
                                  $$ = bit_two_qubit_operation_args;
                               }
    ;
%type <oval> bit-toffoli-operation;    
bit-toffoli-operation : CDASH toffoli-gate WS bit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit COMMA_SEPARATOR qubit
                        {
                            compiler::Operation* bit_toffoli_operation = new compiler::Operation( buffer_string, *($6), *($8), *($10) );
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
    ;
%type <oval> all-valid-operations;
all-valid-operations : regular-operations
                     | binary-controlled-operations 
    ;
parallelizable-ops : all-valid-operations
                     {
                        compiler::OperationsCluster* parallel_ops = new compiler::OperationsCluster( $1 );
                        $$ = parallel_ops;
                     }
                   | parallelizable-ops PARALLEL_SEPARATOR all-valid-operations
                     {
                        $1 -> addParallelOperation( $3 );
                        $$ = $1;
                     }
    ;

//# Special operations
%type <oval> special-operations display-operation wait-operation reset-averaging-operation;
special-operations : display-operation | wait-operation | reset-averaging-operation
    ;
display-operation : DISPLAY 
                    {
                        $$ = new compiler::Operation( std::string($1) );
                    }
                  | DISPLAY WS bit
                    {
                        $$ = new compiler::Operation( std::string($1,7), *($3) );
                    }
    ;
wait-operation : WAIT WS INTEGER
                 {
                     $$ = new compiler::Operation( std::string($1,4), ($3) );
                 }
    ;
reset-averaging-operation : RESET_AVERAGING 
                          {
                              $$ = new compiler::Operation( std::string($1) );
                          }
                          | RESET_AVERAGING WS qubit
                            {
                                $$ = new compiler::Operation( std::string($1,15), *($3) );
                            }

    ;

%%

void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}