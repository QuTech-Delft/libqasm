import functools
import struct
import cqasm.v1.primitives


_typemap = {}


def _cbor_read_intlike(cbor, offset, info):
    """Parses the additional information and reads any additional bytes it
    specifies the existence of, and returns the encoded integer. offset
    should point to the byte immediately following the initial byte. Returns
    the encoded integer and the offset immediately following the object."""

    # Info less than 24 is a shorthand for the integer itself.
    if info < 24:
        return info, offset

    # 24 is 8-bit following the info byte.
    if info == 24:
        return cbor[offset], offset + 1

    # 25 is 16-bit following the info byte.
    if info == 25:
        val, = struct.unpack('>H', cbor[offset:offset+2])
        return val, offset + 2

    # 26 is 32-bit following the info byte.
    if info == 26:
        val, = struct.unpack('>I', cbor[offset:offset+4])
        return val, offset + 4

    # 27 is 64-bit following the info byte.
    if info == 27:
        val, = struct.unpack('>Q', cbor[offset:offset+8])
        return val, offset + 8

    # Info greater than or equal to 28 is illegal. Note that 31 is used for
    # indefinite lengths, so this must be checked prior to calling this
    # method.
    raise ValueError("invalid CBOR: illegal additional info for integer or object length")


def _sub_cbor_to_py(cbor, offset):
    """Converts the CBOR object starting at cbor[offset] to its Python
    representation for as far as tree-gen supports CBOR. Returns this Python
    representation and the offset immediately following the CBOR representation
    thereof. Supported types:

     - 0: unsigned integer (int)
     - 1: negative integer (int)
     - 2: byte string (bytes)
     - 3: UTF-8 string (str)
     - 4: array (list)
     - 5: map (dict)
     - 6: semantic tag (ignored)
     - 7.20: false (bool)
     - 7.21: true (bool)
     - 7.22: null (NoneType)
     - 7.27: double-precision float (float)

    Both definite-length and indefinite-length notation is supported for sized
    objects (strings, arrays, maps). A ValueError is thrown if the CBOR is
    invalid or contains unsupported structures."""

    # Read the initial byte.
    initial = cbor[offset]
    typ = initial >> 5
    info = initial & 0x1F
    offset += 1

    # Handle unsigned integer (0) and negative integer (1).
    if typ <= 1:
        value, offset = _cbor_read_intlike(cbor, offset, info)
        if typ == 1:
            value = -1 - value
        return value, offset

    # Handle byte string (2) and UTF-8 string (3).
    if typ <= 3:

        # Gather components of the string in here.
        if info == 31:

            # Handle indefinite length strings. These consist of a
            # break-terminated (0xFF) list of definite-length strings of the
            # same type.
            value = []
            while True:
                sub_initial = cbor[offset]; offset += 1
                if sub_initial == 0xFF:
                    break
                sub_typ = sub_initial >> 5
                sub_info = sub_initial & 0x1F
                if sub_typ != typ:
                    raise ValueError('invalid CBOR: illegal indefinite-length string component')

                # Seek past definite-length string component. The size in
                # bytes is encoded as an integer.
                size, offset = _cbor_read_intlike(cbor, offset, sub_info)
                value.append(cbor[offset:offset + size])
                offset += size
            value = b''.join(value)

        else:

            # Handle definite-length strings. The size in bytes is encoded as
            # an integer.
            size, offset = _cbor_read_intlike(cbor, offset, info)
            value = cbor[offset:offset + size]
            offset += size

        if typ == 3:
            value = value.decode('UTF-8')
        return value, offset

    # Handle array (4) and map (5).
    if typ <= 5:

        # Create result container.
        container = [] if typ == 4 else {}

        # Handle indefinite length arrays and maps.
        if info == 31:

            # Read objects/object pairs until we encounter a break.
            while cbor[offset] != 0xFF:
                if typ == 4:
                    value, offset = _sub_cbor_to_py(cbor, offset)
                    container.append(value)
                else:
                    key, offset = _sub_cbor_to_py(cbor, offset)
                    if not isinstance(key, str):
                        raise ValueError('invalid CBOR: map key is not a UTF-8 string')
                    value, offset = _sub_cbor_to_py(cbor, offset)
                    container[key] = value

            # Seek past the break.
            offset += 1

        else:

            # Handle definite-length arrays and maps. The amount of
            # objects/object pairs is encoded as an integer.
            size, offset = _cbor_read_intlike(cbor, offset, info)
            for _ in range(size):
                if typ == 4:
                    value, offset = _sub_cbor_to_py(cbor, offset)
                    container.append(value)
                else:
                    key, offset = _sub_cbor_to_py(cbor, offset)
                    if not isinstance(key, str):
                        raise ValueError('invalid CBOR: map key is not a UTF-8 string')
                    value, offset = _sub_cbor_to_py(cbor, offset)
                    container[key] = value

        return container, offset

    # Handle semantic tags.
    if typ == 6:

        # We don't use semantic tags for anything, but ignoring them is
        # legal and reading past them is easy enough.
        _, offset = _cbor_read_intlike(cbor, offset, info)
        return _sub_cbor_to_py(cbor, offset)

    # Handle major type 7. Here, the type is defined by the additional info.
    # Additional info 24 is reserved for having the type specified by the
    # next byte, but all such values are unassigned.
    if info == 20:
        # false
        return False, offset

    if info == 21:
        # true
        return True, offset

    if info == 22:
        # null
        return None, offset

    if info == 23:
        # Undefined value.
        raise ValueError('invalid CBOR: undefined value is not supported')

    if info == 25:
        # Half-precision float.
        raise ValueError('invalid CBOR: half-precision float is not supported')

    if info == 26:
        # Single-precision float.
        raise ValueError('invalid CBOR: single-precision float is not supported')

    if info == 27:
        # Double-precision float.
        value, = struct.unpack('>d', cbor[offset:offset+8])
        return value, offset + 8

    if info == 31:
        # Break value used for indefinite-length objects.
        raise ValueError('invalid CBOR: unexpected break')

    raise ValueError('invalid CBOR: unknown type code')


def _cbor_to_py(cbor):
    """Converts the given CBOR object (bytes) to its Python representation for
    as far as tree-gen supports CBOR. Supported types:

     - 0: unsigned integer (int)
     - 1: negative integer (int)
     - 2: byte string (bytes)
     - 3: UTF-8 string (str)
     - 4: array (list)
     - 5: map (dict)
     - 6: semantic tag (ignored)
     - 7.20: false (bool)
     - 7.21: true (bool)
     - 7.22: null (NoneType)
     - 7.27: double-precision float (float)

    Both definite-length and indefinite-length notation is supported for sized
    objects (strings, arrays, maps). A ValueError is thrown if the CBOR is
    invalid or contains unsupported structures."""

    value, length = _sub_cbor_to_py(cbor, 0)
    if length < len(cbor):
        raise ValueError('invalid CBOR: garbage at the end')
    return value


class _Cbor(bytes):
    """Marker class indicating that this bytes object represents CBOR."""
    pass


def _cbor_write_intlike(value, major=0):
    """Converts the given integer to its minimal representation in CBOR. The
    major code can be overridden to write lengths for strings, arrays, and
    maps."""

    # Negative integers use major code 1.
    if value < 0:
        major = 1
        value = -1 - value
    initial = major << 5

    # Use the minimal representation.
    if value < 24:
        return struct.pack('>B', initial | value)
    if value < 0x100:
        return struct.pack('>BB', initial | 24, value)
    if value < 0x10000:
        return struct.pack('>BH', initial | 25, value)
    if value < 0x100000000:
        return struct.pack('>BI', initial | 26, value)
    if value < 0x10000000000000000:
        return struct.pack('>BQ', initial | 27, value)

    raise ValueError('integer too large for CBOR (bigint not supported)')


def _py_to_cbor(value, type_converter=None):
    """Inverse of _cbor_to_py(). type_converter optionally specifies a function
    that takes a value and either converts it to a primitive for serialization,
    converts it to a _Cbor object manually, or raises a TypeError if no
    conversion is known. If no type_converter is specified, a TypeError is
    raised in all cases the type_converter would otherwise be called. The cbor
    serialization is returned using a _Cbor object, which is just a marker class
    behaving just like bytes."""
    if isinstance(value, _Cbor):
        return value

    if isinstance(value, int):
        return _Cbor(_cbor_write_intlike(value))

    if isinstance(value, float):
        return _Cbor(struct.pack('>Bd', 0xFB, value))

    if isinstance(value, str):
        value = value.encode('UTF-8')
        return _Cbor(_cbor_write_intlike(len(value), 3) + value)

    if isinstance(value, bytes):
        return _Cbor(_cbor_write_intlike(len(value), 2) + value)

    if value is False:
        return _Cbor(b'\xF4')

    if value is True:
        return _Cbor(b'\xF5')

    if value is None:
        return _Cbor(b'\xF6')

    if isinstance(value, (list, tuple)):
        cbor = [_cbor_write_intlike(len(value), 4)]
        for val in value:
            cbor.append(_py_to_cbor(val, type_converter))
        return _Cbor(b''.join(cbor))

    if isinstance(value, dict):
        cbor = [_cbor_write_intlike(len(value), 5)]
        for key, val in sorted(value.items()):
            if not isinstance(key, str):
                raise TypeError('dict keys must be strings')
            cbor.append(_py_to_cbor(key, type_converter))
            cbor.append(_py_to_cbor(val, type_converter))
        return _Cbor(b''.join(cbor))

    if type_converter is not None:
        return _py_to_cbor(type_converter(value))

    raise TypeError('unsupported type for conversion to cbor: %r' % (value,))


class NotWellFormed(ValueError):
    """Exception class for well-formedness checks."""

    def __init__(self, msg):
        super().__init__('not well-formed: ' + str(msg))


class Node(object):
    """Base class for nodes."""

    __slots__ = ['_annot']

    def __init__(self):
        super().__init__()
        self._annot = {}

    def __getitem__(self, key):
        """Returns the annotation object with the specified key, or raises
        KeyError if not found."""
        if not isinstance(key, str):
            raise TypeError('indexing a node with something other than an '
                            'annotation key string')
        return self._annot[key]

    def __setitem__(self, key, val):
        """Assigns the annotation object with the specified key."""
        if not isinstance(key, str):
            raise TypeError('indexing a node with something other than an '
                            'annotation key string')
        self._annot[key] = val

    def __delitem__(self, key):
        """Deletes the annotation object with the specified key."""
        if not isinstance(key, str):
            raise TypeError('indexing a node with something other than an '
                            'annotation key string')
        del self._annot[key]

    def __contains__(self, key):
        """Returns whether an annotation exists for the specified key."""
        return key in self._annot

    @staticmethod
    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it. Note that this is overridden
        by the actual node class implementations; this base function does very
        little."""
        if id_map is None:
            id_map = {}
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes. Note that this is
        overridden by the actual node class implementations; this base function
        always raises an exception."""
        raise NotWellFormed('found node of abstract type ' + type(self).__name__)

    def check_well_formed(self):
        """Checks whether the tree starting at this node is well-formed. That
        is:

         - all One, Link, and Many edges have (at least) one entry;
         - all the One entries internally stored by Any/Many have an entry;
         - all Link and filled OptLink nodes link to a node that's reachable
           from this node;
         - the nodes referred to be One/Maybe only appear once in the tree
           (except through links).

        If it isn't well-formed, a NotWellFormed is thrown."""
        self.check_complete()

    def is_well_formed(self):
        """Returns whether the tree starting at this node is well-formed. That
        is:

         - all One, Link, and Many edges have (at least) one entry;
         - all the One entries internally stored by Any/Many have an entry;
         - all Link and filled OptLink nodes link to a node that's reachable
           from this node;
         - the nodes referred to be One/Maybe only appear once in the tree
           (except through links)."""
        try:
            self.check_well_formed()
            return True
        except NotWellFormed:
            return False

    def copy(self):
        """Returns a shallow copy of this node. Note that this is overridden by
        the actual node class implementations; this base function always raises
        an exception."""
        raise TypeError('can\'t copy node of abstract type ' + type(self).__name__)

    def clone(self):
        """Returns a deep copy of this node. Note that this is overridden by
        the actual node class implementations; this base function always raises
        an exception."""
        raise TypeError('can\'t clone node of abstract type ' + type(self).__name__)

    @classmethod
    def deserialize(cls, cbor):
        """Attempts to deserialize the given cbor object (either as bytes or as
        its Python primitive representation) into a node of this type."""
        if isinstance(cbor, bytes):
            cbor = _cbor_to_py(cbor)
        seq_to_ob = {}
        links = []
        root = cls._deserialize(cbor, seq_to_ob, links)
        for link_setter, seq in links:
            ob = seq_to_ob.get(seq, None)
            if ob is None:
                raise ValueError('found link to nonexistent object')
            link_setter(ob)
        return root

    def serialize(self):
        """Serializes this node into its cbor representation in the form of a
        bytes object."""
        id_map = self.find_reachable()
        self.check_complete(id_map)
        return _py_to_cbor(self._serialize(id_map))

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        node_type = _typemap.get(cbor.get('@t'), None)
        if node_type is None:
            raise ValueError('unknown node type (@t): ' + str(cbor.get('@t')))
        return node_type._deserialize(cbor, seq_to_ob, links)


