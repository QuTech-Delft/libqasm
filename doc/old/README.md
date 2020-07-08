# Old documentation

This directory contains outdated documentation for libqasm.

File list:

 - `1805.09607.pdf`: arXiv paper describing the intent of cQASM 1.0 and its featureset. It is mostly accurate at the time of writing, except for the following grammar differences and typos:

    - the syntax of the "multi-binary-controlled quantum gate" as described in Section III. E. and its accompanying code example is inaccurate. To prevent grammatical ambiguity, multiple conditions are instead specied using multiple indices, as follows:

      ```
      # Instead of
      # c-x b[2], b[3], b[4], q[5]
      # as described in the paper, use
      c-x b[2,3,4], q[5]
      ```

      Interestingly, example 5 uses the syntax as implemented.

    - Code example 2 uses the mapping name `extra` without defining this mapping.
    - Code example 7 uses `reset_averaging`, whereas this is in fact `reset-averaging` in the implementation.
    - Code example 7 is missing a comma between the qubit and angle arguments of rx and ry.
    - Code example 7 uses `q[0:3]` to refer to the full 3-qubit register, which should in fact be `q[0:2]`, as the range is inclusive.

   Note that not all current language features or (default) instructions are documented in the paper, such as the `U` or `skip` gates.

 - `qasm.bnf`: a severely broken description of cQASM. I don't dare remove it but don't use it. Some problems with it:

    - The version statement is missing.
    - Subcircuits can be empty.
    - The BNF grammar is ambiguous; for instance, multiple statements can go into different headerless subcircuits or into one subcircuit (latter being correct).
    - Numbers are defined differently in the implementation. The biggest thing here is that <number> does not match "0", while it's used for instance in qubit indices.
    - Underscores in names are supported in the implementation but not in the BNF.
    - For some reason, the BNF seems to disallow multiple-indexing for qubits in some places (`map` directive, `measure_parity`). The implementation allows this everywhere.
    - (stopped checking around line 100)

 - `testBnf.sh`: uses one of the tools in `qtools` to generate random code samples that match `qasm.bnf`.

 - `qtools`: contains some tools for operating on BNF files.
