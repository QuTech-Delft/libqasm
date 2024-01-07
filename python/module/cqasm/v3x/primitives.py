from cqasm.v3x.instruction import Instruction, InstructionRef
import cqasm.v3x.types

Str = str
Bool = bool
Int = int
Real = float
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
    elif typ is Axis:
        return {'x': val[0], 'y': val[1], 'z': val[2]}
    elif typ is Bool:
        return {'x': val}
    elif typ is Int:
        return {'x': val}
    elif typ is Real:
        return {'x': val}
    elif typ is Complex:
        return {'r': val.real, 'i': val.imag}
    elif typ is Version:
        return {'x': list(val)}
    elif typ is InstructionRef:
        if val.data is None:
            return {}
        else:
            return {
                'n': val.data.name,
                'i': val.data.request_qubit_and_bit_indices_have_same_size,
                't': [x.serialize() for x in val.data.types]
            }
    else:
        assert False


def deserialize(typ, val):
    if isinstance(typ, str):
        return None
    elif typ is Str:
        return Str(val['x'])
    elif typ is Axis:
        return Axis(val['x'], val['y'], val['z'])
    elif typ is Bool:
        return Bool(val['x'])
    elif typ is Int:
        return Int(val['x'])
    elif typ is Real:
        return Real(val['x'])
    elif typ is Complex:
        return Complex(val['r'], val['i'])
    elif typ is Version:
        return Version(val['x'])
    elif typ is InstructionRef:
        if 'n' in val:
            return InstructionRef(
                val['n'],
                [cqasm.v3x.types.Node.deserialize(x) for x in val['t']],
                val['i']
            )
        else:
            return InstructionRef()
    else:
        assert False
