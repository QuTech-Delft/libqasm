import cqasm.v1.types

class Instruction(object):
    def __init__(
        self,
        name,
        types=None,
        allow_conditional=True,
        allow_parallel=True,
        allow_reused_qubits=False,
        allow_different_index_sizes=False
    ):
        super().__init__()
        self._name = str(name)
        if types is None:
            self._types = ()
        else:
            for typ in types:
                if not isinstance(typ, cqasm.v1.types.Node):
                    raise TypeError('types must be an iterable of cqasm.v1.types.Node if specified')
            self._types = tuple(types)
        self._allow_conditional = bool(allow_conditional)
        self._allow_parallel = bool(allow_parallel)
        self._allow_reused_qubits = bool(allow_reused_qubits)
        self._allow_different_index_sizes = bool(allow_different_index_sizes)

    @property
    def name(self):
        return self._name

    @property
    def types(self):
        return self._types

    @property
    def allow_conditional(self):
        return self._allow_conditional

    @property
    def allow_parallel(self):
        return self._allow_parallel

    @property
    def allow_reused_qubits(self):
        return self._allow_reused_qubits

    @property
    def allow_different_index_sizes(self):
        return self._allow_different_index_sizes

    def __str__(self):
        return '{}({})'.format(self._name, ', '.join(map(str, self._types)))


class InstructionRef(object):
    def __init__(self, *args, **kwargs):
        if not args and not kwargs:
            self._data = None
        elif len(args) == 1 and not kwargs and (isinstance(args[0], Instruction) or args[0] is None):
            self._data = args[0]
        else:
            self._data = Instruction(*args, **kwargs)

    @property
    def data(self):
        return self._data

    def __str__(self):
        return 'unresolved' if self._data is None else str(self._data)
