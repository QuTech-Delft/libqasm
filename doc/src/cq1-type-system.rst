Type system
===========

During semantic analysis, libqasm assigns data types to expressions trees in
depth-first order. This type system knows the following types:

- qubits;
- bits (a.k.a. booleans);
- axes (X, Y, or Z);
- integers (64-bit signed in libqasm, may be less in target);
- real scalars (IEEE 754 double precision in libqasm, may be different in
  target);
- complex scalars (2x IEEE 754 double precision for real and imaginary
  components in libqasm, may be different in target);
- real vectors and matrices (IEEE 754 double precision per element in libqasm,
  may be different in target);
- complex vectors and matrices (2x IEEE 754 double precision for real and
  imaginary components for each element in libqasm, may be different in target);
- strings (libqasm makes no assertions about encoding); and
- JSON strings.

Types are used in semantic analysis to check whether the operands for
instructions, functions, and operators make sense (for example, ``X 1`` makes no
sense; you can't apply an X gate to an integer), and in some cases are used to
disambiguate between multiple overloads of the same gate or function.

.. admonition:: Historical

    The above used to be hardcoded in the grammar. That is, ``X 1`` wouldn't
    parse not because of a type system, but because the ``X`` keyword had to
    be followed by a single qubit. This is a bad idea for many reasons, such as
    readability of the grammar specification, maintainability, inability to
    redefine ``X`` to refer to a qubit using a mapping, and lack of
    configurability of the instruction set as done in OpenQL. It is, however,
    easy to write things down this way that are difficult to express with a
    type system. Combined with the backward compatibility requirement when
    instruction set configurability became important, this relatively extensive
    type system had to be introduced.

Constant propagation and coercion
---------------------------------

While doing type analysis, libqasm also performs constant propagation where
applicable. This means that the expression ``1 + 3`` is completely equivalent
to just ``4``. Wherever constants are expected, you can still use operators and
functions with constant propagation rules to help make the code clearer.

libqasm also evaluates promotion rules during this process. Specifically;

- If an integer constant is found but a real number is expected, the integer is
  implicitly converted to a real number.
- If an integer or real number constant is found but a complex number is
  expected, the integer or real number is implicitly converted to a complex
  number.
- If a constant real matrix is found but a complex matrix is expected, and the
  real matrix has dimensions matching the ones expected for the complex matrix,
  the real matrix is converted to a complex matrix.
- If a constant real row vector with 2n\ :sup:`2` entries is found but a complex
  matrix of size n by n is expected, the elements in the vector are interpreted
  as real-imaginary pairs in row-major order, and the value is converted
  accordingly. For example:

  .. code:: text

    [1, 2, 3, 4, 5, 6, 7, 8]
    # is promoted to
    [
        1 + 2*im, 3 + 4*im
        5 + 6*im, 7 + 8*im
    ]
    # whenever a 2x2 complex matrix is expected.

  This rule exists only for backward compatibility for the one-qubit ``U`` gate.

Note that all promotion rules only apply to constants. When a target supports
dynamic evaluation, it must provide functions to convert between types if the
target supports this, and the user must explicitly use these functions.
