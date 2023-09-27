Python API
==========

Besides the intended usage from C++, libqasm also has a (comparatively
simplistic) Python interface, installable via the
`libqasm <https://pypi.org/project/libqasm/>`_ PyPI package or using
``pip3 install -v -e .`` (or equivalent) from the root directory of the
repository. This exposes two modules, ``libQasm`` and ``cqasm.v1x``. The
former exists for backward compatibility with the original Python API,
and supports only cQASM 1.0 and the default instruction set. The latter
supports up to cQASM 1.2, mirroring the C++ API.

Documentation is unfortunately scarce, as the Python interface has not been a
priority compared to the C++ interface. The new API is rather straightforward,
however. This should hopefully get you started:

.. code-block:: python

    import cqasm.v1x
    analyzer = cqasm.v1x.Analyzer('1.2')
    result = a.analyze_string('version 1.2; qubits 3; x q[0]')

The argument passed to the ``Analyzer()`` constructor specifies the API version
that your application supports. This version follows the cQASM file versions.
This ensures that you don't get problems when a new version of libqasm comes
out that supports a newer version; if the user then passes a file that's too
new, they get an error message stating the maximum version, rather than a
cryptic error down the line when your application receives something from
libqasm that it doesn't understand.

The ``Analyzer()`` has an optional second argument, too. This is a boolean,
named ``without_defaults``, defaulting to ``False``. When set to ``True``, the
process that registers the default instruction set and error models with the
analyzer is skipped. You can then add instructions to the instruction set using
the ``register_instruction()`` method:

.. code-block:: text

    Analyzer.register_instruction(
        name,
        param_types='',
        allow_conditional=True,
        allow_parallel=True,
        allow_reused_qubits=False,
        allow_different_index_sizes=False
    )

The ``param_types`` argument specifies the allowed operand types for the
instruction. Each character represents an operand:

 - ``Q`` = qubit;
 - ``B`` = assignable bit/boolean (measurement register);
 - ``b`` = bit/boolean;
 - ``a`` = axis (x, y, or z);
 - ``i`` = integer;
 - ``r`` = real;
 - ``c`` = complex;
 - ``u`` = complex matrix of size 4^n, where n is the number of qubits in
   the parameter list (automatically deduced);
 - ``s`` = (quoted) string;
 - ``j`` = json.

The rest of the parameters should be fairly obvious.

It is also possibly to disable the default instruction set and not add any
instructions. In this case -- rather than rejecting all cQASM files -- libqasm
will accept any instruction with any operand list, allowing you to do
instruction set management yourself.

You can register error models in much the same way, if you want:

.. code-block:: text

    Analyzer.register_error_model(
        name,
        param_types=''
    )

Besides ``analyze_string(data)``, the ``Analyzer`` class also has
``analyze_file(filename)``, ``parse_string(data)``, and ``parse_file(filename)``
variants. The difference between ``*_string`` and ``*_file`` should be obvious;
the difference between ``analyze_*`` and ``parse_*`` is that the former runs
the complete analysis process, while the latter gives you the raw parse result.
You'll almost always want the former.

The result of these functions is either a list of error messages or a tree
structure. The tree structure is quite involved, but also very easy to traverse
once you have one in Python. Simply print the value to get a multiline,
pretty-printed representation of the (sub)tree. Tree access (or manipulation,
if you want) is then done via the usual Python operators (``.`` for field
access, ``[]`` for indexing into lists, and so on).

Some additional information may be available via the C++ documentation
`here <doxy/classV1Analyzer.html>`_. Note however that some stuff here may be
inaccurate or incomplete, as it only documents the C++ portion, and some Python
stuff is added on top to make the interface more Pythonic. This unfortunately
does not have generated documentation at this time.
