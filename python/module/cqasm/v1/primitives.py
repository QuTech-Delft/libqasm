from enum import Enum
import numpy as np
from cqasm.v1.error_model import ErrorModel, ErrorModelRef
from cqasm.v1.instruction import Instruction, InstructionRef
import cqasm.v1.types

Str = str
Bool = bool
Int = int
Real = float
Complex = complex

class Axis(Enum):
    X = 0
    Y = 1
    Z = 2

class _Matrix(object):
    """Matrix adapter."""

    def __init__(self, data):
        super().__init__()
        self._data = data
        if len(self._data.shape) == 1:
            self._ncols = self._data.shape[0]
            self._nrows = 1
        elif len(self._data.shape) == 2:
            self._ncols = self._data.shape[0]
            self._nrows = self._data.shape[1]
        else:
            raise TypeError('expecting a real-valued vector or matrix, got ' + repr(mat))

    def size_rows(self):
        return self._nrows

    def size_cols(self):
        return self._ncols

    def as_raw_data(self):
        return self._data.reshape([self._nrows * self._ncols])

    def as_numpy(self):
        return self._data


class RMatrix(_Matrix):
    """Real-valued matrix adapter."""

    def __init__(self, data):
        super().__init__(np.array(mat, float))

class CMatrix(_Matrix):
    """Complex-valued matrix adapter."""

    def __init__(self, data):
        super().__init__(np.array(mat, complex))

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
    elif typ is Real:
        return {'x': val}
    elif typ is Complex:
        return {'r': val.real, 'i': val.imag}
    elif typ is Axis:
        if val == Axis.X:
            return {'x': 0}
        elif val == Axis.Y:
            return {'x': 1}
        elif val == Axis.Z:
            return {'x': 2}
        else:
            assert False
    elif typ is RMatrix:
        return {'c': val.size_cols(), 'd': list(val.as_raw_data())}
    elif typ is CMatrix:
        data = []
        for c in val.as_raw_data():
            data.append(c.real)
            data.append(c.imag)
        return {'c': val.size_cols(), 'd': data}
    elif typ is Version:
        return {'x': list(val)}
    elif typ is ErrorModelRef:
        if val.data is None:
            return {}
        else:
            return {
                'n': val.data.name,
                't': [x.serialize() for x in val.data.types]
            }
    elif typ is InstructionRef:
        if val.data is None:
            return {}
        else:
            return {
                'n': val.data.name,
                'c': val.data.allow_conditional,
                'p': val.data.allow_parallel,
                'r': val.data.allow_reused_qubits,
                'd': val.data.allow_different_index_sizes,
                't': [x.serialize() for x in val.data.types]
            }
    else:
        assert False

def deserialize(typ, val):
    if isinstance(typ, str):
        return None
    elif typ is Str:
        return Str(val['x'])
    elif typ is Bool:
        return Bool(val['x'])
    elif typ is Int:
        return Int(val['x'])
    elif typ is Real:
        return Real(val['x'])
    elif typ is Complex:
        return Complex(val['r'], val['i'])
    elif typ is Axis:
        if val['x'] == 0:
            return Axis.X
        elif val['x'] == 1:
            return Axis.Y
        elif val['x'] == 2:
            return Axis.Z
        else:
            assert False
    elif typ is RMatrix:
        return RMatrix(np.array(val['d'], dtype=float).reshape([val['c'], len(val['d']) // val['c']]))
    elif typ is CMatrix:
        return CMatrix(np.array(val['d'], dtype=complex).reshape([val['c'], len(val['d']) // val['c']]))
    elif typ is Version:
        return Version(val['x'])
    elif typ is ErrorModelRef:
        if 'n' in val:
            return ErrorModelRef(
                val['n'],
                [cqasm.v1.types.Node.deserialize(x) for x in val['t']]
            )
        else:
            return ErrorModelRef()
    elif typ is InstructionRef:
        if 'n' in val:
            return InstructionRef(
                val['n'],
                [cqasm.v1.types.Node.deserialize(x) for x in val['t']],
                val['c'],
                val['p'],
                val['r'],
                val['d']
            )
        else:
            return InstructionRef()
    else:
        assert False
