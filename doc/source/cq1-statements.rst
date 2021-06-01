Statements
==========

A cQASM 1.x file ultimately consists of a number of statements. Several kinds
of statements are defined. They are all listed in this section.

Version statement
-----------------

The first statement in every cQASM file must be a version statement. It
specifies which version of the cQASM language is to be used when parsing. It
has the form

.. code:: text

    version <period-separated-integers>

For example, the version statement for a cQASM 1.0 file must be

.. code:: text

    version 1.0

cQASM 1.1 adds support for variables and dynamic expressions. In order to use
these features, you have to select at least version 1.1:

.. code:: text

    version 1.1

cQASM 1.2 adds support for assignment statements and structured control-flow.
In order to use these features, you have to select at least version 1.2:

.. code:: text

    version 1.2

Qubits statement
----------------

This statement declares the desired size of the qubit register (``q``) and
associated measurement bit register (``b``). It has the form:

.. code:: text

    qubits <positive-integer-constant>

It must be the second statement in the cQASM file if specified. The statement
is mandatory in cQASM 1.0, but optional for cQASM 1.1+. The latter allows
variables to be used to replace the ``qubits`` statement, avoiding the implicit
link between qubits and measurement bit storage that is not universal among
control architectures.

Error model statement
---------------------

Optionally, an error model statement may be used to select and configure an
error model for the targeted simulator. It has the following form:

.. code:: text

    error_model <name>, <comma-separated-operands>

Available error models must be registered with libqasm while the parser is
constructed when using the new API, but for the old API, QX's error models is
built in. In this case, the only value for ``<name>`` is
``depolarizing_channel``, and zero or more real numbers may be specified as
arguments. Refer to QX's documentation for information about what these
arguments do.

.. admonition:: Historical

    The original API of libqasm allowed the user to specify the error model
    anywhere in the program, and performed no check to ensure that it would only
    be specified once. Ultimately, the final error model specified determines
    the error model for the program. For backward-compatibility reasons, this
    behavior is mimicked by the new API.

Error model definitions may be annotated.

Variable statement (1.1+)
-------------------------

This statement declares one or more variables. It is only supported for targets
that actually support variables. It has the following form:

.. code:: text

    var <comma-separated-names>: <type>

where ``<type>`` may be ``qubit``, ``bool``, ``bit`` (synonym for bool),
``int``, ``real``, or ``complex``.

Variable statements may appear anywhere in the program. the variable will be
usable from its declaration onwards. It is legal, if perhaps ill-advised, to
make multiple variables of the same name; they will be interpreted as distinct
variables, where references to that variable name simply refer to the latest
declaration.

.. note::

    Because variables can be declared anywhere, they can also be declared
    within structured control-flow statements in cQASM 1.2+. However, cQASM
    does *not* support scoping of any kind aside from name resolution. All
    variables are assumed to be mapped to distinct registers. This means that,
    style and readability aside, the following is perfectly fine:

    .. code:: text

        if (...) {
            var y: int;
        }
        set y = 1;

    It is equivalent to the following:

        if (...) {
        }
        var y: int;
        set y = 1;

Variable statements can be annotated.

Map statement
-------------

This statement maps an expression to a word, allowing the expression to be used
later by simply using the word, like an alias or a macro. It has the following
equivalent forms:

.. code:: text

    map <expression>, <alias>
    map <alias> = <expression>

Here, ``<alias>`` is a word consisting of letters, digits, and underscores, not
starting with a digit. ``<expression>`` is any expression of any data type.
Name resolution for any mappings in the expression occurs in the context of the
``map`` statement, but the expression itself is only evaluated when the alias is
used.

Map statements can be annotated.

.. note::

    Map statements are NOT assignment statements. They are functionally
    equivalent to a lambda without arguments in Python or (more or less)
    preprocessor definitions in C; the expression is not evaluated by the map
    statement, but where the mapping is used. For example, in the following
    algorithm (assuming perfect qubits and measurement),

    .. code:: text

        prep_z q[0]
        measure q[0]

        map q0_measured_zero = !b[0]
        cond (q0_measured_zero) x q[0]

        measure q[0]
        cond (q0_measured_zero) x q[0]

    the first X gate would be executed, because ``b[0]`` is false/zero, thus
    ``!b[0]`` is true. But ``q0_measured_zero`` has not assumed the value
    ``true``; it retains the complete expression. Therefore, the second X gate
    would NOT be executed, as the second measurement flips the state of
    ``b[0]``. The position of the map statement doesn't matter, as long as it's
    before the first line where ``q0_measured_zero`` is used.

.. admonition:: Historical

    Mappings are called such because originally they were only used to allow
    users to rename the numbered qubits and measurement bits in the qubit
    register to some user-specified name to make programming algorithms in
    cQASM more ergonomic. When expressions were added however, it made sense
    to extend the definition to any kind of expression, allowing it to be
    reused for all the things listed above.

Subcircuit headers
------------------

Subcircuit header statements can be used to divide an algorithm up into logical
subsections, and may also be used to specify that a part of the algorithm must
be repeated a constant number of times. They have the following forms:

.. code:: text

    .<name>
    .<name>(<repeat-count>)

