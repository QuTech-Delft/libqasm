import cqasm.v3x.types


class Instruction(object):
    def __init__(self, name, types=None):
        super().__init__()
        self._name = str(name)
        if types is None:
            self._types = ()
        else:
            for typ in types:
                if not isinstance(typ, cqasm.v3x.types.Node):
                    raise TypeError('types must be an iterable of cqasm.v3x.types.Node if specified')
            self._types = tuple(types)

    @property
    def name(self):
        return self._name

    @property
    def types(self):
        return self._types

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
