Structure
=========

cQASM 1.x files consist of a ``version`` statement, optionally a ``qubits``
statement (mandatory for 1.0, optional for 1.1+), optionally an
``error_model`` statement, and zero or more of any combination and ordering
of the following statement types:

- mappings,
- variables (version 1.1+),
- subcircuit headers, and
- bundles.

A simple algorithm might look like this.

.. code-block:: text

    version 1.0
    qubits 2

    map input = q[0]
    map output = q[1]

    .initialize
    prep_z input | prep_z output
    x output
    h input | h output

    .oracle
    cnot input, output

    .measure
    h input
    measure input

File extension
--------------

The file extension for cQASM files is preferably `.cq`. Historically everyone
seems to use `.qasm`, but this conflicts with the OpenQASM format. libqasm
doesn't care, but DQCsim goes as far as to require that the file extension is
`.cq`, as it uses file extensions to disambiguate the input file format (the
problem, here, is that it also has an OpenQASM frontend).

Whitespace and comments
-----------------------

cQASM 1.x is newline-sensitive. In most contexts, newlines are used to terminate
statements. A semicolon (``;``) may be used instead of the newline if the code
is more readable when two statements occupy the same line. Newlines can also be
"escaped" using a backslash (``\``) immediately in front; this disables the
functional behavior of the newline and thus allows statements to be split over
a single line.

.. code-block:: text

    statement 1
    statement 2; statement 3
    a very long statement 4,    \
        but a single statement  \
        nontheless.

Besides newlines, cQASM 1.x is not whitespace-sensitive; tabs and spaces can go
between any two tokens.

Single-line comments use the hash (``#``) prefix as in Python and most other
scripting languages. Block comments use ``/* */`` as in C and various C-like
languages. Like spaces and tabs, block comments can go between any two tokens.

.. code-block:: text

    statement 1 # I'm a comment
    statement /* I'm a much
    longer comment */ 2

cQASM 1.x files are not case-sensitive.

.. code-block:: text

    h q[1]
    H Q[1] # this means the same thing

Keywords
--------

cQASM 1.x recognizes the following keywords. These words cannot be used to name
mappings or variables.

.. code-block:: text

    qubits  map  var  cond

.. note::

    All gates, axes (``x``, ``y``, ``z``), the qubit register (``q``), and the
    bit register (``b``) used to be keywords. This is no longer the case, so
    you can freely rename a qubit to for instance ``b`` now. Be aware, however,
    that this shadows the bit register, meaning that you won't be able to use
    it anymore later. The type system is such that the operands of a gate are
    used to disambiguate the gate; the gate can thus not be used to disambiguate
    between ``b`` as in a mapping to a qubit and ``b`` as in the bit register.
    The same applies for ``q``, ``x``, ``y``, and ``z`` (the latter three only
    if you use a gate that takes an axis operand). You can work around this by
    first remapping the entire register or the axis to a longer word, for
    instance using ``map qubits = q``.