Here, ``<name>`` is a word consisting of letters, digits, and underscores, not
starting with a digit. ``<repeat-count>`` is a constant positive integer. The
subcircuit header signifies that all bundles up to the next subcircuit header
or the end of the file belong to a subcircuit named ``<name>``, and that that
subcircuit must be evaluated ``<repeat-count>`` times. The repeat count is
implicitly one when not specified.

Subcircuit headers may be annotated.

Bundles and instructions
------------------------

The algorithm is ultimately described using bundles, defined to be one or more
instructions issued simultaneously. They are either specified using a
pipe-separated list (``|``) of one or more instructions on a single line, or
using a multiline curly-bracket-delimited list (``{`` and ``}``) of one or more
pipe-separated lists of one or more instructions. For example,

.. code:: text

    <insn-a> | <insn-b> | <insn-c>
    # In both the above and below case, all three instructions start simultaneously.
    {
        <insn-a> | <insn-b>
        <insn-c>
    }

The instruction format is documented in the next section.

.. note::

    Instructions are not statements; only bundles are. A single instruction on
    its own line is simply a bundle with only one instruction in it.

Both the individual instructions in a bundle and the bundle as a whole can be
annotated. The former takes precedence; therefore, annotating a bundle can only
be done using the curly-bracket notation.

If-else chain (1.2+)
--------------------

In cQASM 1.2+, branch-based conditional blocks can be constructed using
if-else chains. They have the following syntax.

.. code:: text

    # Simple if statement.
    if (<condition>) {
        <statements>
    }

    # If-else statement.
    if (<condition>) {
        <statements>
    } else {
        <statements>
    }

    # If-elif-else statement. You can repeat as many "else if" blocks as you
    # like.
    if (<condition>) {
        <statements>
    } else if (<condition>) {
        <statements>
    } else {
        <statements>
    }

The shown newlines are optional. The ``{``\ s and ``}``\ s are mandatory.

.. note::

    The ``{}`` tokens do *not* imply parallel execution in this context.

The conditions must evaluate to a boolean.

C-style for loop (1.2+)
-----------------------

In cQASM 1.2+, C-style for loops can be written as follows.

.. code:: text

    for (<initialize>; <condition>; <update>) {
        <statements>
    }

The ``<initialize>`` subblock is optional. If specified, it must be of the form
``<name> = <expression>``, representing an initializing assignment statement.
It is executed at the start of the loop.

.. note::

    Unlike C, it is not possible to declare a new variable as part of the
    ``<initialize>`` subblock.

The ``<condition>`` subblock must be an expression that evaluates to a boolean.
It is evaluated at the start of each loop iteration. If it yields true,
iteration continues; if it yields false, execution continues after the for
loop.

The ``<update>`` subblock is optional. If specified, it must be of the form
``<name> = <expression>``, representing an assignment statement. It is executed
at the end of each loop iteration. It is intended to be used to update the loop
variable.

The loop body may include ``continue`` and ``break`` statements.

Foreach loop (1.2+)
-------------------

In cQASM 1.2+, a loop that iterates over a range of integer values can be
written as follows.

.. code:: text

    foreach (<name> = <from> .. <to>) {
        <statements>
    }

``<name>`` must be an integer variable, and ``<from>`` and ``<to>`` must
constant-propagate to integer literals; that is, both integers must be
known at compile-time. The loop body will be executed for all values in
the specified inclusive range. If ``<from>`` is less than ``<to>``, ``<name>``
will be incremented by one after each iteration. If ``<from>`` is greater than
``<to>``, ``<name>`` will be decremented by one after each iteration.

Behavior is undefined if ``<name>`` is reassigned from within the loop body.

While loop (1.2+)
-----------------

In cQASM 1.2+, a loop that iterates while a condition is true can be written
as follows.

.. code:: text

    while (<condition>) {
        <statements>
    }

``<condition>`` must be an expression that evaluates to a boolean. It is
evaluated at the before a new iteration. If it evaluates to true, iteration
will continue. Otherwise, execution will continue after the while loop.

Repeat-until loop (1.2+)
------------------------

In cQASM 1.2+, a loop that iterates until a condition is true can be written
as follows.

.. code:: text

    repeat {
        <statements>
    } until (<condition>)

``<condition>`` must be an expression that evaluates to a boolean. It is
evaluated at the end of each iteration. If it evaluates to false, iteration
will continue. Otherwise, execution will continue after the repeat-until
loop.

Break statement (1.2+)
----------------------

In cQASM 1.2+, the innermost loop can be terminated at any time by means of a
``break`` statement. The syntax is simply:

.. code:: text

    break

It is illegal to use a ``break`` statement outside of the context of a
structured loop (``for``, ``foreach``, ``while``, or ``repeat``-``until``).
A subcircuit with a repetition count does *not* qualify as a loop in this
context.

Continue statement (1.2+)
-------------------------

In cQASM 1.2+, the current loop iteration can be stopped at any time by means
of a ``continue`` statement. Execution will continue as if the end of the loop
body had been reached. The syntax is simply:

.. code:: text

    continue

It is illegal to use a ``continue`` statement outside of the context of a
structured loop (``for``, ``foreach``, ``while``, or ``repeat``-``until``).
A subcircuit with a repetition count does *not* qualify as a loop in this
context.