@functools.total_ordering
class _Multiple(object):
    """Base class for the Any* and Many* edge helper classes. Inheriting
    classes must set the class constant _T to the node type they are made
    for."""

    __slots__ = ['_l']

    def __init__(self,  *args, **kwargs):
        super().__init__()
        self._l = list(*args, **kwargs)
        for idx, val in enumerate(self._l):
            if not isinstance(val, self._T):
                raise TypeError(
                    'object {!r} at index {:d} is not an instance of {!r}'
                    .format(val, idx, self._T))

    def __repr__(self):
        return '{}({!r})'.format(type(self).__name__, self._l)

    def clone(self):
        return self.__class__(map(lambda node: node.clone(), self._l))

    def __len__(self):
        return len(self._l)

    def __getitem__(self, idx):
        return self._l[idx]

    def __setitem__(self, idx, val):
        if not isinstance(val, self._T):
            raise TypeError(
                'object {!r} is not an instance of {!r}'
                .format(val, idx, self._T))
        self._l[idx] = val

    def __delitem__(self, idx):
        del self._l[idx]

    def __iter__(self):
        return iter(self._l)

    def __reversed__(self):
        return reversed(self._l)

    def __contains__(self, val):
        return val in self._l

    def append(self, val):
        if not isinstance(val, self._T):
            raise TypeError(
                'object {!r} is not an instance of {!r}'
                .format(val, self._T))
        self._l.append(val)

    def extend(self, iterable):
        for val in iterable:
            self.append(val)

    def insert(self, idx, val):
        if not isinstance(val, self._T):
            raise TypeError(
                'object {!r} is not an instance of {!r}'
                .format(val, self._T))
        self._l.insert(idx, val)

    def remote(self, val):
        self._l.remove(val)

    def pop(self, idx=-1):
        return self._l.pop(idx)

    def clear(self):
        self._l.clear()

    def idx(self, val, start=0, end=-1):
        return self._l.idx(val, start, end)

    def count(self, val):
        return self._l.count(val)

    def sort(self, key=None, reverse=False):
        self._l.sort(key=key, reverse=reverse)

    def reverse(self):
        self._l.reverse()

    def copy(self):
        return self.__class__(self)

    def __eq__(self, other):
        if not isinstance(other, _Multiple):
            return False
        return self._l == other._l

    def __lt__(self, other):
        return self._l < other._l

    def __iadd__(self, other):
        self.extend(other)

    def __add__(self, other):
        copy = self.copy()
        copy += other
        return copy

    def __imul__(self, other):
        self._l *= other

    def __mul__(self, other):
        copy = self.copy()
        copy *= other
        return copy

    def __rmul__(self, other):
        copy = self.copy()
        copy *= other
        return copy


class MultiNode(_Multiple):
    """Wrapper for an edge with multiple Node objects."""

    _T = Node


def _cloned(obj):
    """Attempts to clone the given object by calling its clone() method, if it
    has one."""
    if hasattr(obj, 'clone'):
        return obj.clone()
    return obj


class Expression(Node):
    """Any kind of expression."""

    __slots__ = []

    def __init__(self):
        super().__init__()

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'IntegerLiteral':
            return IntegerLiteral._deserialize(cbor, seq_to_ob, links)
        if typ == 'FloatLiteral':
            return FloatLiteral._deserialize(cbor, seq_to_ob, links)
        if typ == 'Identifier':
            return Identifier._deserialize(cbor, seq_to_ob, links)
        if typ == 'MatrixLiteral':
            return MatrixLiteral._deserialize(cbor, seq_to_ob, links)
        if typ == 'StringLiteral':
            return StringLiteral._deserialize(cbor, seq_to_ob, links)
        if typ == 'JsonLiteral':
            return JsonLiteral._deserialize(cbor, seq_to_ob, links)
        if typ == 'FunctionCall':
            return FunctionCall._deserialize(cbor, seq_to_ob, links)
        if typ == 'Index':
            return Index._deserialize(cbor, seq_to_ob, links)
        if typ == 'Negate':
            return Negate._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseNot':
            return BitwiseNot._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalNot':
            return LogicalNot._deserialize(cbor, seq_to_ob, links)
        if typ == 'Power':
            return Power._deserialize(cbor, seq_to_ob, links)
        if typ == 'Multiply':
            return Multiply._deserialize(cbor, seq_to_ob, links)
        if typ == 'Divide':
            return Divide._deserialize(cbor, seq_to_ob, links)
        if typ == 'IntDivide':
            return IntDivide._deserialize(cbor, seq_to_ob, links)
        if typ == 'Modulo':
            return Modulo._deserialize(cbor, seq_to_ob, links)
        if typ == 'Add':
            return Add._deserialize(cbor, seq_to_ob, links)
        if typ == 'Subtract':
            return Subtract._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftLeft':
            return ShiftLeft._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightArith':
            return ShiftRightArith._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightLogic':
            return ShiftRightLogic._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpEq':
            return CmpEq._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpNe':
            return CmpNe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGt':
            return CmpGt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGe':
            return CmpGe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLt':
            return CmpLt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLe':
            return CmpLe._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseAnd':
            return BitwiseAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseXor':
            return BitwiseXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseOr':
            return BitwiseOr._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalAnd':
            return LogicalAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalXor':
            return LogicalXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalOr':
            return LogicalOr._deserialize(cbor, seq_to_ob, links)
        if typ == 'TernaryCond':
            return TernaryCond._deserialize(cbor, seq_to_ob, links)
        if typ == 'ErroneousExpression':
            return ErroneousExpression._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Expression'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiExpression(_Multiple):
    """Wrapper for an edge with multiple Expression objects."""

    _T = Expression


_typemap['Expression'] = Expression

class BinaryOp(Expression):
    """Any binary operator."""

    __slots__ = [
        '_attr_lhs',
        '_attr_rhs',
    ]

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__()
        self.lhs = lhs
        self.rhs = rhs

    @property
    def lhs(self):
        """The left-hand side of the expression."""
        return self._attr_lhs

    @lhs.setter
    def lhs(self, val):
        if val is None:
            del self.lhs
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('lhs must be of type Expression')
            val = Expression(val)
        self._attr_lhs = val

    @lhs.deleter
    def lhs(self):
        self._attr_lhs = None

    @property
    def rhs(self):
        """The right-hand side of the expression."""
        return self._attr_rhs

    @rhs.setter
    def rhs(self, val):
        if val is None:
            del self.rhs
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('rhs must be of type Expression')
            val = Expression(val)
        self._attr_rhs = val

    @rhs.deleter
    def rhs(self):
        self._attr_rhs = None

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Power':
            return Power._deserialize(cbor, seq_to_ob, links)
        if typ == 'Multiply':
            return Multiply._deserialize(cbor, seq_to_ob, links)
        if typ == 'Divide':
            return Divide._deserialize(cbor, seq_to_ob, links)
        if typ == 'IntDivide':
            return IntDivide._deserialize(cbor, seq_to_ob, links)
        if typ == 'Modulo':
            return Modulo._deserialize(cbor, seq_to_ob, links)
        if typ == 'Add':
            return Add._deserialize(cbor, seq_to_ob, links)
        if typ == 'Subtract':
            return Subtract._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftLeft':
            return ShiftLeft._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightArith':
            return ShiftRightArith._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightLogic':
            return ShiftRightLogic._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpEq':
            return CmpEq._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpNe':
            return CmpNe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGt':
            return CmpGt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGe':
            return CmpGe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLt':
            return CmpLt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLe':
            return CmpLe._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseAnd':
            return BitwiseAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseXor':
            return BitwiseXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseOr':
            return BitwiseOr._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalAnd':
            return LogicalAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalXor':
            return LogicalXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalOr':
            return LogicalOr._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BinaryOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBinaryOp(_Multiple):
    """Wrapper for an edge with multiple BinaryOp objects."""

    _T = BinaryOp


_typemap['BinaryOp'] = BinaryOp

