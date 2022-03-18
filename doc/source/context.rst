Context and history
===================

.. note::

    This recounts my (Jeroen van Straten) personal understanding on this matter
    as of October 19th, 2020. It may not be entirely correct; please amend if
    need be. But I feel that it's nevertheless important to provide context on
    how some of these things came to be.

cQASM 1.0 was originally documented and published by means of `an arXiv paper
<https://arxiv.org/abs/1805.09607v1>`_. It "aims to abstract away qubit
technology details and guarantee the interoperability between all the quantum
compilation and simulation tools supporting this standard." Nevertheless, the
feature set of cQASM 1.0 was very much chosen based on what the QCA group needed
at the time, with no regard for generality beyond that; mostly, it was just a
replacement for the quantum code frontend baked into the QX simulator. In most
of the time since, features were added in an ad-hoc fashion when they became
absolutely necessary (such as the arbitrary single-qubit unitary gate, or the
shuttle gates added for spin qubits in a fork), awaiting an almost mystical
"cQASM 2.0" specification that was supposed to add support for classical
control-flow.

This was the state when I joined the QCA team at the end of 2018, and I was
tasked to write this cQASM 2.0 specification. Little did I know at the time that
various attempts had been made before at this time, and there was also already
`some working document on Overleaf
<https://www.overleaf.com/project/59690199d094287443563643>`_ (unfortunately not
publicly accessible for whatever reason) to express and discuss requirements
that no one at QCA could tell me about. `The specification I wrote independently
<https://github.com/QuTech-Delft/cQASM-spec/tree/7967e00dd3ec965b37edcbff30a666b0baada9dd>`_
was ultimately shot down for being far too extensive and ambitious, so in the end,
all I could do was add to the Overleaf document. Unfortunately no one has felt
responsible to pursue cQASM 2.0 since then, that I know of.

Fast forward to summer 2020, when I was tasked to look into making libqasm's
instruction set configurable, in the same way that OpenQL's instruction set is
specified in the JSON target description file. The idea here is that cQASM could
be used as a representation of OpenQL's internal intermediate representation, as
part of OpenQL's modularization effort. However, since libqasm's Flex/Bison
parser was written with the instruction set hardcoded into it, this
automatically `required a complete rewrite of the library
<https://github.com/QuTech-Delft/libqasm/pull/105>`_. Backward compatibility was key
here; after all, libqasm is used in many places beyond QX at this time (most
importantly, various benchmark suites have been ported to cQASM 1.0, and Quantum
Inspire runs on it). This required some special cases in the grammar and a type
system with overloading capability, but ultimately this was doable. Significant
parts of the new code were copied from what I had already been working on based
on my cQASM 2.0 specification, as it made little sense to me to strip everything
out, although I intentionally only added language features that would pose no
requirements on the programs/libraries using libqasm whatsoever.

The major downside of making the instruction set configurable is that it reduces
the cQASM specification to just syntax, while the original intention of cQASM
was to define the complete language, including semantics. To mitigate this,
libqasm provides methods to construct a "default" parser with the instruction
set cQASM understood before this change, and the semantics of this instruction
set is documented along with the cQASM 1.0 syntax here. Users of the libqasm
library are recommended to just use this default instruction set unless they
have a good reason not to do so (like OpenQL).

Not too long after that pull request, extending cQASM's conditional execution
syntax was on the agenda, in order to allow expression of all conditional
constructs supported by the QuTech Central Controller using cQASM. This required
state variables and dynamically-evaluated expressions. Hence, basic support for
this was recently added. Since this is a bit of a paradigm shift, but still
entirely backward compatible, I figured it would be a good idea to name this
cQASM 1.1.

The idea was to continue with 2.0 after that. The latest spec for this can be
found `here <https://github.com/QuTech-Delft/cQASM-spec/tree/95ee081c0910548b8201c510f88ebc89be9284e8>`_.
However, it was ultimately determined that completing and implementing this
specification would take more manpower than we have. Therefore, cQASM 1.2 was
created. This update adds crude assignment statements and basic structured
control-flow statements. It is, again, backward compatible, save for the newly
added keywords.
