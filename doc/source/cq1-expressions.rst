Expressions
===========

All operands in cQASM 1.x can in principle be arbitrary expressions, as long as
the type system (explained in the next section) considers them valid within
context. The primary use cases of expressions in cQASM are indexing into the
qubit and measurement bit registers, writing things more intuitively through
constant propagation (for instance by scaling a matrix using scalar-matrix
multiplication to make it unitary), and combining measurement results together
for gate conditions. This chapter lists the available types of expressions.

Number literals
---------------

cQASM 1.x supports unsigned integer literals in decimal format only. Floating
point literals use a period (``.``) decimal separator. The integer part may be
omitted if zero, but the fractional part may not be; for example, ``.1`` is
legal, but ``0.`` is not. Power-of-ten exponent notation is supported; for
example, ``1.0e3`` is the same as ``1000.0``.

Negative numbers are expressed by applying the unary negation operator on a
positive number literal (this of course looks the same as a negative number
literal, but this is how it works internally). Complex numbers are expressed
using sums and products of real numbers and the imaginary unit (``im``, a named
constant).

Matrix literals
---------------

Matrix literals are lists of (constant) numbers enclosed within square brackets
(``[`` and ``]``). Separation of elements along the minor axis is done using
commas (``,``), while separation of elements along the major axis is done using
newlines or semicolons. An optional newline (or semicolon) may appear between
the opening bracket and the first element, and/or between the last element and
the closing bracket.

.. code:: text

    [1, 2, 3]     # 3-entry row vector
    [1; 2; 3]     # 3-entry column vector
    [1, 2; 3, 4]  # 2x2 matrix
    [
        1, 2
        3, 4
    ]  # alternative notation for the same 2x2 matrix

cQASM 1.x only supports real- and complex-valued matrices.

String and JSON literals
------------------------