class ArithOp(BinaryOp):
    """Arithmetic operators."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Power':
            return Power._deserialize(cbor, seq_to_ob, links)
        if typ == 'Multiply':
            return Multiply._deserialize(cbor, seq_to_ob, links)
        if typ == 'Divide':
            return Divide._deserialize(cbor, seq_to_ob, links)
        if typ == 'IntDivide':
            return IntDivide._deserialize(cbor, seq_to_ob, links)
        if typ == 'Modulo':
            return Modulo._deserialize(cbor, seq_to_ob, links)
        if typ == 'Add':
            return Add._deserialize(cbor, seq_to_ob, links)
        if typ == 'Subtract':
            return Subtract._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ArithOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiArithOp(_Multiple):
    """Wrapper for an edge with multiple ArithOp objects."""

    _T = ArithOp


_typemap['ArithOp'] = ArithOp

class Add(ArithOp):
    """Addition operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Add):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Add(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Add(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Add(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Add':
            raise ValueError('found node serialization for ' + typ + ', but expected Add')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Add node.
        node = Add(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Add'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiAdd(_Multiple):
    """Wrapper for an edge with multiple Add objects."""

    _T = Add


_typemap['Add'] = Add

class Annotated(Node):
    """Represents a node that carries annotation data."""

    __slots__ = [
        '_attr_annotations',
    ]

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__()
        self.annotations = annotations

    @property
    def annotations(self):
        """Zero or more annotations attached to this object."""
        return self._attr_annotations

    @annotations.setter
    def annotations(self, val):
        if val is None:
            del self.annotations
            return
        if not isinstance(val, MultiAnnotationData):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('annotations must be of type MultiAnnotationData')
            val = MultiAnnotationData(val)
        self._attr_annotations = val

    @annotations.deleter
    def annotations(self):
        self._attr_annotations = MultiAnnotationData()

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Instruction':
            return Instruction._deserialize(cbor, seq_to_ob, links)
        if typ == 'Bundle':
            return Bundle._deserialize(cbor, seq_to_ob, links)
        if typ == 'Mapping':
            return Mapping._deserialize(cbor, seq_to_ob, links)
        if typ == 'Variables':
            return Variables._deserialize(cbor, seq_to_ob, links)
        if typ == 'Subcircuit':
            return Subcircuit._deserialize(cbor, seq_to_ob, links)
        if typ == 'IfElse':
            return IfElse._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForLoop':
            return ForLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForeachLoop':
            return ForeachLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'WhileLoop':
            return WhileLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'RepeatUntilLoop':
            return RepeatUntilLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'BreakStatement':
            return BreakStatement._deserialize(cbor, seq_to_ob, links)
        if typ == 'ContinueStatement':
            return ContinueStatement._deserialize(cbor, seq_to_ob, links)
        if typ == 'ErroneousStatement':
            return ErroneousStatement._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Annotated'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiAnnotated(_Multiple):
    """Wrapper for an edge with multiple Annotated objects."""

    _T = Annotated


_typemap['Annotated'] = Annotated

class AnnotationData(Node):
    """Represents an annotation."""

    __slots__ = [
        '_attr_interface',
        '_attr_operation',
        '_attr_operands',
    ]

    def __init__(
        self,
        interface=None,
        operation=None,
        operands=None,
    ):
        super().__init__()
        self.interface = interface
        self.operation = operation
        self.operands = operands

    @property
    def interface(self):
        """The interface this annotation is intended for. If a target doesn't
        support an interface, it should silently ignore the annotation."""
        return self._attr_interface

    @interface.setter
    def interface(self, val):
        if val is None:
            del self.interface
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('interface must be of type Identifier')
            val = Identifier(val)
        self._attr_interface = val

    @interface.deleter
    def interface(self):
        self._attr_interface = None

    @property
    def operation(self):
        """The operation within the interface that this annotation is intended
        for. If a supports the corresponding interface but not the operation, it
        should throw an error."""
        return self._attr_operation

    @operation.setter
    def operation(self, val):
        if val is None:
            del self.operation
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('operation must be of type Identifier')
            val = Identifier(val)
        self._attr_operation = val

    @operation.deleter
    def operation(self):
        self._attr_operation = None

    @property
    def operands(self):
        """Any operands attached to the annotation."""
        return self._attr_operands

    @operands.setter
    def operands(self, val):
        if val is None:
            del self.operands
            return
        if not isinstance(val, ExpressionList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('operands must be of type ExpressionList')
            val = ExpressionList(val)
        self._attr_operands = val

    @operands.deleter
    def operands(self):
        self._attr_operands = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, AnnotationData):
            return False
        if self.interface != other.interface:
            return False
        if self.operation != other.operation:
            return False
        if self.operands != other.operands:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('AnnotationData(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('interface: ')
        if self.interface is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.interface.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('operation: ')
        if self.operation is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.operation.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('operands: ')
        if self.operands is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.operands.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_interface is not None:
            self._attr_interface.find_reachable(id_map)
        if self._attr_operation is not None:
            self._attr_operation.find_reachable(id_map)
        if self._attr_operands is not None:
            self._attr_operands.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_interface is None:
            raise NotWellFormed('interface is required but not set')
        if self._attr_interface is not None:
            self._attr_interface.check_complete(id_map)
        if self._attr_operation is None:
            raise NotWellFormed('operation is required but not set')
        if self._attr_operation is not None:
            self._attr_operation.check_complete(id_map)
        if self._attr_operands is None:
            raise NotWellFormed('operands is required but not set')
        if self._attr_operands is not None:
            self._attr_operands.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return AnnotationData(
            interface=self._attr_interface,
            operation=self._attr_operation,
            operands=self._attr_operands
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return AnnotationData(
            interface=_cloned(self._attr_interface),
            operation=_cloned(self._attr_operation),
            operands=_cloned(self._attr_operands)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'AnnotationData':
            raise ValueError('found node serialization for ' + typ + ', but expected AnnotationData')

        # Deserialize the interface field.
        field = cbor.get('interface', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field interface')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field interface')
        if field.get('@t', None) is None:
            f_interface = None
        else:
            f_interface = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the operation field.
        field = cbor.get('operation', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field operation')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field operation')
        if field.get('@t', None) is None:
            f_operation = None
        else:
            f_operation = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the operands field.
        field = cbor.get('operands', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field operands')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field operands')
        if field.get('@t', None) is None:
            f_operands = None
        else:
            f_operands = ExpressionList._deserialize(field, seq_to_ob, links)

        # Construct the AnnotationData node.
        node = AnnotationData(f_interface, f_operation, f_operands)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'AnnotationData'}

        # Serialize the interface field.
        field = {'@T': '1'}
        if self._attr_interface is None:
            field['@t'] = None
        else:
            field.update(self._attr_interface._serialize(id_map))
        cbor['interface'] = field

        # Serialize the operation field.
        field = {'@T': '1'}
        if self._attr_operation is None:
            field['@t'] = None
        else:
            field.update(self._attr_operation._serialize(id_map))
        cbor['operation'] = field

        # Serialize the operands field.
        field = {'@T': '1'}
        if self._attr_operands is None:
            field['@t'] = None
        else:
            field.update(self._attr_operands._serialize(id_map))
        cbor['operands'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiAnnotationData(_Multiple):
    """Wrapper for an edge with multiple AnnotationData objects."""

    _T = AnnotationData


_typemap['AnnotationData'] = AnnotationData

class Assignment(Node):
    """An assignment statement within a loop definition."""

    __slots__ = [
        '_attr_lhs',
        '_attr_rhs',
    ]

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__()
        self.lhs = lhs
        self.rhs = rhs

    @property
    def lhs(self):
        """Name of the variable being assigned."""
        return self._attr_lhs

    @lhs.setter
    def lhs(self, val):
        if val is None:
            del self.lhs
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('lhs must be of type Expression')
            val = Expression(val)
        self._attr_lhs = val

    @lhs.deleter
    def lhs(self):
        self._attr_lhs = None

    @property
    def rhs(self):
        """Value being assigned to the variable."""
        return self._attr_rhs

    @rhs.setter
    def rhs(self, val):
        if val is None:
            del self.rhs
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('rhs must be of type Expression')
            val = Expression(val)
        self._attr_rhs = val

    @rhs.deleter
    def rhs(self):
        self._attr_rhs = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Assignment):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Assignment(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Assignment(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Assignment(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Assignment':
            raise ValueError('found node serialization for ' + typ + ', but expected Assignment')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Assignment node.
        node = Assignment(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Assignment'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiAssignment(_Multiple):
    """Wrapper for an edge with multiple Assignment objects."""

    _T = Assignment


_typemap['Assignment'] = Assignment

class BitwiseOp(BinaryOp):
    """Bitwise operators."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'BitwiseAnd':
            return BitwiseAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseXor':
            return BitwiseXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseOr':
            return BitwiseOr._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BitwiseOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitwiseOp(_Multiple):
    """Wrapper for an edge with multiple BitwiseOp objects."""

    _T = BitwiseOp


_typemap['BitwiseOp'] = BitwiseOp

class BitwiseAnd(BitwiseOp):
    """Bitwise AND operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BitwiseAnd):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('BitwiseAnd(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BitwiseAnd(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BitwiseAnd(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'BitwiseAnd':
            raise ValueError('found node serialization for ' + typ + ', but expected BitwiseAnd')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the BitwiseAnd node.
        node = BitwiseAnd(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BitwiseAnd'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitwiseAnd(_Multiple):
    """Wrapper for an edge with multiple BitwiseAnd objects."""

    _T = BitwiseAnd


_typemap['BitwiseAnd'] = BitwiseAnd

class UnaryOp(Expression):
    """Any unary operator."""

    __slots__ = [
        '_attr_expr',
    ]

    def __init__(
        self,
        expr=None,
    ):
        super().__init__()
        self.expr = expr

    @property
    def expr(self):
        """The expression being operated on."""
        return self._attr_expr

    @expr.setter
    def expr(self, val):
        if val is None:
            del self.expr
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('expr must be of type Expression')
            val = Expression(val)
        self._attr_expr = val

    @expr.deleter
    def expr(self):
        self._attr_expr = None

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Negate':
            return Negate._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitwiseNot':
            return BitwiseNot._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalNot':
            return LogicalNot._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'UnaryOp'}

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiUnaryOp(_Multiple):
    """Wrapper for an edge with multiple UnaryOp objects."""

    _T = UnaryOp


_typemap['UnaryOp'] = UnaryOp

class BitwiseNot(UnaryOp):
    """Bitwise NOT (one's complement)."""

    __slots__ = []

    def __init__(
        self,
        expr=None,
    ):
        super().__init__(expr=expr)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BitwiseNot):
            return False
        if self.expr != other.expr:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('BitwiseNot(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('expr: ')
        if self.expr is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.expr.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_expr is not None:
            self._attr_expr.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_expr is None:
            raise NotWellFormed('expr is required but not set')
        if self._attr_expr is not None:
            self._attr_expr.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BitwiseNot(
            expr=self._attr_expr
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BitwiseNot(
            expr=_cloned(self._attr_expr)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'BitwiseNot':
            raise ValueError('found node serialization for ' + typ + ', but expected BitwiseNot')

        # Deserialize the expr field.
        field = cbor.get('expr', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field expr')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field expr')
        if field.get('@t', None) is None:
            f_expr = None
        else:
            f_expr = Expression._deserialize(field, seq_to_ob, links)

        # Construct the BitwiseNot node.
        node = BitwiseNot(f_expr)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BitwiseNot'}

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitwiseNot(_Multiple):
    """Wrapper for an edge with multiple BitwiseNot objects."""

    _T = BitwiseNot


_typemap['BitwiseNot'] = BitwiseNot

class BitwiseOr(BitwiseOp):
    """Bitwise OR operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BitwiseOr):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('BitwiseOr(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BitwiseOr(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BitwiseOr(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'BitwiseOr':
            raise ValueError('found node serialization for ' + typ + ', but expected BitwiseOr')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the BitwiseOr node.
        node = BitwiseOr(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BitwiseOr'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitwiseOr(_Multiple):
    """Wrapper for an edge with multiple BitwiseOr objects."""

    _T = BitwiseOr


_typemap['BitwiseOr'] = BitwiseOr

class BitwiseXor(BitwiseOp):
    """Bitwise XOR operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BitwiseXor):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('BitwiseXor(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BitwiseXor(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BitwiseXor(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'BitwiseXor':
            raise ValueError('found node serialization for ' + typ + ', but expected BitwiseXor')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the BitwiseXor node.
        node = BitwiseXor(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BitwiseXor'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitwiseXor(_Multiple):
    """Wrapper for an edge with multiple BitwiseXor objects."""

    _T = BitwiseXor


_typemap['BitwiseXor'] = BitwiseXor

class Statement(Annotated):
    """Any kind of statement."""

    __slots__ = []

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__(annotations=annotations)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Bundle':
            return Bundle._deserialize(cbor, seq_to_ob, links)
        if typ == 'Mapping':
            return Mapping._deserialize(cbor, seq_to_ob, links)
        if typ == 'Variables':
            return Variables._deserialize(cbor, seq_to_ob, links)
        if typ == 'Subcircuit':
            return Subcircuit._deserialize(cbor, seq_to_ob, links)
        if typ == 'IfElse':
            return IfElse._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForLoop':
            return ForLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForeachLoop':
            return ForeachLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'WhileLoop':
            return WhileLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'RepeatUntilLoop':
            return RepeatUntilLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'BreakStatement':
            return BreakStatement._deserialize(cbor, seq_to_ob, links)
        if typ == 'ContinueStatement':
            return ContinueStatement._deserialize(cbor, seq_to_ob, links)
        if typ == 'ErroneousStatement':
            return ErroneousStatement._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Statement'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiStatement(_Multiple):
    """Wrapper for an edge with multiple Statement objects."""

    _T = Statement


_typemap['Statement'] = Statement

class Structured(Statement):
    """Any version 1.2+ structured control-flow statement."""

    __slots__ = []

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__(annotations=annotations)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'IfElse':
            return IfElse._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForLoop':
            return ForLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'ForeachLoop':
            return ForeachLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'WhileLoop':
            return WhileLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'RepeatUntilLoop':
            return RepeatUntilLoop._deserialize(cbor, seq_to_ob, links)
        if typ == 'BreakStatement':
            return BreakStatement._deserialize(cbor, seq_to_ob, links)
        if typ == 'ContinueStatement':
            return ContinueStatement._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Structured'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiStructured(_Multiple):
    """Wrapper for an edge with multiple Structured objects."""

    _T = Structured


_typemap['Structured'] = Structured

class BreakStatement(Structured):
    """A break statement."""

    __slots__ = []

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__(annotations=annotations)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BreakStatement):
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('BreakStatement(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BreakStatement(
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BreakStatement(
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'BreakStatement':
            raise ValueError('found node serialization for ' + typ + ', but expected BreakStatement')

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the BreakStatement node.
        node = BreakStatement(f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'BreakStatement'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBreakStatement(_Multiple):
    """Wrapper for an edge with multiple BreakStatement objects."""

    _T = BreakStatement


_typemap['BreakStatement'] = BreakStatement

class Bundle(Statement):
    """A list of parallel instructions."""

    __slots__ = [
        '_attr_items',
    ]

    def __init__(
        self,
        items=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.items = items

    @property
    def items(self):
        """The list of parallel instructions."""
        return self._attr_items

    @items.setter
    def items(self, val):
        if val is None:
            del self.items
            return
        if not isinstance(val, MultiInstruction):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type MultiInstruction')
            val = MultiInstruction(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = MultiInstruction()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Bundle):
            return False
        if self.items != other.items:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Bundle(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('items: ')
        if not self.items:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.items:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_items:
            el.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_items:
            raise NotWellFormed('items needs at least one node but has zero')
        for child in self._attr_items:
            child.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Bundle(
            items=self._attr_items.copy(),
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Bundle(
            items=_cloned(self._attr_items),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Bundle':
            raise ValueError('found node serialization for ' + typ + ', but expected Bundle')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field items')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_items = MultiInstruction()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_items.append(Instruction._deserialize(element, seq_to_ob, links))

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the Bundle node.
        node = Bundle(f_items, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Bundle'}

        # Serialize the items field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_items:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['items'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBundle(_Multiple):
    """Wrapper for an edge with multiple Bundle objects."""

    _T = Bundle


_typemap['Bundle'] = Bundle

class CmpOp(BinaryOp):
    """Comparison operators."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'CmpEq':
            return CmpEq._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpNe':
            return CmpNe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGt':
            return CmpGt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpGe':
            return CmpGe._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLt':
            return CmpLt._deserialize(cbor, seq_to_ob, links)
        if typ == 'CmpLe':
            return CmpLe._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpOp(_Multiple):
    """Wrapper for an edge with multiple CmpOp objects."""

    _T = CmpOp


_typemap['CmpOp'] = CmpOp

class CmpEq(CmpOp):
    """Equality operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpEq):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpEq(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpEq(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpEq(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpEq':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpEq')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpEq node.
        node = CmpEq(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpEq'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpEq(_Multiple):
    """Wrapper for an edge with multiple CmpEq objects."""

    _T = CmpEq


_typemap['CmpEq'] = CmpEq

class CmpGe(CmpOp):
    """Greater-than-or-equal operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpGe):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpGe(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpGe(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpGe(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpGe':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpGe')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpGe node.
        node = CmpGe(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpGe'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpGe(_Multiple):
    """Wrapper for an edge with multiple CmpGe objects."""

    _T = CmpGe


_typemap['CmpGe'] = CmpGe

class CmpGt(CmpOp):
    """Greater-than operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpGt):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpGt(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpGt(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpGt(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpGt':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpGt')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpGt node.
        node = CmpGt(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpGt'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpGt(_Multiple):
    """Wrapper for an edge with multiple CmpGt objects."""

    _T = CmpGt


_typemap['CmpGt'] = CmpGt

class CmpLe(CmpOp):
    """Less-than-or-equal operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpLe):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpLe(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpLe(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpLe(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpLe':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpLe')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpLe node.
        node = CmpLe(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpLe'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpLe(_Multiple):
    """Wrapper for an edge with multiple CmpLe objects."""

    _T = CmpLe


_typemap['CmpLe'] = CmpLe

class CmpLt(CmpOp):
    """Less-than operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpLt):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpLt(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpLt(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpLt(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpLt':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpLt')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpLt node.
        node = CmpLt(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpLt'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpLt(_Multiple):
    """Wrapper for an edge with multiple CmpLt objects."""

    _T = CmpLt


_typemap['CmpLt'] = CmpLt

class CmpNe(CmpOp):
    """Inequality operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, CmpNe):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('CmpNe(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return CmpNe(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return CmpNe(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'CmpNe':
            raise ValueError('found node serialization for ' + typ + ', but expected CmpNe')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the CmpNe node.
        node = CmpNe(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'CmpNe'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiCmpNe(_Multiple):
    """Wrapper for an edge with multiple CmpNe objects."""

    _T = CmpNe


_typemap['CmpNe'] = CmpNe

class ContinueStatement(Structured):
    """A continue statement."""

    __slots__ = []

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__(annotations=annotations)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ContinueStatement):
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ContinueStatement(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ContinueStatement(
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ContinueStatement(
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ContinueStatement':
            raise ValueError('found node serialization for ' + typ + ', but expected ContinueStatement')

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the ContinueStatement node.
        node = ContinueStatement(f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ContinueStatement'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiContinueStatement(_Multiple):
    """Wrapper for an edge with multiple ContinueStatement objects."""

    _T = ContinueStatement


_typemap['ContinueStatement'] = ContinueStatement

class Divide(ArithOp):
    """True division operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Divide):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Divide(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Divide(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Divide(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Divide':
            raise ValueError('found node serialization for ' + typ + ', but expected Divide')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Divide node.
        node = Divide(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Divide'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiDivide(_Multiple):
    """Wrapper for an edge with multiple Divide objects."""

    _T = Divide


_typemap['Divide'] = Divide

class ErroneousExpression(Expression):
    """Placeholder for an expression with a parse error."""

    __slots__ = []

    def __init__(self):
        super().__init__()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ErroneousExpression):
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ErroneousExpression(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return ErroneousExpression(

        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ErroneousExpression(

        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ErroneousExpression':
            raise ValueError('found node serialization for ' + typ + ', but expected ErroneousExpression')

        # Construct the ErroneousExpression node.
        node = ErroneousExpression()

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ErroneousExpression'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiErroneousExpression(_Multiple):
    """Wrapper for an edge with multiple ErroneousExpression objects."""

    _T = ErroneousExpression


_typemap['ErroneousExpression'] = ErroneousExpression

class Root(Node):
    """Any root node for the AST."""

    __slots__ = []

    def __init__(self):
        super().__init__()

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'Program':
            return Program._deserialize(cbor, seq_to_ob, links)
        if typ == 'ErroneousProgram':
            return ErroneousProgram._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Root'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiRoot(_Multiple):
    """Wrapper for an edge with multiple Root objects."""

    _T = Root


_typemap['Root'] = Root

class ErroneousProgram(Root):
    """Placeholder for a program with a parse error."""

    __slots__ = []

    def __init__(self):
        super().__init__()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ErroneousProgram):
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ErroneousProgram(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return ErroneousProgram(

        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ErroneousProgram(

        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ErroneousProgram':
            raise ValueError('found node serialization for ' + typ + ', but expected ErroneousProgram')

        # Construct the ErroneousProgram node.
        node = ErroneousProgram()

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ErroneousProgram'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiErroneousProgram(_Multiple):
    """Wrapper for an edge with multiple ErroneousProgram objects."""

    _T = ErroneousProgram


_typemap['ErroneousProgram'] = ErroneousProgram

class ErroneousStatement(Statement):
    """Placeholder for a statement with a parse error."""

    __slots__ = []

    def __init__(
        self,
        annotations=None,
    ):
        super().__init__(annotations=annotations)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ErroneousStatement):
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ErroneousStatement(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ErroneousStatement(
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ErroneousStatement(
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ErroneousStatement':
            raise ValueError('found node serialization for ' + typ + ', but expected ErroneousStatement')

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the ErroneousStatement node.
        node = ErroneousStatement(f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ErroneousStatement'}

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiErroneousStatement(_Multiple):
    """Wrapper for an edge with multiple ErroneousStatement objects."""

    _T = ErroneousStatement


_typemap['ErroneousStatement'] = ErroneousStatement

class ExpressionList(Node):
    """Represents a comma-separated list of expressions."""

    __slots__ = [
        '_attr_items',
    ]

    def __init__(
        self,
        items=None,
    ):
        super().__init__()
        self.items = items

    @property
    def items(self):
        """The list of expressions."""
        return self._attr_items

    @items.setter
    def items(self, val):
        if val is None:
            del self.items
            return
        if not isinstance(val, MultiExpression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type MultiExpression')
            val = MultiExpression(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = MultiExpression()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ExpressionList):
            return False
        if self.items != other.items:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ExpressionList(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('items: ')
        if not self.items:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.items:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_items:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_items:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ExpressionList(
            items=self._attr_items.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ExpressionList(
            items=_cloned(self._attr_items)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ExpressionList':
            raise ValueError('found node serialization for ' + typ + ', but expected ExpressionList')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field items')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_items = MultiExpression()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_items.append(Expression._deserialize(element, seq_to_ob, links))

        # Construct the ExpressionList node.
        node = ExpressionList(f_items)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ExpressionList'}

        # Serialize the items field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_items:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['items'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiExpressionList(_Multiple):
    """Wrapper for an edge with multiple ExpressionList objects."""

    _T = ExpressionList


_typemap['ExpressionList'] = ExpressionList

class FloatLiteral(Expression):
    """A floating point literal."""

    __slots__ = [
        '_attr_value',
    ]

    def __init__(
        self,
        value=None,
    ):
        super().__init__()
        self.value = value

    @property
    def value(self):
        """The floating point number."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Real):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Real')
            val = cqasm.v1.primitives.Real(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Real()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, FloatLiteral):
            return False
        if self.value != other.value:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('FloatLiteral(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('value: ')
        s.append(str(self.value) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return FloatLiteral(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return FloatLiteral(
            value=_cloned(self._attr_value)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'FloatLiteral':
            raise ValueError('found node serialization for ' + typ + ', but expected FloatLiteral')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Real, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Real.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Real, field)

        # Construct the FloatLiteral node.
        node = FloatLiteral(f_value)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'FloatLiteral'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Real, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiFloatLiteral(_Multiple):
    """Wrapper for an edge with multiple FloatLiteral objects."""

    _T = FloatLiteral


_typemap['FloatLiteral'] = FloatLiteral

class ForLoop(Structured):
    """A C-style for loop."""

    __slots__ = [
        '_attr_initialize',
        '_attr_condition',
        '_attr_update',
        '_attr_body',
    ]

    def __init__(
        self,
        initialize=None,
        condition=None,
        update=None,
        body=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.initialize = initialize
        self.condition = condition
        self.update = update
        self.body = body

    @property
    def initialize(self):
        """The optional initializing assignment, run before the loop starts."""
        return self._attr_initialize

    @initialize.setter
    def initialize(self, val):
        if val is None:
            del self.initialize
            return
        if not isinstance(val, Assignment):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('initialize must be of type Assignment')
            val = Assignment(val)
        self._attr_initialize = val

    @initialize.deleter
    def initialize(self):
        self._attr_initialize = None

    @property
    def condition(self):
        """The condition for starting another iteration."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type Expression')
            val = Expression(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

    @property
    def update(self):
        """The updating assignment, done at the end of the loop body and upon
         continue."""
        return self._attr_update

    @update.setter
    def update(self, val):
        if val is None:
            del self.update
            return
        if not isinstance(val, Assignment):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('update must be of type Assignment')
            val = Assignment(val)
        self._attr_update = val

    @update.deleter
    def update(self):
        self._attr_update = None

    @property
    def body(self):
        """The loop body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type StatementList')
            val = StatementList(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ForLoop):
            return False
        if self.initialize != other.initialize:
            return False
        if self.condition != other.condition:
            return False
        if self.update != other.update:
            return False
        if self.body != other.body:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ForLoop(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('initialize: ')
        if self.initialize is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.initialize.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('update: ')
        if self.update is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.update.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.body.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_initialize is not None:
            self._attr_initialize.find_reachable(id_map)
        if self._attr_condition is not None:
            self._attr_condition.find_reachable(id_map)
        if self._attr_update is not None:
            self._attr_update.find_reachable(id_map)
        if self._attr_body is not None:
            self._attr_body.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_initialize is not None:
            self._attr_initialize.check_complete(id_map)
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        if self._attr_update is not None:
            self._attr_update.check_complete(id_map)
        if self._attr_body is None:
            raise NotWellFormed('body is required but not set')
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ForLoop(
            initialize=self._attr_initialize,
            condition=self._attr_condition,
            update=self._attr_update,
            body=self._attr_body,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ForLoop(
            initialize=_cloned(self._attr_initialize),
            condition=_cloned(self._attr_condition),
            update=_cloned(self._attr_update),
            body=_cloned(self._attr_body),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ForLoop':
            raise ValueError('found node serialization for ' + typ + ', but expected ForLoop')

        # Deserialize the initialize field.
        field = cbor.get('initialize', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field initialize')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field initialize')
        if field.get('@t', None) is None:
            f_initialize = None
        else:
            f_initialize = Assignment._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the update field.
        field = cbor.get('update', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field update')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field update')
        if field.get('@t', None) is None:
            f_update = None
        else:
            f_update = Assignment._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = StatementList._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the ForLoop node.
        node = ForLoop(f_initialize, f_condition, f_update, f_body, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ForLoop'}

        # Serialize the initialize field.
        field = {'@T': '?'}
        if self._attr_initialize is None:
            field['@t'] = None
        else:
            field.update(self._attr_initialize._serialize(id_map))
        cbor['initialize'] = field

        # Serialize the condition field.
        field = {'@T': '1'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the update field.
        field = {'@T': '?'}
        if self._attr_update is None:
            field['@t'] = None
        else:
            field.update(self._attr_update._serialize(id_map))
        cbor['update'] = field

        # Serialize the body field.
        field = {'@T': '1'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiForLoop(_Multiple):
    """Wrapper for an edge with multiple ForLoop objects."""

    _T = ForLoop


_typemap['ForLoop'] = ForLoop

class ForeachLoop(Structured):
    """A foreach loop. Note that this is rather dumbed-down in cQASM 1.2
    compared to the cQASM 2.0 syntax it was inspired from, and is only usable as
    a range-based loop."""

    __slots__ = [
        '_attr_lhs',
        '_attr_frm',
        '_attr_to',
        '_attr_body',
    ]

    def __init__(
        self,
        lhs=None,
        frm=None,
        to=None,
        body=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.lhs = lhs
        self.frm = frm
        self.to = to
        self.body = body

    @property
    def lhs(self):
        """Reference to the variable used for looping."""
        return self._attr_lhs

    @lhs.setter
    def lhs(self, val):
        if val is None:
            del self.lhs
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('lhs must be of type Expression')
            val = Expression(val)
        self._attr_lhs = val

    @lhs.deleter
    def lhs(self):
        self._attr_lhs = None

    @property
    def frm(self):
        """The first value."""
        return self._attr_frm

    @frm.setter
    def frm(self, val):
        if val is None:
            del self.frm
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('frm must be of type Expression')
            val = Expression(val)
        self._attr_frm = val

    @frm.deleter
    def frm(self):
        self._attr_frm = None

    @property
    def to(self):
        """The last value."""
        return self._attr_to

    @to.setter
    def to(self, val):
        if val is None:
            del self.to
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('to must be of type Expression')
            val = Expression(val)
        self._attr_to = val

    @to.deleter
    def to(self):
        self._attr_to = None

    @property
    def body(self):
        """The loop body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type StatementList')
            val = StatementList(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ForeachLoop):
            return False
        if self.lhs != other.lhs:
            return False
        if self.frm != other.frm:
            return False
        if self.to != other.to:
            return False
        if self.body != other.body:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ForeachLoop(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('frm: ')
        if self.frm is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.frm.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('to: ')
        if self.to is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.to.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.body.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_frm is not None:
            self._attr_frm.find_reachable(id_map)
        if self._attr_to is not None:
            self._attr_to.find_reachable(id_map)
        if self._attr_body is not None:
            self._attr_body.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_frm is None:
            raise NotWellFormed('frm is required but not set')
        if self._attr_frm is not None:
            self._attr_frm.check_complete(id_map)
        if self._attr_to is None:
            raise NotWellFormed('to is required but not set')
        if self._attr_to is not None:
            self._attr_to.check_complete(id_map)
        if self._attr_body is None:
            raise NotWellFormed('body is required but not set')
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ForeachLoop(
            lhs=self._attr_lhs,
            frm=self._attr_frm,
            to=self._attr_to,
            body=self._attr_body,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ForeachLoop(
            lhs=_cloned(self._attr_lhs),
            frm=_cloned(self._attr_frm),
            to=_cloned(self._attr_to),
            body=_cloned(self._attr_body),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ForeachLoop':
            raise ValueError('found node serialization for ' + typ + ', but expected ForeachLoop')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the frm field.
        field = cbor.get('frm', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field frm')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field frm')
        if field.get('@t', None) is None:
            f_frm = None
        else:
            f_frm = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the to field.
        field = cbor.get('to', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field to')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field to')
        if field.get('@t', None) is None:
            f_to = None
        else:
            f_to = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = StatementList._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the ForeachLoop node.
        node = ForeachLoop(f_lhs, f_frm, f_to, f_body, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ForeachLoop'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the frm field.
        field = {'@T': '1'}
        if self._attr_frm is None:
            field['@t'] = None
        else:
            field.update(self._attr_frm._serialize(id_map))
        cbor['frm'] = field

        # Serialize the to field.
        field = {'@T': '1'}
        if self._attr_to is None:
            field['@t'] = None
        else:
            field.update(self._attr_to._serialize(id_map))
        cbor['to'] = field

        # Serialize the body field.
        field = {'@T': '1'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiForeachLoop(_Multiple):
    """Wrapper for an edge with multiple ForeachLoop objects."""

    _T = ForeachLoop


_typemap['ForeachLoop'] = ForeachLoop

class FunctionCall(Expression):
    """A function call."""

    __slots__ = [
        '_attr_name',
        '_attr_arguments',
    ]

    def __init__(
        self,
        name=None,
        arguments=None,
    ):
        super().__init__()
        self.name = name
        self.arguments = arguments

    @property
    def name(self):
        """The name of the function."""
        return self._attr_name

    @name.setter
    def name(self, val):
        if val is None:
            del self.name
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('name must be of type Identifier')
            val = Identifier(val)
        self._attr_name = val

    @name.deleter
    def name(self):
        self._attr_name = None

    @property
    def arguments(self):
        """The function arguments."""
        return self._attr_arguments

    @arguments.setter
    def arguments(self, val):
        if val is None:
            del self.arguments
            return
        if not isinstance(val, ExpressionList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('arguments must be of type ExpressionList')
            val = ExpressionList(val)
        self._attr_arguments = val

    @arguments.deleter
    def arguments(self):
        self._attr_arguments = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, FunctionCall):
            return False
        if self.name != other.name:
            return False
        if self.arguments != other.arguments:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('FunctionCall(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('name: ')
        if self.name is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.name.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('arguments: ')
        if self.arguments is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.arguments.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_name is not None:
            self._attr_name.find_reachable(id_map)
        if self._attr_arguments is not None:
            self._attr_arguments.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_name is None:
            raise NotWellFormed('name is required but not set')
        if self._attr_name is not None:
            self._attr_name.check_complete(id_map)
        if self._attr_arguments is None:
            raise NotWellFormed('arguments is required but not set')
        if self._attr_arguments is not None:
            self._attr_arguments.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return FunctionCall(
            name=self._attr_name,
            arguments=self._attr_arguments
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return FunctionCall(
            name=_cloned(self._attr_name),
            arguments=_cloned(self._attr_arguments)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'FunctionCall':
            raise ValueError('found node serialization for ' + typ + ', but expected FunctionCall')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field name')
        if field.get('@t', None) is None:
            f_name = None
        else:
            f_name = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the arguments field.
        field = cbor.get('arguments', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field arguments')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field arguments')
        if field.get('@t', None) is None:
            f_arguments = None
        else:
            f_arguments = ExpressionList._deserialize(field, seq_to_ob, links)

        # Construct the FunctionCall node.
        node = FunctionCall(f_name, f_arguments)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'FunctionCall'}

        # Serialize the name field.
        field = {'@T': '1'}
        if self._attr_name is None:
            field['@t'] = None
        else:
            field.update(self._attr_name._serialize(id_map))
        cbor['name'] = field

        # Serialize the arguments field.
        field = {'@T': '1'}
        if self._attr_arguments is None:
            field['@t'] = None
        else:
            field.update(self._attr_arguments._serialize(id_map))
        cbor['arguments'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiFunctionCall(_Multiple):
    """Wrapper for an edge with multiple FunctionCall objects."""

    _T = FunctionCall


_typemap['FunctionCall'] = FunctionCall

class Identifier(Expression):
    """An identifier."""

    __slots__ = [
        '_attr_name',
    ]

    def __init__(
        self,
        name=None,
    ):
        super().__init__()
        self.name = name

    @property
    def name(self):
        """The identifier."""
        return self._attr_name

    @name.setter
    def name(self, val):
        if val is None:
            del self.name
            return
        if not isinstance(val, cqasm.v1.primitives.Str):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('name must be of type cqasm.v1.primitives.Str')
            val = cqasm.v1.primitives.Str(val)
        self._attr_name = val

    @name.deleter
    def name(self):
        self._attr_name = cqasm.v1.primitives.Str()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Identifier):
            return False
        if self.name != other.name:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Identifier(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('name: ')
        s.append(str(self.name) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return Identifier(
            name=self._attr_name
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Identifier(
            name=_cloned(self._attr_name)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Identifier':
            raise ValueError('found node serialization for ' + typ + ', but expected Identifier')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Construct the Identifier node.
        node = Identifier(f_name)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Identifier'}

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIdentifier(_Multiple):
    """Wrapper for an edge with multiple Identifier objects."""

    _T = Identifier


_typemap['Identifier'] = Identifier

class IfElse(Structured):
    """An if-else chain."""

    __slots__ = [
        '_attr_branches',
        '_attr_otherwise',
    ]

    def __init__(
        self,
        branches=None,
        otherwise=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.branches = branches
        self.otherwise = otherwise

    @property
    def branches(self):
        """The if-else branches."""
        return self._attr_branches

    @branches.setter
    def branches(self, val):
        if val is None:
            del self.branches
            return
        if not isinstance(val, MultiIfElseBranch):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('branches must be of type MultiIfElseBranch')
            val = MultiIfElseBranch(val)
        self._attr_branches = val

    @branches.deleter
    def branches(self):
        self._attr_branches = MultiIfElseBranch()

    @property
    def otherwise(self):
        """The final else block, if any."""
        return self._attr_otherwise

    @otherwise.setter
    def otherwise(self, val):
        if val is None:
            del self.otherwise
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('otherwise must be of type StatementList')
            val = StatementList(val)
        self._attr_otherwise = val

    @otherwise.deleter
    def otherwise(self):
        self._attr_otherwise = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IfElse):
            return False
        if self.branches != other.branches:
            return False
        if self.otherwise != other.otherwise:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IfElse(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('branches: ')
        if not self.branches:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.branches:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('otherwise: ')
        if self.otherwise is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.otherwise.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_branches:
            el.find_reachable(id_map)
        if self._attr_otherwise is not None:
            self._attr_otherwise.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_branches:
            raise NotWellFormed('branches needs at least one node but has zero')
        for child in self._attr_branches:
            child.check_complete(id_map)
        if self._attr_otherwise is not None:
            self._attr_otherwise.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IfElse(
            branches=self._attr_branches.copy(),
            otherwise=self._attr_otherwise,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IfElse(
            branches=_cloned(self._attr_branches),
            otherwise=_cloned(self._attr_otherwise),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IfElse':
            raise ValueError('found node serialization for ' + typ + ', but expected IfElse')

        # Deserialize the branches field.
        field = cbor.get('branches', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field branches')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field branches')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_branches = MultiIfElseBranch()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_branches.append(IfElseBranch._deserialize(element, seq_to_ob, links))

        # Deserialize the otherwise field.
        field = cbor.get('otherwise', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field otherwise')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field otherwise')
        if field.get('@t', None) is None:
            f_otherwise = None
        else:
            f_otherwise = StatementList._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the IfElse node.
        node = IfElse(f_branches, f_otherwise, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IfElse'}

        # Serialize the branches field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_branches:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['branches'] = field

        # Serialize the otherwise field.
        field = {'@T': '?'}
        if self._attr_otherwise is None:
            field['@t'] = None
        else:
            field.update(self._attr_otherwise._serialize(id_map))
        cbor['otherwise'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIfElse(_Multiple):
    """Wrapper for an edge with multiple IfElse objects."""

    _T = IfElse


_typemap['IfElse'] = IfElse

class IfElseBranch(Node):
    """A single condition + block for use in an if-else chain."""

    __slots__ = [
        '_attr_condition',
        '_attr_body',
    ]

    def __init__(
        self,
        condition=None,
        body=None,
    ):
        super().__init__()
        self.condition = condition
        self.body = body

    @property
    def condition(self):
        """The condition."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type Expression')
            val = Expression(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

    @property
    def body(self):
        """The body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type StatementList')
            val = StatementList(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IfElseBranch):
            return False
        if self.condition != other.condition:
            return False
        if self.body != other.body:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IfElseBranch(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.body.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_condition is not None:
            self._attr_condition.find_reachable(id_map)
        if self._attr_body is not None:
            self._attr_body.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        if self._attr_body is None:
            raise NotWellFormed('body is required but not set')
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IfElseBranch(
            condition=self._attr_condition,
            body=self._attr_body
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IfElseBranch(
            condition=_cloned(self._attr_condition),
            body=_cloned(self._attr_body)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IfElseBranch':
            raise ValueError('found node serialization for ' + typ + ', but expected IfElseBranch')

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = StatementList._deserialize(field, seq_to_ob, links)

        # Construct the IfElseBranch node.
        node = IfElseBranch(f_condition, f_body)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IfElseBranch'}

        # Serialize the condition field.
        field = {'@T': '1'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the body field.
        field = {'@T': '1'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIfElseBranch(_Multiple):
    """Wrapper for an edge with multiple IfElseBranch objects."""

    _T = IfElseBranch


_typemap['IfElseBranch'] = IfElseBranch

class Index(Expression):
    """Indexation operator."""

    __slots__ = [
        '_attr_expr',
        '_attr_indices',
    ]

    def __init__(
        self,
        expr=None,
        indices=None,
    ):
        super().__init__()
        self.expr = expr
        self.indices = indices

    @property
    def expr(self):
        """The expression being indexed."""
        return self._attr_expr

    @expr.setter
    def expr(self, val):
        if val is None:
            del self.expr
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('expr must be of type Expression')
            val = Expression(val)
        self._attr_expr = val

    @expr.deleter
    def expr(self):
        self._attr_expr = None

    @property
    def indices(self):
        """The list of indices."""
        return self._attr_indices

    @indices.setter
    def indices(self, val):
        if val is None:
            del self.indices
            return
        if not isinstance(val, IndexList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('indices must be of type IndexList')
            val = IndexList(val)
        self._attr_indices = val

    @indices.deleter
    def indices(self):
        self._attr_indices = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Index):
            return False
        if self.expr != other.expr:
            return False
        if self.indices != other.indices:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Index(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('expr: ')
        if self.expr is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.expr.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('indices: ')
        if self.indices is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.indices.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_expr is not None:
            self._attr_expr.find_reachable(id_map)
        if self._attr_indices is not None:
            self._attr_indices.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_expr is None:
            raise NotWellFormed('expr is required but not set')
        if self._attr_expr is not None:
            self._attr_expr.check_complete(id_map)
        if self._attr_indices is None:
            raise NotWellFormed('indices is required but not set')
        if self._attr_indices is not None:
            self._attr_indices.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Index(
            expr=self._attr_expr,
            indices=self._attr_indices
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Index(
            expr=_cloned(self._attr_expr),
            indices=_cloned(self._attr_indices)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Index':
            raise ValueError('found node serialization for ' + typ + ', but expected Index')

        # Deserialize the expr field.
        field = cbor.get('expr', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field expr')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field expr')
        if field.get('@t', None) is None:
            f_expr = None
        else:
            f_expr = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the indices field.
        field = cbor.get('indices', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field indices')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field indices')
        if field.get('@t', None) is None:
            f_indices = None
        else:
            f_indices = IndexList._deserialize(field, seq_to_ob, links)

        # Construct the Index node.
        node = Index(f_expr, f_indices)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Index'}

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize the indices field.
        field = {'@T': '1'}
        if self._attr_indices is None:
            field['@t'] = None
        else:
            field.update(self._attr_indices._serialize(id_map))
        cbor['indices'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIndex(_Multiple):
    """Wrapper for an edge with multiple Index objects."""

    _T = Index


_typemap['Index'] = Index

class IndexEntry(Node):
    """An entry in an index list. Can be a single index or a range."""

    __slots__ = []

    def __init__(self):
        super().__init__()

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'IndexItem':
            return IndexItem._deserialize(cbor, seq_to_ob, links)
        if typ == 'IndexRange':
            return IndexRange._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IndexEntry'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIndexEntry(_Multiple):
    """Wrapper for an edge with multiple IndexEntry objects."""

    _T = IndexEntry


_typemap['IndexEntry'] = IndexEntry

class IndexItem(IndexEntry):
    """A single index in an index list."""

    __slots__ = [
        '_attr_index',
    ]

    def __init__(
        self,
        index=None,
    ):
        super().__init__()
        self.index = index

    @property
    def index(self):
        """An integer expression representing the index."""
        return self._attr_index

    @index.setter
    def index(self, val):
        if val is None:
            del self.index
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('index must be of type Expression')
            val = Expression(val)
        self._attr_index = val

    @index.deleter
    def index(self):
        self._attr_index = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IndexItem):
            return False
        if self.index != other.index:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IndexItem(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('index: ')
        if self.index is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.index.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_index is not None:
            self._attr_index.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_index is None:
            raise NotWellFormed('index is required but not set')
        if self._attr_index is not None:
            self._attr_index.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IndexItem(
            index=self._attr_index
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IndexItem(
            index=_cloned(self._attr_index)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IndexItem':
            raise ValueError('found node serialization for ' + typ + ', but expected IndexItem')

        # Deserialize the index field.
        field = cbor.get('index', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field index')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field index')
        if field.get('@t', None) is None:
            f_index = None
        else:
            f_index = Expression._deserialize(field, seq_to_ob, links)

        # Construct the IndexItem node.
        node = IndexItem(f_index)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IndexItem'}

        # Serialize the index field.
        field = {'@T': '1'}
        if self._attr_index is None:
            field['@t'] = None
        else:
            field.update(self._attr_index._serialize(id_map))
        cbor['index'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIndexItem(_Multiple):
    """Wrapper for an edge with multiple IndexItem objects."""

    _T = IndexItem


_typemap['IndexItem'] = IndexItem

class IndexList(Node):
    """A list of one or more indices."""

    __slots__ = [
        '_attr_items',
    ]

    def __init__(
        self,
        items=None,
    ):
        super().__init__()
        self.items = items

    @property
    def items(self):
        """The list of indices."""
        return self._attr_items

    @items.setter
    def items(self, val):
        if val is None:
            del self.items
            return
        if not isinstance(val, MultiIndexEntry):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type MultiIndexEntry')
            val = MultiIndexEntry(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = MultiIndexEntry()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IndexList):
            return False
        if self.items != other.items:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IndexList(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('items: ')
        if not self.items:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.items:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_items:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_items:
            raise NotWellFormed('items needs at least one node but has zero')
        for child in self._attr_items:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IndexList(
            items=self._attr_items.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IndexList(
            items=_cloned(self._attr_items)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IndexList':
            raise ValueError('found node serialization for ' + typ + ', but expected IndexList')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field items')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_items = MultiIndexEntry()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_items.append(IndexEntry._deserialize(element, seq_to_ob, links))

        # Construct the IndexList node.
        node = IndexList(f_items)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IndexList'}

        # Serialize the items field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_items:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['items'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIndexList(_Multiple):
    """Wrapper for an edge with multiple IndexList objects."""

    _T = IndexList


_typemap['IndexList'] = IndexList

class IndexRange(IndexEntry):
    """An inclusive range of indices in an index list."""

    __slots__ = [
        '_attr_first',
        '_attr_last',
    ]

    def __init__(
        self,
        first=None,
        last=None,
    ):
        super().__init__()
        self.first = first
        self.last = last

    @property
    def first(self):
        """An integer expression representing the first index."""
        return self._attr_first

    @first.setter
    def first(self, val):
        if val is None:
            del self.first
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('first must be of type Expression')
            val = Expression(val)
        self._attr_first = val

    @first.deleter
    def first(self):
        self._attr_first = None

    @property
    def last(self):
        """An integer expression representing the last index."""
        return self._attr_last

    @last.setter
    def last(self, val):
        if val is None:
            del self.last
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('last must be of type Expression')
            val = Expression(val)
        self._attr_last = val

    @last.deleter
    def last(self):
        self._attr_last = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IndexRange):
            return False
        if self.first != other.first:
            return False
        if self.last != other.last:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IndexRange(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('first: ')
        if self.first is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.first.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('last: ')
        if self.last is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.last.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_first is not None:
            self._attr_first.find_reachable(id_map)
        if self._attr_last is not None:
            self._attr_last.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_first is None:
            raise NotWellFormed('first is required but not set')
        if self._attr_first is not None:
            self._attr_first.check_complete(id_map)
        if self._attr_last is None:
            raise NotWellFormed('last is required but not set')
        if self._attr_last is not None:
            self._attr_last.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IndexRange(
            first=self._attr_first,
            last=self._attr_last
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IndexRange(
            first=_cloned(self._attr_first),
            last=_cloned(self._attr_last)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IndexRange':
            raise ValueError('found node serialization for ' + typ + ', but expected IndexRange')

        # Deserialize the first field.
        field = cbor.get('first', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field first')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field first')
        if field.get('@t', None) is None:
            f_first = None
        else:
            f_first = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the last field.
        field = cbor.get('last', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field last')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field last')
        if field.get('@t', None) is None:
            f_last = None
        else:
            f_last = Expression._deserialize(field, seq_to_ob, links)

        # Construct the IndexRange node.
        node = IndexRange(f_first, f_last)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IndexRange'}

        # Serialize the first field.
        field = {'@T': '1'}
        if self._attr_first is None:
            field['@t'] = None
        else:
            field.update(self._attr_first._serialize(id_map))
        cbor['first'] = field

        # Serialize the last field.
        field = {'@T': '1'}
        if self._attr_last is None:
            field['@t'] = None
        else:
            field.update(self._attr_last._serialize(id_map))
        cbor['last'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIndexRange(_Multiple):
    """Wrapper for an edge with multiple IndexRange objects."""

    _T = IndexRange


_typemap['IndexRange'] = IndexRange

class Instruction(Annotated):
    """Any kind of instruction. Note that this is NOT a statement; instructions
    are always considered part of a parallel bundle, even if they're on their
     own."""

    __slots__ = [
        '_attr_name',
        '_attr_condition',
        '_attr_operands',
    ]

    def __init__(
        self,
        name=None,
        condition=None,
        operands=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.name = name
        self.condition = condition
        self.operands = operands

    @property
    def name(self):
        """Name identifying the instruction."""
        return self._attr_name

    @name.setter
    def name(self, val):
        if val is None:
            del self.name
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('name must be of type Identifier')
            val = Identifier(val)
        self._attr_name = val

    @name.deleter
    def name(self):
        self._attr_name = None

    @property
    def condition(self):
        """Optional conditional expression."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type Expression')
            val = Expression(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

    @property
    def operands(self):
        """Operands for the instruction."""
        return self._attr_operands

    @operands.setter
    def operands(self, val):
        if val is None:
            del self.operands
            return
        if not isinstance(val, ExpressionList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('operands must be of type ExpressionList')
            val = ExpressionList(val)
        self._attr_operands = val

    @operands.deleter
    def operands(self):
        self._attr_operands = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Instruction):
            return False
        if self.name != other.name:
            return False
        if self.condition != other.condition:
            return False
        if self.operands != other.operands:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Instruction(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('name: ')
        if self.name is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.name.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('operands: ')
        if self.operands is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.operands.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_name is not None:
            self._attr_name.find_reachable(id_map)
        if self._attr_condition is not None:
            self._attr_condition.find_reachable(id_map)
        if self._attr_operands is not None:
            self._attr_operands.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_name is None:
            raise NotWellFormed('name is required but not set')
        if self._attr_name is not None:
            self._attr_name.check_complete(id_map)
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        if self._attr_operands is None:
            raise NotWellFormed('operands is required but not set')
        if self._attr_operands is not None:
            self._attr_operands.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Instruction(
            name=self._attr_name,
            condition=self._attr_condition,
            operands=self._attr_operands,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Instruction(
            name=_cloned(self._attr_name),
            condition=_cloned(self._attr_condition),
            operands=_cloned(self._attr_operands),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Instruction':
            raise ValueError('found node serialization for ' + typ + ', but expected Instruction')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field name')
        if field.get('@t', None) is None:
            f_name = None
        else:
            f_name = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the operands field.
        field = cbor.get('operands', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field operands')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field operands')
        if field.get('@t', None) is None:
            f_operands = None
        else:
            f_operands = ExpressionList._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the Instruction node.
        node = Instruction(f_name, f_condition, f_operands, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Instruction'}

        # Serialize the name field.
        field = {'@T': '1'}
        if self._attr_name is None:
            field['@t'] = None
        else:
            field.update(self._attr_name._serialize(id_map))
        cbor['name'] = field

        # Serialize the condition field.
        field = {'@T': '?'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the operands field.
        field = {'@T': '1'}
        if self._attr_operands is None:
            field['@t'] = None
        else:
            field.update(self._attr_operands._serialize(id_map))
        cbor['operands'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiInstruction(_Multiple):
    """Wrapper for an edge with multiple Instruction objects."""

    _T = Instruction


_typemap['Instruction'] = Instruction

class IntDivide(ArithOp):
    """Integer division operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IntDivide):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IntDivide(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return IntDivide(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IntDivide(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IntDivide':
            raise ValueError('found node serialization for ' + typ + ', but expected IntDivide')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the IntDivide node.
        node = IntDivide(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IntDivide'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIntDivide(_Multiple):
    """Wrapper for an edge with multiple IntDivide objects."""

    _T = IntDivide


_typemap['IntDivide'] = IntDivide

class IntegerLiteral(Expression):
    """An integer literal."""

    __slots__ = [
        '_attr_value',
    ]

    def __init__(
        self,
        value=None,
    ):
        super().__init__()
        self.value = value

    @property
    def value(self):
        """The integer."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Int):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Int')
            val = cqasm.v1.primitives.Int(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Int()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, IntegerLiteral):
            return False
        if self.value != other.value:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('IntegerLiteral(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('value: ')
        s.append(str(self.value) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return IntegerLiteral(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return IntegerLiteral(
            value=_cloned(self._attr_value)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'IntegerLiteral':
            raise ValueError('found node serialization for ' + typ + ', but expected IntegerLiteral')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Construct the IntegerLiteral node.
        node = IntegerLiteral(f_value)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'IntegerLiteral'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiIntegerLiteral(_Multiple):
    """Wrapper for an edge with multiple IntegerLiteral objects."""

    _T = IntegerLiteral


_typemap['IntegerLiteral'] = IntegerLiteral

class JsonLiteral(Expression):
    """Represents a JSON literal."""

    __slots__ = [
        '_attr_value',
    ]

    def __init__(
        self,
        value=None,
    ):
        super().__init__()
        self.value = value

    @property
    def value(self):
        """The JSON literal."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Str):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Str')
            val = cqasm.v1.primitives.Str(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Str()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, JsonLiteral):
            return False
        if self.value != other.value:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('JsonLiteral(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('value: ')
        s.append(str(self.value) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return JsonLiteral(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return JsonLiteral(
            value=_cloned(self._attr_value)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'JsonLiteral':
            raise ValueError('found node serialization for ' + typ + ', but expected JsonLiteral')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Construct the JsonLiteral node.
        node = JsonLiteral(f_value)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'JsonLiteral'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiJsonLiteral(_Multiple):
    """Wrapper for an edge with multiple JsonLiteral objects."""

    _T = JsonLiteral


_typemap['JsonLiteral'] = JsonLiteral

class LogicalOp(BinaryOp):
    """Logical operators."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'LogicalAnd':
            return LogicalAnd._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalXor':
            return LogicalXor._deserialize(cbor, seq_to_ob, links)
        if typ == 'LogicalOr':
            return LogicalOr._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'LogicalOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiLogicalOp(_Multiple):
    """Wrapper for an edge with multiple LogicalOp objects."""

    _T = LogicalOp


_typemap['LogicalOp'] = LogicalOp

class LogicalAnd(LogicalOp):
    """Logical AND operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, LogicalAnd):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('LogicalAnd(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return LogicalAnd(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return LogicalAnd(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'LogicalAnd':
            raise ValueError('found node serialization for ' + typ + ', but expected LogicalAnd')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the LogicalAnd node.
        node = LogicalAnd(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'LogicalAnd'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiLogicalAnd(_Multiple):
    """Wrapper for an edge with multiple LogicalAnd objects."""

    _T = LogicalAnd


_typemap['LogicalAnd'] = LogicalAnd

class LogicalNot(UnaryOp):
    """Logical/boolean NOT."""

    __slots__ = []

    def __init__(
        self,
        expr=None,
    ):
        super().__init__(expr=expr)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, LogicalNot):
            return False
        if self.expr != other.expr:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('LogicalNot(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('expr: ')
        if self.expr is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.expr.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_expr is not None:
            self._attr_expr.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_expr is None:
            raise NotWellFormed('expr is required but not set')
        if self._attr_expr is not None:
            self._attr_expr.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return LogicalNot(
            expr=self._attr_expr
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return LogicalNot(
            expr=_cloned(self._attr_expr)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'LogicalNot':
            raise ValueError('found node serialization for ' + typ + ', but expected LogicalNot')

        # Deserialize the expr field.
        field = cbor.get('expr', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field expr')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field expr')
        if field.get('@t', None) is None:
            f_expr = None
        else:
            f_expr = Expression._deserialize(field, seq_to_ob, links)

        # Construct the LogicalNot node.
        node = LogicalNot(f_expr)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'LogicalNot'}

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiLogicalNot(_Multiple):
    """Wrapper for an edge with multiple LogicalNot objects."""

    _T = LogicalNot


_typemap['LogicalNot'] = LogicalNot

class LogicalOr(LogicalOp):
    """Logical OR operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, LogicalOr):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('LogicalOr(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return LogicalOr(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return LogicalOr(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'LogicalOr':
            raise ValueError('found node serialization for ' + typ + ', but expected LogicalOr')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the LogicalOr node.
        node = LogicalOr(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'LogicalOr'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiLogicalOr(_Multiple):
    """Wrapper for an edge with multiple LogicalOr objects."""

    _T = LogicalOr


_typemap['LogicalOr'] = LogicalOr

class LogicalXor(LogicalOp):
    """Logical XOR operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, LogicalXor):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('LogicalXor(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return LogicalXor(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return LogicalXor(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'LogicalXor':
            raise ValueError('found node serialization for ' + typ + ', but expected LogicalXor')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the LogicalXor node.
        node = LogicalXor(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'LogicalXor'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiLogicalXor(_Multiple):
    """Wrapper for an edge with multiple LogicalXor objects."""

    _T = LogicalXor


_typemap['LogicalXor'] = LogicalXor

class Mapping(Statement):
    """A mapping (alias) for an expression. Originally just a way of naming a
    single qubit."""

    __slots__ = [
        '_attr_alias',
        '_attr_expr',
    ]

    def __init__(
        self,
        alias=None,
        expr=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.alias = alias
        self.expr = expr

    @property
    def alias(self):
        """The identifier used to refer to the expression."""
        return self._attr_alias

    @alias.setter
    def alias(self, val):
        if val is None:
            del self.alias
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('alias must be of type Identifier')
            val = Identifier(val)
        self._attr_alias = val

    @alias.deleter
    def alias(self):
        self._attr_alias = None

    @property
    def expr(self):
        """The aliased expression."""
        return self._attr_expr

    @expr.setter
    def expr(self, val):
        if val is None:
            del self.expr
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('expr must be of type Expression')
            val = Expression(val)
        self._attr_expr = val

    @expr.deleter
    def expr(self):
        self._attr_expr = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Mapping):
            return False
        if self.alias != other.alias:
            return False
        if self.expr != other.expr:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Mapping(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('alias: ')
        if self.alias is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.alias.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('expr: ')
        if self.expr is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.expr.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_alias is not None:
            self._attr_alias.find_reachable(id_map)
        if self._attr_expr is not None:
            self._attr_expr.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_alias is None:
            raise NotWellFormed('alias is required but not set')
        if self._attr_alias is not None:
            self._attr_alias.check_complete(id_map)
        if self._attr_expr is None:
            raise NotWellFormed('expr is required but not set')
        if self._attr_expr is not None:
            self._attr_expr.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Mapping(
            alias=self._attr_alias,
            expr=self._attr_expr,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Mapping(
            alias=_cloned(self._attr_alias),
            expr=_cloned(self._attr_expr),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Mapping':
            raise ValueError('found node serialization for ' + typ + ', but expected Mapping')

        # Deserialize the alias field.
        field = cbor.get('alias', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field alias')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field alias')
        if field.get('@t', None) is None:
            f_alias = None
        else:
            f_alias = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the expr field.
        field = cbor.get('expr', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field expr')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field expr')
        if field.get('@t', None) is None:
            f_expr = None
        else:
            f_expr = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the Mapping node.
        node = Mapping(f_alias, f_expr, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Mapping'}

        # Serialize the alias field.
        field = {'@T': '1'}
        if self._attr_alias is None:
            field['@t'] = None
        else:
            field.update(self._attr_alias._serialize(id_map))
        cbor['alias'] = field

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiMapping(_Multiple):
    """Wrapper for an edge with multiple Mapping objects."""

    _T = Mapping


_typemap['Mapping'] = Mapping

class MatrixLiteral(Expression):
    """Represents a matrix literal. An empty matrix is defined to have one row
    with zero elements. Arrays are represented as a matrix with one row. Row vs
    column vectors are different by syntax, depending on whether comma
    separation vs newline/semicolon is respectively used. While the AST allows a
    different number of elements for each row, this is illegal; it is caught at
     parse-time."""

    __slots__ = [
        '_attr_rows',
    ]

    def __init__(
        self,
        rows=None,
    ):
        super().__init__()
        self.rows = rows

    @property
    def rows(self):
        """The list of rows in the matrix."""
        return self._attr_rows

    @rows.setter
    def rows(self, val):
        if val is None:
            del self.rows
            return
        if not isinstance(val, MultiExpressionList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('rows must be of type MultiExpressionList')
            val = MultiExpressionList(val)
        self._attr_rows = val

    @rows.deleter
    def rows(self):
        self._attr_rows = MultiExpressionList()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, MatrixLiteral):
            return False
        if self.rows != other.rows:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('MatrixLiteral(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('rows: ')
        if not self.rows:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.rows:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_rows:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_rows:
            raise NotWellFormed('rows needs at least one node but has zero')
        for child in self._attr_rows:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return MatrixLiteral(
            rows=self._attr_rows.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return MatrixLiteral(
            rows=_cloned(self._attr_rows)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'MatrixLiteral':
            raise ValueError('found node serialization for ' + typ + ', but expected MatrixLiteral')

        # Deserialize the rows field.
        field = cbor.get('rows', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rows')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field rows')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_rows = MultiExpressionList()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_rows.append(ExpressionList._deserialize(element, seq_to_ob, links))

        # Construct the MatrixLiteral node.
        node = MatrixLiteral(f_rows)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'MatrixLiteral'}

        # Serialize the rows field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_rows:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['rows'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiMatrixLiteral(_Multiple):
    """Wrapper for an edge with multiple MatrixLiteral objects."""

    _T = MatrixLiteral


_typemap['MatrixLiteral'] = MatrixLiteral

class Modulo(ArithOp):
    """Modulo operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Modulo):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Modulo(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Modulo(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Modulo(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Modulo':
            raise ValueError('found node serialization for ' + typ + ', but expected Modulo')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Modulo node.
        node = Modulo(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Modulo'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiModulo(_Multiple):
    """Wrapper for an edge with multiple Modulo objects."""

    _T = Modulo


_typemap['Modulo'] = Modulo

class Multiply(ArithOp):
    """Multiplication operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Multiply):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Multiply(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Multiply(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Multiply(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Multiply':
            raise ValueError('found node serialization for ' + typ + ', but expected Multiply')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Multiply node.
        node = Multiply(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Multiply'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiMultiply(_Multiple):
    """Wrapper for an edge with multiple Multiply objects."""

    _T = Multiply


_typemap['Multiply'] = Multiply

class Negate(UnaryOp):
    """Negation operator (two's complement)."""

    __slots__ = []

    def __init__(
        self,
        expr=None,
    ):
        super().__init__(expr=expr)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Negate):
            return False
        if self.expr != other.expr:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Negate(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('expr: ')
        if self.expr is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.expr.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_expr is not None:
            self._attr_expr.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_expr is None:
            raise NotWellFormed('expr is required but not set')
        if self._attr_expr is not None:
            self._attr_expr.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Negate(
            expr=self._attr_expr
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Negate(
            expr=_cloned(self._attr_expr)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Negate':
            raise ValueError('found node serialization for ' + typ + ', but expected Negate')

        # Deserialize the expr field.
        field = cbor.get('expr', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field expr')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field expr')
        if field.get('@t', None) is None:
            f_expr = None
        else:
            f_expr = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Negate node.
        node = Negate(f_expr)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Negate'}

        # Serialize the expr field.
        field = {'@T': '1'}
        if self._attr_expr is None:
            field['@t'] = None
        else:
            field.update(self._attr_expr._serialize(id_map))
        cbor['expr'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiNegate(_Multiple):
    """Wrapper for an edge with multiple Negate objects."""

    _T = Negate


_typemap['Negate'] = Negate

class Power(ArithOp):
    """Exponentiation operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Power):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Power(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Power(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Power(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Power':
            raise ValueError('found node serialization for ' + typ + ', but expected Power')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Power node.
        node = Power(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Power'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiPower(_Multiple):
    """Wrapper for an edge with multiple Power objects."""

    _T = Power


_typemap['Power'] = Power

class Program(Root):
    """A complete program."""

    __slots__ = [
        '_attr_version',
        '_attr_num_qubits',
        '_attr_statements',
    ]

    def __init__(
        self,
        version=None,
        num_qubits=None,
        statements=None,
    ):
        super().__init__()
        self.version = version
        self.num_qubits = num_qubits
        self.statements = statements

    @property
    def version(self):
        """File version."""
        return self._attr_version

    @version.setter
    def version(self, val):
        if val is None:
            del self.version
            return
        if not isinstance(val, Version):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('version must be of type Version')
            val = Version(val)
        self._attr_version = val

    @version.deleter
    def version(self):
        self._attr_version = None

    @property
    def num_qubits(self):
        """Integer expression indicating the number of qubits."""
        return self._attr_num_qubits

    @num_qubits.setter
    def num_qubits(self, val):
        if val is None:
            del self.num_qubits
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('num_qubits must be of type Expression')
            val = Expression(val)
        self._attr_num_qubits = val

    @num_qubits.deleter
    def num_qubits(self):
        self._attr_num_qubits = None

    @property
    def statements(self):
        """The statement list."""
        return self._attr_statements

    @statements.setter
    def statements(self, val):
        if val is None:
            del self.statements
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('statements must be of type StatementList')
            val = StatementList(val)
        self._attr_statements = val

    @statements.deleter
    def statements(self):
        self._attr_statements = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Program):
            return False
        if self.version != other.version:
            return False
        if self.num_qubits != other.num_qubits:
            return False
        if self.statements != other.statements:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Program(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('version: ')
        if self.version is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.version.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('num_qubits: ')
        if self.num_qubits is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.num_qubits.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('statements: ')
        if self.statements is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.statements.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_version is not None:
            self._attr_version.find_reachable(id_map)
        if self._attr_num_qubits is not None:
            self._attr_num_qubits.find_reachable(id_map)
        if self._attr_statements is not None:
            self._attr_statements.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_version is None:
            raise NotWellFormed('version is required but not set')
        if self._attr_version is not None:
            self._attr_version.check_complete(id_map)
        if self._attr_num_qubits is not None:
            self._attr_num_qubits.check_complete(id_map)
        if self._attr_statements is None:
            raise NotWellFormed('statements is required but not set')
        if self._attr_statements is not None:
            self._attr_statements.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Program(
            version=self._attr_version,
            num_qubits=self._attr_num_qubits,
            statements=self._attr_statements
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Program(
            version=_cloned(self._attr_version),
            num_qubits=_cloned(self._attr_num_qubits),
            statements=_cloned(self._attr_statements)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Program':
            raise ValueError('found node serialization for ' + typ + ', but expected Program')

        # Deserialize the version field.
        field = cbor.get('version', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field version')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field version')
        if field.get('@t', None) is None:
            f_version = None
        else:
            f_version = Version._deserialize(field, seq_to_ob, links)

        # Deserialize the num_qubits field.
        field = cbor.get('num_qubits', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field num_qubits')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field num_qubits')
        if field.get('@t', None) is None:
            f_num_qubits = None
        else:
            f_num_qubits = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the statements field.
        field = cbor.get('statements', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field statements')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field statements')
        if field.get('@t', None) is None:
            f_statements = None
        else:
            f_statements = StatementList._deserialize(field, seq_to_ob, links)

        # Construct the Program node.
        node = Program(f_version, f_num_qubits, f_statements)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Program'}

        # Serialize the version field.
        field = {'@T': '1'}
        if self._attr_version is None:
            field['@t'] = None
        else:
            field.update(self._attr_version._serialize(id_map))
        cbor['version'] = field

        # Serialize the num_qubits field.
        field = {'@T': '?'}
        if self._attr_num_qubits is None:
            field['@t'] = None
        else:
            field.update(self._attr_num_qubits._serialize(id_map))
        cbor['num_qubits'] = field

        # Serialize the statements field.
        field = {'@T': '1'}
        if self._attr_statements is None:
            field['@t'] = None
        else:
            field.update(self._attr_statements._serialize(id_map))
        cbor['statements'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiProgram(_Multiple):
    """Wrapper for an edge with multiple Program objects."""

    _T = Program


_typemap['Program'] = Program

class RepeatUntilLoop(Structured):
    """A repeat-until loop."""

    __slots__ = [
        '_attr_body',
        '_attr_condition',
    ]

    def __init__(
        self,
        body=None,
        condition=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.body = body
        self.condition = condition

    @property
    def body(self):
        """The loop body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type StatementList')
            val = StatementList(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    @property
    def condition(self):
        """The condition for stopping iteration."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type Expression')
            val = Expression(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, RepeatUntilLoop):
            return False
        if self.body != other.body:
            return False
        if self.condition != other.condition:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('RepeatUntilLoop(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.body.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_body is not None:
            self._attr_body.find_reachable(id_map)
        if self._attr_condition is not None:
            self._attr_condition.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_body is None:
            raise NotWellFormed('body is required but not set')
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return RepeatUntilLoop(
            body=self._attr_body,
            condition=self._attr_condition,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return RepeatUntilLoop(
            body=_cloned(self._attr_body),
            condition=_cloned(self._attr_condition),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'RepeatUntilLoop':
            raise ValueError('found node serialization for ' + typ + ', but expected RepeatUntilLoop')

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = StatementList._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the RepeatUntilLoop node.
        node = RepeatUntilLoop(f_body, f_condition, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'RepeatUntilLoop'}

        # Serialize the body field.
        field = {'@T': '1'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize the condition field.
        field = {'@T': '1'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiRepeatUntilLoop(_Multiple):
    """Wrapper for an edge with multiple RepeatUntilLoop objects."""

    _T = RepeatUntilLoop


_typemap['RepeatUntilLoop'] = RepeatUntilLoop

class ShiftOp(BinaryOp):
    """Shift operators."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ == 'ShiftLeft':
            return ShiftLeft._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightArith':
            return ShiftRightArith._deserialize(cbor, seq_to_ob, links)
        if typ == 'ShiftRightLogic':
            return ShiftRightLogic._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ShiftOp'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiShiftOp(_Multiple):
    """Wrapper for an edge with multiple ShiftOp objects."""

    _T = ShiftOp


_typemap['ShiftOp'] = ShiftOp

class ShiftLeft(ShiftOp):
    """Shift-left operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ShiftLeft):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ShiftLeft(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ShiftLeft(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ShiftLeft(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ShiftLeft':
            raise ValueError('found node serialization for ' + typ + ', but expected ShiftLeft')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the ShiftLeft node.
        node = ShiftLeft(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ShiftLeft'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiShiftLeft(_Multiple):
    """Wrapper for an edge with multiple ShiftLeft objects."""

    _T = ShiftLeft


_typemap['ShiftLeft'] = ShiftLeft

class ShiftRightArith(ShiftOp):
    """Arithmetic/signed shift-right operator (shifts in sign)."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ShiftRightArith):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ShiftRightArith(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ShiftRightArith(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ShiftRightArith(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ShiftRightArith':
            raise ValueError('found node serialization for ' + typ + ', but expected ShiftRightArith')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the ShiftRightArith node.
        node = ShiftRightArith(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ShiftRightArith'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiShiftRightArith(_Multiple):
    """Wrapper for an edge with multiple ShiftRightArith objects."""

    _T = ShiftRightArith


_typemap['ShiftRightArith'] = ShiftRightArith

class ShiftRightLogic(ShiftOp):
    """Logical/unsigned shift-right operator (shifts in zero)."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ShiftRightLogic):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('ShiftRightLogic(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ShiftRightLogic(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ShiftRightLogic(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'ShiftRightLogic':
            raise ValueError('found node serialization for ' + typ + ', but expected ShiftRightLogic')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the ShiftRightLogic node.
        node = ShiftRightLogic(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'ShiftRightLogic'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiShiftRightLogic(_Multiple):
    """Wrapper for an edge with multiple ShiftRightLogic objects."""

    _T = ShiftRightLogic


_typemap['ShiftRightLogic'] = ShiftRightLogic

class StatementList(Node):
    """A list of statements."""

    __slots__ = [
        '_attr_items',
    ]

    def __init__(
        self,
        items=None,
    ):
        super().__init__()
        self.items = items

    @property
    def items(self):
        """The list of statements."""
        return self._attr_items

    @items.setter
    def items(self, val):
        if val is None:
            del self.items
            return
        if not isinstance(val, MultiStatement):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type MultiStatement')
            val = MultiStatement(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = MultiStatement()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, StatementList):
            return False
        if self.items != other.items:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('StatementList(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('items: ')
        if not self.items:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.items:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_items:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_items:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return StatementList(
            items=self._attr_items.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return StatementList(
            items=_cloned(self._attr_items)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'StatementList':
            raise ValueError('found node serialization for ' + typ + ', but expected StatementList')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field items')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_items = MultiStatement()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_items.append(Statement._deserialize(element, seq_to_ob, links))

        # Construct the StatementList node.
        node = StatementList(f_items)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'StatementList'}

        # Serialize the items field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_items:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['items'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiStatementList(_Multiple):
    """Wrapper for an edge with multiple StatementList objects."""

    _T = StatementList


_typemap['StatementList'] = StatementList

class StringLiteral(Expression):
    """Represents a string literal."""

    __slots__ = [
        '_attr_value',
    ]

    def __init__(
        self,
        value=None,
    ):
        super().__init__()
        self.value = value

    @property
    def value(self):
        """The string literal."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Str):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Str')
            val = cqasm.v1.primitives.Str(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Str()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, StringLiteral):
            return False
        if self.value != other.value:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('StringLiteral(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('value: ')
        s.append(str(self.value) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return StringLiteral(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return StringLiteral(
            value=_cloned(self._attr_value)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'StringLiteral':
            raise ValueError('found node serialization for ' + typ + ', but expected StringLiteral')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Construct the StringLiteral node.
        node = StringLiteral(f_value)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'StringLiteral'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiStringLiteral(_Multiple):
    """Wrapper for an edge with multiple StringLiteral objects."""

    _T = StringLiteral


_typemap['StringLiteral'] = StringLiteral

class Subcircuit(Statement):
    """A subcircuit header."""

    __slots__ = [
        '_attr_name',
        '_attr_iterations',
    ]

    def __init__(
        self,
        name=None,
        iterations=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.name = name
        self.iterations = iterations

    @property
    def name(self):
        """The name of the subcircuit."""
        return self._attr_name

    @name.setter
    def name(self, val):
        if val is None:
            del self.name
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('name must be of type Identifier')
            val = Identifier(val)
        self._attr_name = val

    @name.deleter
    def name(self):
        self._attr_name = None

    @property
    def iterations(self):
        """An optional integer expression representing the number of iterations
        for this subcircuit."""
        return self._attr_iterations

    @iterations.setter
    def iterations(self, val):
        if val is None:
            del self.iterations
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('iterations must be of type Expression')
            val = Expression(val)
        self._attr_iterations = val

    @iterations.deleter
    def iterations(self):
        self._attr_iterations = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Subcircuit):
            return False
        if self.name != other.name:
            return False
        if self.iterations != other.iterations:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Subcircuit(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('name: ')
        if self.name is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.name.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('iterations: ')
        if self.iterations is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.iterations.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_name is not None:
            self._attr_name.find_reachable(id_map)
        if self._attr_iterations is not None:
            self._attr_iterations.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_name is None:
            raise NotWellFormed('name is required but not set')
        if self._attr_name is not None:
            self._attr_name.check_complete(id_map)
        if self._attr_iterations is not None:
            self._attr_iterations.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Subcircuit(
            name=self._attr_name,
            iterations=self._attr_iterations,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Subcircuit(
            name=_cloned(self._attr_name),
            iterations=_cloned(self._attr_iterations),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Subcircuit':
            raise ValueError('found node serialization for ' + typ + ', but expected Subcircuit')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field name')
        if field.get('@t', None) is None:
            f_name = None
        else:
            f_name = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the iterations field.
        field = cbor.get('iterations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field iterations')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field iterations')
        if field.get('@t', None) is None:
            f_iterations = None
        else:
            f_iterations = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the Subcircuit node.
        node = Subcircuit(f_name, f_iterations, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Subcircuit'}

        # Serialize the name field.
        field = {'@T': '1'}
        if self._attr_name is None:
            field['@t'] = None
        else:
            field.update(self._attr_name._serialize(id_map))
        cbor['name'] = field

        # Serialize the iterations field.
        field = {'@T': '?'}
        if self._attr_iterations is None:
            field['@t'] = None
        else:
            field.update(self._attr_iterations._serialize(id_map))
        cbor['iterations'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiSubcircuit(_Multiple):
    """Wrapper for an edge with multiple Subcircuit objects."""

    _T = Subcircuit


_typemap['Subcircuit'] = Subcircuit

class Subtract(ArithOp):
    """Subtraction operator."""

    __slots__ = []

    def __init__(
        self,
        lhs=None,
        rhs=None,
    ):
        super().__init__(lhs=lhs, rhs=rhs)

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Subtract):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Subtract(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('lhs: ')
        if self.lhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.lhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('rhs: ')
        if self.rhs is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.rhs.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Subtract(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Subtract(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Subtract':
            raise ValueError('found node serialization for ' + typ + ', but expected Subtract')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = Expression._deserialize(field, seq_to_ob, links)

        # Construct the Subtract node.
        node = Subtract(f_lhs, f_rhs)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Subtract'}

        # Serialize the lhs field.
        field = {'@T': '1'}
        if self._attr_lhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_lhs._serialize(id_map))
        cbor['lhs'] = field

        # Serialize the rhs field.
        field = {'@T': '1'}
        if self._attr_rhs is None:
            field['@t'] = None
        else:
            field.update(self._attr_rhs._serialize(id_map))
        cbor['rhs'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiSubtract(_Multiple):
    """Wrapper for an edge with multiple Subtract objects."""

    _T = Subtract


_typemap['Subtract'] = Subtract

class TernaryCond(Expression):
    """Ternary conditional operator."""

    __slots__ = [
        '_attr_cond',
        '_attr_if_true',
        '_attr_if_false',
    ]

    def __init__(
        self,
        cond=None,
        if_true=None,
        if_false=None,
    ):
        super().__init__()
        self.cond = cond
        self.if_true = if_true
        self.if_false = if_false

    @property
    def cond(self):
        """The boolean condition for selection."""
        return self._attr_cond

    @cond.setter
    def cond(self, val):
        if val is None:
            del self.cond
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('cond must be of type Expression')
            val = Expression(val)
        self._attr_cond = val

    @cond.deleter
    def cond(self):
        self._attr_cond = None

    @property
    def if_true(self):
        """The result if cond is true."""
        return self._attr_if_true

    @if_true.setter
    def if_true(self, val):
        if val is None:
            del self.if_true
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('if_true must be of type Expression')
            val = Expression(val)
        self._attr_if_true = val

    @if_true.deleter
    def if_true(self):
        self._attr_if_true = None

    @property
    def if_false(self):
        """The result if cond is false."""
        return self._attr_if_false

    @if_false.setter
    def if_false(self, val):
        if val is None:
            del self.if_false
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('if_false must be of type Expression')
            val = Expression(val)
        self._attr_if_false = val

    @if_false.deleter
    def if_false(self):
        self._attr_if_false = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, TernaryCond):
            return False
        if self.cond != other.cond:
            return False
        if self.if_true != other.if_true:
            return False
        if self.if_false != other.if_false:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('TernaryCond(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('cond: ')
        if self.cond is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.cond.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('if_true: ')
        if self.if_true is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.if_true.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('if_false: ')
        if self.if_false is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.if_false.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_cond is not None:
            self._attr_cond.find_reachable(id_map)
        if self._attr_if_true is not None:
            self._attr_if_true.find_reachable(id_map)
        if self._attr_if_false is not None:
            self._attr_if_false.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_cond is None:
            raise NotWellFormed('cond is required but not set')
        if self._attr_cond is not None:
            self._attr_cond.check_complete(id_map)
        if self._attr_if_true is None:
            raise NotWellFormed('if_true is required but not set')
        if self._attr_if_true is not None:
            self._attr_if_true.check_complete(id_map)
        if self._attr_if_false is None:
            raise NotWellFormed('if_false is required but not set')
        if self._attr_if_false is not None:
            self._attr_if_false.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return TernaryCond(
            cond=self._attr_cond,
            if_true=self._attr_if_true,
            if_false=self._attr_if_false
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return TernaryCond(
            cond=_cloned(self._attr_cond),
            if_true=_cloned(self._attr_if_true),
            if_false=_cloned(self._attr_if_false)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'TernaryCond':
            raise ValueError('found node serialization for ' + typ + ', but expected TernaryCond')

        # Deserialize the cond field.
        field = cbor.get('cond', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field cond')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field cond')
        if field.get('@t', None) is None:
            f_cond = None
        else:
            f_cond = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the if_true field.
        field = cbor.get('if_true', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field if_true')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field if_true')
        if field.get('@t', None) is None:
            f_if_true = None
        else:
            f_if_true = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the if_false field.
        field = cbor.get('if_false', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field if_false')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field if_false')
        if field.get('@t', None) is None:
            f_if_false = None
        else:
            f_if_false = Expression._deserialize(field, seq_to_ob, links)

        # Construct the TernaryCond node.
        node = TernaryCond(f_cond, f_if_true, f_if_false)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'TernaryCond'}

        # Serialize the cond field.
        field = {'@T': '1'}
        if self._attr_cond is None:
            field['@t'] = None
        else:
            field.update(self._attr_cond._serialize(id_map))
        cbor['cond'] = field

        # Serialize the if_true field.
        field = {'@T': '1'}
        if self._attr_if_true is None:
            field['@t'] = None
        else:
            field.update(self._attr_if_true._serialize(id_map))
        cbor['if_true'] = field

        # Serialize the if_false field.
        field = {'@T': '1'}
        if self._attr_if_false is None:
            field['@t'] = None
        else:
            field.update(self._attr_if_false._serialize(id_map))
        cbor['if_false'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiTernaryCond(_Multiple):
    """Wrapper for an edge with multiple TernaryCond objects."""

    _T = TernaryCond


_typemap['TernaryCond'] = TernaryCond

class Variables(Statement):
    """One or more variable declaration for some type."""

    __slots__ = [
        '_attr_names',
        '_attr_typ',
    ]

    def __init__(
        self,
        names=None,
        typ=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.names = names
        self.typ = typ

    @property
    def names(self):
        """Name of the variables. Multiple can be declared on one line."""
        return self._attr_names

    @names.setter
    def names(self, val):
        if val is None:
            del self.names
            return
        if not isinstance(val, MultiIdentifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('names must be of type MultiIdentifier')
            val = MultiIdentifier(val)
        self._attr_names = val

    @names.deleter
    def names(self):
        self._attr_names = MultiIdentifier()

    @property
    def typ(self):
        """Name of the type."""
        return self._attr_typ

    @typ.setter
    def typ(self, val):
        if val is None:
            del self.typ
            return
        if not isinstance(val, Identifier):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('typ must be of type Identifier')
            val = Identifier(val)
        self._attr_typ = val

    @typ.deleter
    def typ(self):
        self._attr_typ = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Variables):
            return False
        if self.names != other.names:
            return False
        if self.typ != other.typ:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Variables(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('names: ')
        if not self.names:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.names:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('typ: ')
        if self.typ is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.typ.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        for el in self._attr_names:
            el.find_reachable(id_map)
        if self._attr_typ is not None:
            self._attr_typ.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_names:
            raise NotWellFormed('names needs at least one node but has zero')
        for child in self._attr_names:
            child.check_complete(id_map)
        if self._attr_typ is None:
            raise NotWellFormed('typ is required but not set')
        if self._attr_typ is not None:
            self._attr_typ.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Variables(
            names=self._attr_names.copy(),
            typ=self._attr_typ,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Variables(
            names=_cloned(self._attr_names),
            typ=_cloned(self._attr_typ),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Variables':
            raise ValueError('found node serialization for ' + typ + ', but expected Variables')

        # Deserialize the names field.
        field = cbor.get('names', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field names')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field names')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_names = MultiIdentifier()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_names.append(Identifier._deserialize(element, seq_to_ob, links))

        # Deserialize the typ field.
        field = cbor.get('typ', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field typ')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field typ')
        if field.get('@t', None) is None:
            f_typ = None
        else:
            f_typ = Identifier._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the Variables node.
        node = Variables(f_names, f_typ, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Variables'}

        # Serialize the names field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_names:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['names'] = field

        # Serialize the typ field.
        field = {'@T': '1'}
        if self._attr_typ is None:
            field['@t'] = None
        else:
            field.update(self._attr_typ._serialize(id_map))
        cbor['typ'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiVariables(_Multiple):
    """Wrapper for an edge with multiple Variables objects."""

    _T = Variables


_typemap['Variables'] = Variables

class Version(Node):
    """The file version identifier."""

    __slots__ = [
        '_attr_items',
    ]

    def __init__(
        self,
        items=None,
    ):
        super().__init__()
        self.items = items

    @property
    def items(self):
        """The list of version components, ordered major to minor."""
        return self._attr_items

    @items.setter
    def items(self, val):
        if val is None:
            del self.items
            return
        if not isinstance(val, cqasm.v1.primitives.Version):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type cqasm.v1.primitives.Version')
            val = cqasm.v1.primitives.Version(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = cqasm.v1.primitives.Version()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Version):
            return False
        if self.items != other.items:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Version(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('items: ')
        s.append(str(self.items) + '\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()

    def copy(self):
        """Returns a shallow copy of this node."""
        return Version(
            items=self._attr_items
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Version(
            items=_cloned(self._attr_items)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'Version':
            raise ValueError('found node serialization for ' + typ + ', but expected Version')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if hasattr(cqasm.v1.primitives.Version, 'deserialize_cbor'):
            f_items = cqasm.v1.primitives.Version.deserialize_cbor(field)
        else:
            f_items = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Version, field)

        # Construct the Version node.
        node = Version(f_items)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Version'}

        # Serialize the items field.
        if hasattr(self._attr_items, 'serialize_cbor'):
            cbor['items'] = self._attr_items.serialize_cbor()
        else:
            cbor['items'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Version, self._attr_items)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiVersion(_Multiple):
    """Wrapper for an edge with multiple Version objects."""

    _T = Version


_typemap['Version'] = Version

class WhileLoop(Structured):
    """A while loop."""

    __slots__ = [
        '_attr_condition',
        '_attr_body',
    ]

    def __init__(
        self,
        condition=None,
        body=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.condition = condition
        self.body = body

    @property
    def condition(self):
        """The condition for starting another iteration."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, Expression):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type Expression')
            val = Expression(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

    @property
    def body(self):
        """The loop body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, StatementList):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type StatementList')
            val = StatementList(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, WhileLoop):
            return False
        if self.condition != other.condition:
            return False
        if self.body != other.body:
            return False
        if self.annotations != other.annotations:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('WhileLoop(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.body.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('annotations: ')
        if not self.annotations:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.annotations:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        indent -= 1
        s.append('  '*indent)
        s.append(')')
        return ''.join(s)

    __str__ = dump
    __repr__ = dump

    def find_reachable(self, id_map=None):
        """Returns a dictionary mapping Python id() values to stable sequence
        numbers for all nodes in the tree rooted at this node. If id_map is
        specified, found nodes are appended to it."""
        if id_map is None:
            id_map = {}
        if id(self) in id_map:
            raise NotWellFormed('node {!r} with id {} occurs more than once'.format(self, id(self)))
        id_map[id(self)] = len(id_map)
        if self._attr_condition is not None:
            self._attr_condition.find_reachable(id_map)
        if self._attr_body is not None:
            self._attr_body.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        if self._attr_body is None:
            raise NotWellFormed('body is required but not set')
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return WhileLoop(
            condition=self._attr_condition,
            body=self._attr_body,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return WhileLoop(
            condition=_cloned(self._attr_condition),
            body=_cloned(self._attr_body),
            annotations=_cloned(self._attr_annotations)
        )

    @staticmethod
    def _deserialize(cbor, seq_to_ob, links):
        """Attempts to deserialize the given cbor object (in Python primitive
        representation) into a node of this type. All (sub)nodes are added to
        the seq_to_ob dict, indexed by their cbor sequence number. All links are
        registered in the links list by means of a two-tuple of the setter
        function for the link field and the sequence number of the target node.
        """
        if not isinstance(cbor, dict):
            raise TypeError('node description object must be a dict')
        typ = cbor.get('@t', None)
        if typ is None:
            raise ValueError('type (@t) field is missing from node serialization')
        if typ != 'WhileLoop':
            raise ValueError('found node serialization for ' + typ + ', but expected WhileLoop')

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = Expression._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = StatementList._deserialize(field, seq_to_ob, links)

        # Deserialize the annotations field.
        field = cbor.get('annotations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field annotations')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field annotations')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_annotations = MultiAnnotationData()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_annotations.append(AnnotationData._deserialize(element, seq_to_ob, links))

        # Construct the WhileLoop node.
        node = WhileLoop(f_condition, f_body, f_annotations)

        # Deserialize annotations.
        for key, val in cbor.items():
            if not (key.startswith('{') and key.endswith('}')):
                continue
            key = key[1:-1]
            node[key] = cqasm.v1.primitives.deserialize(key, val)

        # Register node in sequence number lookup.
        seq = cbor.get('@i', None)
        if not isinstance(seq, int):
            raise ValueError('sequence number field (@i) is not an integer or missing from node serialization')
        if seq in seq_to_ob:
            raise ValueError('duplicate sequence number %d' % seq)
        seq_to_ob[seq] = node

        return node

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'WhileLoop'}

        # Serialize the condition field.
        field = {'@T': '1'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the body field.
        field = {'@T': '1'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiWhileLoop(_Multiple):
    """Wrapper for an edge with multiple WhileLoop objects."""

    _T = WhileLoop


_typemap['WhileLoop'] = WhileLoop

