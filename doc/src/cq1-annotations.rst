Annotations
===========

Annotations may be used to attach arbitrary data to various constructs in a
cQASM 1.x file, in order to communicate things to the target that cQASM 1.x
does not know about. For example, you might want to annotate a qubit with
its specific decoherence time, or a gate with additional error information.
The intention of annotations is that the program is still valid and sensible
if all annotations are ignored.

Annotations have the following forms:

.. code:: text

    <object-to-annotate> @<interface>.<operation>
    <object-to-annotate> @<interface>.<operation>()
    <object-to-annotate> @<interface>.<operation>(<comma-separated-operands>)

Here, ``<interface>`` and ``<operation>`` are two identifiers (words consisting
of letters, digits, and underscores, not starting with a digit) that specify
the high-level behavior of the annotation. The first should ideally be named
the same as the program or library that the annotation targets, while the
second should signify what the annotation means. libqasm makes no assertion
about the number of operands and their types; it is up to the target to provide
error checking for this. A target that supports a certain value for
``<interface>`` should throw an error when it encounters an annotation with
that interface but an unknown operation, as this is likely to indicate that the
cQASM file was written for a newer version of the target.

Annotatable objects can take any number of annotations by simply chaining
annotations. For example:

.. code:: text

    x q[1] @sim.model("high-accuracy") @insn.duration(10)

The following constructs can be annotated in cQASM 1.x:

- error models;
- instructions;
- bundles;
- subcircuit headers;
- mappings;
- and variables (version 1.1+).