String literals are enclosed in double quotation marks on either end (``"``) as
in C. Newlines embedded in the string become part of the string unless escaped
with a backslash (``\``). Escape sequences exist for tab (``\t``), newline
(``\n``), single quote (``\'``), double quote (``\"``) and backslash (``\\``).

JSON objects can be represented as-is, except the outermost curly brackets must
be replaced by ``{|`` and ``|}`` to disambiguate between bundle notation and a
JSON literal. The contents of the JSON object support everything that the JSON
specification supports, but libqasm makes no attempt to parse or validate the
contents as such.

Note that these types of literals are primarily intended for use in annotations
and meta-instructions such as a print statement for simulation, as they don't
map cleanly to hardware.

Mappings, references, and named literals
----------------------------------------

Any word consisting of letters, numbers, and/or underscores (``_``) and not
starting with a number that appears where an expression is expected and isn't
a function name, may be any of the following things:

- a user-specified mapping, defined by a preceding ``map`` statement;
- a reference to the qubit register, defined by the ``qubits`` statement to
  the letter ``q``;
- a reference to the measurement bit register, defined by the ``qubits``
  statement to the letter ``b``;
- a reference to a user-specific variable, defined by a ``var`` statement (if
  supported by the target);
- one of the following named constants:
  - ``pi`` for 3.1415...,
  - ``eu`` for 2.7182...,
  - ``im`` for the imaginary unit,
  - ``true`` and ``false`` for booleans, and
  - ``x``, ``y``, and ``z`` for axes;
- a target-specific reference.

Internally, all these things are actually mappings. The latter two are called
"initial mappings;" they are defined using libqasm's API during construction of
the parser object.

Indexation and slicing
----------------------

The qubit and measurement bit registers behave like one-dimensional vectors
that can be indexed and sliced. The notation is ``<register>[<index-list>]``,
where ``<index-list>>`` is a comma-separated list of integer indices or ranges.
Range notation specifically consists of a colon-separated (``:``) pair of
integers representing the lower and upper limit of an inclusive, ascending
range; ``1:4`` is for instance equivalent to ``1,2,3,4``.

The result of the index operator behaves as a new reference to a qubit or bit
register, but of the size equal to the number of indices in the slice (it can
thus also be larger than the original register if indices are repeated) and
with the elements in a potentially different order. The indices must be
constant, and indexation/slicing is only supported on qubit and measurement
bit registers (not on matrices).

Functions
---------

Functions have the form ``<name>(<comma-separated-arguments>)``; you should
recognize this notation from basically every other programming language out
there. They can be used for constant propagation, or (when supported by the
target) to describe classical operation on data. cQASM 1.x does not provide a
way to define functions in the cQASM file; instead, they are defined by the
program or library using libqasm prior to parsing. This program or library is
free to define whatever functions they want, but a default set is provided by
libqasm as a sort of standard library. This library currently includes the
following:

- ``sqrt(x)``: returns the square root of ``x``, where ``x`` is a real or
  complex number.
- ``exp(x)``: returns e\ :sup:`x`, where ``x`` is a real or complex number.
- ``log(x)``: returns the natural logarithm of ``x``, where ``x`` is a real or
  complex number.
- ``sin(x)``: returns the sine of ``x``, where ``x`` is a real or complex number.
- ``cos(x)``: returns the cosine of ``x``, where ``x`` is a real or complex number.
- ``tan(x)``: returns the tangent of ``x``, where ``x`` is a real or complex number.
- ``asin(x)``: returns the arcsine of ``x``, where ``x`` is a real or complex number.
- ``acos(x)``: returns the arccosine of ``x``, where ``x`` is a real or complex number.
- ``atan(x)``: returns the arctangent of ``x``, where ``x`` is a real or complex number.
- ``sinh(x)``: returns the hyperbolic sine of ``x``, where ``x`` is a real or complex number.
- ``cosh(x)``: returns the hyperbolic cosine of ``x``, where ``x`` is a real or complex number.
- ``tanh(x)``: returns the hyperbolic tangent of ``x``, where ``x`` is a real or complex number.
- ``asinh(x)``: returns the hyperbolic arcsine of ``x``, where ``x`` is a real or complex number.
- ``acosh(x)``: returns the hyperbolic arccosine of ``x``, where ``x`` is a real or complex number.
- ``atanh(x)``: returns the hyperbolic arctangent of ``x``, where ``x`` is a real or complex number.
- ``abs(x)``: returns the absolute value of ``x``, where ``x`` is an integer or
  a real number.
- ``complex(r, i)``: returns the complex number with real part ``r`` and
  imaginary part ``i``, where ``r`` and ``i`` are real numbers.
- ``polar(norm, arg)``: returns the complex number with magnitude ``norm`` and
  angle ``arg`` (in radians), where ``norm`` and ``arg`` are real numbers.
- ``real(c)``: returns the real part of the given complex number.
- ``imag(c)``: returns the imaginary part of the given complex number.
- ``arg(c)``: returns the argument of the given complex number in radians.
- ``norm(c)``: returns the norm of the given complex number.
- ``conj(c)``: returns the conjugate of the given complex number.

Functions operating on matrices are not yet part of the standard library,
because there's quite a bit of manual labor involved with implementing those.
Please make an issue in the libqasm GitHub project if you would like to have
them.

cQASM 1.0 does not support functions evaluated at runtime; only constant
propagation is supported. cQASM 1.1 adds support for such dynamic expressions,
but the default set of functions is still constant-propagation-only.

Unary, binary, and ternary operators
------------------------------------

cQASM 1.x supports various unary and binary operators, as well as the ternary
conditional operator from C. Internally, these are just syntactic sugar for
functions named ``operator`` followed by the prefix or infix symbols
representing the operator. The full list, along with precedence rules, is
listed below. Deviations from the C standard are specified.

+---------------+-------------------------------+------------------------------+
| Operator      | Description                   | Precedence and associativity |
+===============+===============================+==============================+
| ``-x``        | Negation                      | 1, ←                         |
+---------------+-------------------------------+                              |
| ``!x``        | Logical NOT                   |                              |
+---------------+-------------------------------+                              |
| ``~x``        | Bitwise NOT                   |                              |
+---------------+-------------------------------+------------------------------+
| ``x ** y``    | Exponentiation (Python)       | 2, ←                         |
+---------------+-------------------------------+------------------------------+
| ``x * y``     | Multiplication                | 3, →                         |
+---------------+-------------------------------+                              |
| ``x / y``     | True division (Python)        |                              |
+---------------+-------------------------------+                              |
| ``x // y``    | Flooring division (Python)    |                              |
+---------------+-------------------------------+                              |
| ``x % y``     | Modulo (Python)               |                              |
+---------------+-------------------------------+------------------------------+
| ``x + y``     | Addition                      | 4, →                         |
+---------------+-------------------------------+                              |
| ``x - y``     | Subtraction                   |                              |
+---------------+-------------------------------+------------------------------+
| ``x << y``    | Shift left                    | 5, →                         |
+---------------+-------------------------------+                              |
| ``x >> y``    | Arithmetic shift right (Java) |                              |
+---------------+-------------------------------+                              |
| ``x >>> y``   | Logical shift right (Java)    |                              |
+---------------+-------------------------------+------------------------------+
| ``x < y``     | Less than                     | 6, →                         |
+---------------+-------------------------------+                              |
| ``x <= y``    | Less than or equal            |                              |
+---------------+-------------------------------+                              |
| ``x > y``     | Greater than                  |                              |
+---------------+-------------------------------+                              |
| ``x >= y``    | Greater than or equal         |                              |
+---------------+-------------------------------+------------------------------+
| ``x == y``    | Equality                      | 7, →                         |
+---------------+-------------------------------+                              |
| ``x != y``    | Inequality                    |                              |
+---------------+-------------------------------+------------------------------+
| ``x & y``     | Bitwise AND                   | 8, →                         |
+---------------+-------------------------------+------------------------------+
| ``x ^ y``     | Bitwise XOR                   | 9, →                         |
+---------------+-------------------------------+------------------------------+
| ``x \| y``    | Bitwise OR (*)                | 10, →                        |
+---------------+-------------------------------+------------------------------+
| ``x && y``    | Logical AND                   | 11, →                        |
+---------------+-------------------------------+------------------------------+
| ``x ^^ y``    | Logical XOR (*)               | 12, →                        |
+---------------+-------------------------------+------------------------------+
| ``x \|\| y``  | Logical OR                    | 13, →                        |
+---------------+-------------------------------+------------------------------+
| ``x ? y : z`` | Ternary conditional           | 14, ←                        |
+---------------+-------------------------------+------------------------------+

The arithmetic operators deviating from C were taken from Python because
Python's behavior is more intuitive when doing high-level math. Integer division
and accompanying modulo/remainder follow Python's behavior as well (sign of
remainder = sign of divider), because the round-to-zero behavior in the C
standard is more historical than it is useful.

The right-shift operators follow Java, because (like Java) cQASM doesn't have
unsigned integers. Therefore, disambiguation of logical (shift in zeros) vs.
arithmetic (shift in sign bit) for right-shift has to be done using the operator
rather than the operand types.

The `|` operator (bitwise OR) requires further explanation in cQASM 1.x, because
it conflicts with the `|` separator in bundle notation. To avoid breaking
anything, the `|` operator is illegal as argument to a gate without at least one
level of parentheses surrounding it; `|` always defaults to separating gates in
a bundle otherwise. This applies even when the resulting parse tree would be
invalid; this is a limitation of LALR(1) parsers such as Bison, the one used by
libqasm.

The `^^` operator is typically missing from languages, because one of the
primary purposes of it is "short-circuiting" of operand evaluation, and an XOR
gate always requires both operands to be evaluated. Nevertheless, `there are
several arguments for including it anyway, even in C
<https://stackoverflow.com/questions/1596668/logical-xor-operator-in-c>`_. Not
mentioned there is that it is also a natural expansion of the operator
precedence table. Worth noting also is that side effects of expressions are not
first-class citizens in cQASM as they are in C (libqasm makes no assertion about
whether they can have side effects or not), so the short-circuiting argument is
moot anyway. Finally, the existence of the expression doesn't hurt anyone; it
would be invalid syntax would it not exist, and the program/library using
libqasm is free to not register any implementation for `operator^^`, thus
disabling it entirely.

cQASM 1.0 does not support operations evaluated at runtime; only constant
propagation is supported. cQASM 1.1 adds support for such dynamic expressions,
but the default set of operations is still constant-propagation-only.

Grouping parentheses
--------------------

Parentheses (``(`` and ``)``) may be placed around expressions to order expressions
differently than what the precedence rules would result in, or to disambiguate
between the pipe (``|``) used for separating instructions in bundle notation
and the bitwise OR operator.
