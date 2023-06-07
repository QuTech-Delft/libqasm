import functools
import struct
import cqasm.v1.primitives
import cqasm.v1.types
import cqasm.v1.semantic


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


class Reference(Node):
    """Represents a reference to some storage location."""

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
        if typ == 'QubitRefs':
            return QubitRefs._deserialize(cbor, seq_to_ob, links)
        if typ == 'BitRefs':
            return BitRefs._deserialize(cbor, seq_to_ob, links)
        if typ == 'VariableRef':
            return VariableRef._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Reference'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiReference(_Multiple):
    """Wrapper for an edge with multiple Reference objects."""

    _T = Reference


_typemap['Reference'] = Reference

class BitRefs(Reference):
    """Represents a measurement bit, or a number of measurement bits for
    conditional gates with multiple conditions. When there are multiple, the
    conditions are all AND'ed together."""

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
        """The qubit index that this is a measurement bit for, starting at 0.
        """
        return self._attr_index

    @index.setter
    def index(self, val):
        if val is None:
            del self.index
            return
        if not isinstance(val, MultiConstInt):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('index must be of type MultiConstInt')
            val = MultiConstInt(val)
        self._attr_index = val

    @index.deleter
    def index(self):
        self._attr_index = MultiConstInt()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BitRefs):
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
        s.append('BitRefs(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('index: ')
        if not self.index:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.index:
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
        for el in self._attr_index:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_index:
            raise NotWellFormed('index needs at least one node but has zero')
        for child in self._attr_index:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return BitRefs(
            index=self._attr_index.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BitRefs(
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
        if typ != 'BitRefs':
            raise ValueError('found node serialization for ' + typ + ', but expected BitRefs')

        # Deserialize the index field.
        field = cbor.get('index', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field index')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field index')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_index = MultiConstInt()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_index.append(ConstInt._deserialize(element, seq_to_ob, links))

        # Construct the BitRefs node.
        node = BitRefs(f_index)

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
        cbor = {'@i': id_map[id(self)], '@t': 'BitRefs'}

        # Serialize the index field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_index:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['index'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBitRefs(_Multiple):
    """Wrapper for an edge with multiple BitRefs objects."""

    _T = BitRefs


_typemap['BitRefs'] = BitRefs

class Constant(Node):
    """Represents a constant value."""

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
        if typ == 'ConstBool':
            return ConstBool._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstAxis':
            return ConstAxis._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstInt':
            return ConstInt._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstReal':
            return ConstReal._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstComplex':
            return ConstComplex._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstRealMatrix':
            return ConstRealMatrix._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstComplexMatrix':
            return ConstComplexMatrix._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstString':
            return ConstString._deserialize(cbor, seq_to_ob, links)
        if typ == 'ConstJson':
            return ConstJson._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'Constant'}

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstant(_Multiple):
    """Wrapper for an edge with multiple Constant objects."""

    _T = Constant


_typemap['Constant'] = Constant

class ConstAxis(Constant):
    """Represents an axis value (x, y, or z)."""

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
        """The contained value."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Axis):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Axis')
            val = cqasm.v1.primitives.Axis(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Axis()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ConstAxis):
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
        s.append('ConstAxis(')
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
        return ConstAxis(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstAxis(
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
        if typ != 'ConstAxis':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstAxis')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Axis, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Axis.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Axis, field)

        # Construct the ConstAxis node.
        node = ConstAxis(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstAxis'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Axis, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstAxis(_Multiple):
    """Wrapper for an edge with multiple ConstAxis objects."""

    _T = ConstAxis


_typemap['ConstAxis'] = ConstAxis

class ConstBool(Constant):
    """Represents a value of type bool."""

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
        """The contained value."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Bool):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Bool')
            val = cqasm.v1.primitives.Bool(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Bool()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ConstBool):
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
        s.append('ConstBool(')
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
        return ConstBool(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstBool(
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
        if typ != 'ConstBool':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstBool')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Bool, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Bool.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Bool, field)

        # Construct the ConstBool node.
        node = ConstBool(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstBool'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Bool, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstBool(_Multiple):
    """Wrapper for an edge with multiple ConstBool objects."""

    _T = ConstBool


_typemap['ConstBool'] = ConstBool

class ConstComplex(Constant):
    """Represents a value of type complex."""

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
        """The contained value."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.Complex):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.Complex')
            val = cqasm.v1.primitives.Complex(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.Complex()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ConstComplex):
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
        s.append('ConstComplex(')
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
        return ConstComplex(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstComplex(
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
        if typ != 'ConstComplex':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstComplex')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Complex, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Complex.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Complex, field)

        # Construct the ConstComplex node.
        node = ConstComplex(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstComplex'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Complex, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstComplex(_Multiple):
    """Wrapper for an edge with multiple ConstComplex objects."""

    _T = ConstComplex


_typemap['ConstComplex'] = ConstComplex

class ConstComplexMatrix(Constant):
    """Represents a value of type complex_matrix."""

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
        """The contained value."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.CMatrix):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.CMatrix')
            val = cqasm.v1.primitives.CMatrix(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.CMatrix()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ConstComplexMatrix):
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
        s.append('ConstComplexMatrix(')
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
        return ConstComplexMatrix(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstComplexMatrix(
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
        if typ != 'ConstComplexMatrix':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstComplexMatrix')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.CMatrix, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.CMatrix.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.CMatrix, field)

        # Construct the ConstComplexMatrix node.
        node = ConstComplexMatrix(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstComplexMatrix'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.CMatrix, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstComplexMatrix(_Multiple):
    """Wrapper for an edge with multiple ConstComplexMatrix objects."""

    _T = ConstComplexMatrix


_typemap['ConstComplexMatrix'] = ConstComplexMatrix

class ConstInt(Constant):
    """Represents a value of type int."""

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
        """The contained value."""
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
        if not isinstance(other, ConstInt):
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
        s.append('ConstInt(')
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
        return ConstInt(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstInt(
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
        if typ != 'ConstInt':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstInt')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Construct the ConstInt node.
        node = ConstInt(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstInt'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstInt(_Multiple):
    """Wrapper for an edge with multiple ConstInt objects."""

    _T = ConstInt


_typemap['ConstInt'] = ConstInt

class ConstJson(Constant):
    """Represents a value of type json."""

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
        """The contained value."""
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
        if not isinstance(other, ConstJson):
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
        s.append('ConstJson(')
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
        return ConstJson(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstJson(
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
        if typ != 'ConstJson':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstJson')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Construct the ConstJson node.
        node = ConstJson(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstJson'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstJson(_Multiple):
    """Wrapper for an edge with multiple ConstJson objects."""

    _T = ConstJson


_typemap['ConstJson'] = ConstJson

class ConstReal(Constant):
    """Represents a value of type real."""

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
        """The contained value."""
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
        if not isinstance(other, ConstReal):
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
        s.append('ConstReal(')
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
        return ConstReal(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstReal(
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
        if typ != 'ConstReal':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstReal')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Real, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Real.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Real, field)

        # Construct the ConstReal node.
        node = ConstReal(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstReal'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Real, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstReal(_Multiple):
    """Wrapper for an edge with multiple ConstReal objects."""

    _T = ConstReal


_typemap['ConstReal'] = ConstReal

class ConstRealMatrix(Constant):
    """Represents a value of type real_matrix."""

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
        """The contained value."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.primitives.RMatrix):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.primitives.RMatrix')
            val = cqasm.v1.primitives.RMatrix(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = cqasm.v1.primitives.RMatrix()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ConstRealMatrix):
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
        s.append('ConstRealMatrix(')
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
        return ConstRealMatrix(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstRealMatrix(
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
        if typ != 'ConstRealMatrix':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstRealMatrix')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.RMatrix, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.RMatrix.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.RMatrix, field)

        # Construct the ConstRealMatrix node.
        node = ConstRealMatrix(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstRealMatrix'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.RMatrix, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstRealMatrix(_Multiple):
    """Wrapper for an edge with multiple ConstRealMatrix objects."""

    _T = ConstRealMatrix


_typemap['ConstRealMatrix'] = ConstRealMatrix

class ConstString(Constant):
    """Represents a value of type string."""

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
        """The contained value."""
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
        if not isinstance(other, ConstString):
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
        s.append('ConstString(')
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
        return ConstString(
            value=self._attr_value
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ConstString(
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
        if typ != 'ConstString':
            raise ValueError('found node serialization for ' + typ + ', but expected ConstString')

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_value = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_value = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Construct the ConstString node.
        node = ConstString(f_value)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ConstString'}

        # Serialize the value field.
        if hasattr(self._attr_value, 'serialize_cbor'):
            cbor['value'] = self._attr_value.serialize_cbor()
        else:
            cbor['value'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_value)

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiConstString(_Multiple):
    """Wrapper for an edge with multiple ConstString objects."""

    _T = ConstString


_typemap['ConstString'] = ConstString

class Function(Node):
    """This can be returned by user-defined functions as a placeholder value for
    something that needs to be evaluated at runtime rather than during constant
    propagation. Annotations should be used to attach semantic information."""

    __slots__ = [
        '_attr_name',
        '_attr_operands',
        '_attr_return_type',
    ]

    def __init__(
        self,
        name=None,
        operands=None,
        return_type=None,
    ):
        super().__init__()
        self.name = name
        self.operands = operands
        self.return_type = return_type

    @property
    def name(self):
        """Name of the function as it appears or should appear in the cQASM
         file."""
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

    @property
    def operands(self):
        """Operands for the function."""
        return self._attr_operands

    @operands.setter
    def operands(self, val):
        if val is None:
            del self.operands
            return
        if not isinstance(val, cqasm.v1.values.MultiNode):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('operands must be of type cqasm.v1.values.MultiNode')
            val = cqasm.v1.values.MultiNode(val)
        self._attr_operands = val

    @operands.deleter
    def operands(self):
        self._attr_operands = cqasm.v1.values.MultiNode()

    @property
    def return_type(self):
        """Operands for the function."""
        return self._attr_return_type

    @return_type.setter
    def return_type(self, val):
        if val is None:
            del self.return_type
            return
        if not isinstance(val, cqasm.v1.types.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('return_type must be of type cqasm.v1.types.Node')
            val = cqasm.v1.types.Node(val)
        self._attr_return_type = val

    @return_type.deleter
    def return_type(self):
        self._attr_return_type = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Function):
            return False
        if self.name != other.name:
            return False
        if self.operands != other.operands:
            return False
        if self.return_type != other.return_type:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('Function(')
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
        s.append('  '*indent)
        s.append('operands: ')
        if not self.operands:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.operands:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('return_type: ')
        if self.return_type is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.return_type.dump(indent + 1, annotations, links) + '\n')
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
        for el in self._attr_operands:
            el.find_reachable(id_map)
        if self._attr_return_type is not None:
            self._attr_return_type.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_operands:
            child.check_complete(id_map)
        if self._attr_return_type is None:
            raise NotWellFormed('return_type is required but not set')
        if self._attr_return_type is not None:
            self._attr_return_type.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Function(
            name=self._attr_name,
            operands=self._attr_operands.copy(),
            return_type=self._attr_return_type
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Function(
            name=_cloned(self._attr_name),
            operands=_cloned(self._attr_operands),
            return_type=_cloned(self._attr_return_type)
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
        if typ != 'Function':
            raise ValueError('found node serialization for ' + typ + ', but expected Function')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the operands field.
        field = cbor.get('operands', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field operands')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field operands')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_operands = cqasm.v1.values.MultiNode()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_operands.append(cqasm.v1.values.Node._deserialize(element, seq_to_ob, links))

        # Deserialize the return_type field.
        field = cbor.get('return_type', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field return_type')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field return_type')
        if field.get('@t', None) is None:
            f_return_type = None
        else:
            f_return_type = cqasm.v1.types.Node._deserialize(field, seq_to_ob, links)

        # Construct the Function node.
        node = Function(f_name, f_operands, f_return_type)

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
        cbor = {'@i': id_map[id(self)], '@t': 'Function'}

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize the operands field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_operands:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['operands'] = field

        # Serialize the return_type field.
        field = {'@T': '1'}
        if self._attr_return_type is None:
            field['@t'] = None
        else:
            field.update(self._attr_return_type._serialize(id_map))
        cbor['return_type'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiFunction(_Multiple):
    """Wrapper for an edge with multiple Function objects."""

    _T = Function


_typemap['Function'] = Function

class QubitRefs(Reference):
    """Represents a qubit, or a set of qubits for single-gate-multiple-qubit
    notation. The indices must not repeat."""

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
        """Set of qubit indices referred to, starting at 0."""
        return self._attr_index

    @index.setter
    def index(self, val):
        if val is None:
            del self.index
            return
        if not isinstance(val, MultiConstInt):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('index must be of type MultiConstInt')
            val = MultiConstInt(val)
        self._attr_index = val

    @index.deleter
    def index(self):
        self._attr_index = MultiConstInt()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, QubitRefs):
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
        s.append('QubitRefs(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('index: ')
        if not self.index:
            s.append('!MISSING\n')
        else:
            s.append('[\n')
            for child in self.index:
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
        for el in self._attr_index:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if not self._attr_index:
            raise NotWellFormed('index needs at least one node but has zero')
        for child in self._attr_index:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return QubitRefs(
            index=self._attr_index.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return QubitRefs(
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
        if typ != 'QubitRefs':
            raise ValueError('found node serialization for ' + typ + ', but expected QubitRefs')

        # Deserialize the index field.
        field = cbor.get('index', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field index')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field index')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_index = MultiConstInt()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_index.append(ConstInt._deserialize(element, seq_to_ob, links))

        # Construct the QubitRefs node.
        node = QubitRefs(f_index)

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
        cbor = {'@i': id_map[id(self)], '@t': 'QubitRefs'}

        # Serialize the index field.
        field = {'@T': '+'}
        lst = []
        for el in self._attr_index:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['index'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiQubitRefs(_Multiple):
    """Wrapper for an edge with multiple QubitRefs objects."""

    _T = QubitRefs


_typemap['QubitRefs'] = QubitRefs

class VariableRef(Reference):
    """Represents a variable reference."""

    __slots__ = [
        '_attr_variable',
    ]

    def __init__(
        self,
        variable=None,
    ):
        super().__init__()
        self.variable = variable

    @property
    def variable(self):
        """The referenced variable."""
        return self._attr_variable

    @variable.setter
    def variable(self, val):
        if val is None:
            del self.variable
            return
        if not isinstance(val, cqasm.v1.semantic.Variable):
            raise TypeError('variable must be of type cqasm.v1.semantic.Variable')
        self._attr_variable = val

    @variable.deleter
    def variable(self):
        self._attr_variable = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, VariableRef):
            return False
        if self.variable is not other.variable:
            return False
        return True

    def dump(self, indent=0, annotations=None, links=1):
        """Returns a debug representation of this tree as a multiline string.
        indent is the number of double spaces prefixed before every line.
        annotations, if specified, must be a set-like object containing the key
        strings of the annotations that are to be printed. links specifies the
        maximum link recursion depth."""
        s = ['  '*indent]
        s.append('VariableRef(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('variable --> ')
        if self.variable is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            if links:
                s.append(self.variable.dump(indent + 1, annotations, links - 1) + '\n')
            else:
                s.append('  '*(indent+1) + '...\n')
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
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        if self._attr_variable is None:
            raise NotWellFormed('variable is required but not set')
        if self._attr_variable is not None:
            if id(self._attr_variable) not in id_map:
                raise NotWellFormed('variable links to unreachable node')

    def copy(self):
        """Returns a shallow copy of this node."""
        return VariableRef(
            variable=self._attr_variable
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return VariableRef(
            variable=self._attr_variable
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
        if typ != 'VariableRef':
            raise ValueError('found node serialization for ' + typ + ', but expected VariableRef')

        # Deserialize the variable field.
        field = cbor.get('variable', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field variable')
        if field.get('@T') != '$':
            raise ValueError('unexpected edge type for field variable')
        f_variable = None
        l_variable = field.get('@l', None)

        # Construct the VariableRef node.
        node = VariableRef(f_variable)

        # Register links to be made after tree construction.
        links.append((lambda val: VariableRef.variable.fset(node, val), l_variable))

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
        cbor = {'@i': id_map[id(self)], '@t': 'VariableRef'}

        # Serialize the variable field.
        field = {'@T': '$'}
        if self._attr_variable is None:
            field['@l'] = None
        else:
            field['@l'] = id_map[id(self._attr_variable)]
        cbor['variable'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiVariableRef(_Multiple):
    """Wrapper for an edge with multiple VariableRef objects."""

    _T = VariableRef


_typemap['VariableRef'] = VariableRef

