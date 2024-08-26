import cqasm.v3x.instruction
import cqasm.v3x.types

Str = str
Bool = bool
Int = int
Float = float
Complex = complex


class Version(tuple):
    """cQASM file version number."""

    def __new__(cls, elements):
        if isinstance(elements, str):
            elements = elements.split('.')
        return super(Version, cls).__new__(cls, tuple(map(int, elements)))


def serialize(typ, val):
    if isinstance(typ, str):
        return None
    elif typ is Str:
        return {'x': val}
    elif typ is Bool:
        return {'x': val}
    elif typ is Int:
        return {'x': val}
    elif typ is Float:
        return {'x': val}
    elif typ is Version:
        return {'x': list(val)}
    elif typ is cqasm.v3x.instruction.InstructionRef:
        if val.data is None:
            return {}
        else:
            return {
                'n': val.data.name,
                't': [x.serialize() for x in val.data.types]
            }
    else:
        assert False


def deserialize(typ, val):
    if isinstance(typ, str):
        return None
    elif typ is Str and isinstance(val['x'], bytes):
        # CBOR strings are bytes objects. The correct conversion to Str would be through a decoding.
        # Str(b'qubit', 'utf-8') would generate the string "qubit". This is the same as b'qubit'.decode('utf-8')
        # Whereas Str(b'qubit') would generate the string "b'qubit'"
        return Str(val['x'], 'utf-8')
    elif typ is Str:
        return Str(val['x'])
    elif typ is Bool:
        return Bool(val['x'])
    elif typ is Int:
        return Int(val['x'])
    elif typ is Float:
        return Float(val['x'])
    elif typ is Version:
        return Version(val['x'])
    elif typ is cqasm.v3x.instruction.InstructionRef:
        if 'n' in val:
            return cqasm.v3x.instruction.InstructionRef(
                val['n'],
                [cqasm.v3x.types.Node.deserialize(x) for x in val['t']]
            )
        else:
            return cqasm.v3x.instruction.InstructionRef()
    else:
        assert False
