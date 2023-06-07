import functools
import struct
import cqasm.v1.primitives
import cqasm.v1.values
import cqasm.v1.types
import cqasm.v1.error_model
import cqasm.v1.instruction


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
        if typ == 'ErrorModel':
            return ErrorModel._deserialize(cbor, seq_to_ob, links)
        if typ == 'Instruction':
            return Instruction._deserialize(cbor, seq_to_ob, links)
        if typ == 'SetInstruction':
            return SetInstruction._deserialize(cbor, seq_to_ob, links)
        if typ == 'GotoInstruction':
            return GotoInstruction._deserialize(cbor, seq_to_ob, links)
        if typ == 'Bundle':
            return Bundle._deserialize(cbor, seq_to_ob, links)
        if typ == 'BundleExt':
            return BundleExt._deserialize(cbor, seq_to_ob, links)
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
        if typ == 'Subcircuit':
            return Subcircuit._deserialize(cbor, seq_to_ob, links)
        if typ == 'Mapping':
            return Mapping._deserialize(cbor, seq_to_ob, links)
        if typ == 'Variable':
            return Variable._deserialize(cbor, seq_to_ob, links)
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
        if not isinstance(val, cqasm.v1.primitives.Str):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('interface must be of type cqasm.v1.primitives.Str')
            val = cqasm.v1.primitives.Str(val)
        self._attr_interface = val

    @interface.deleter
    def interface(self):
        self._attr_interface = cqasm.v1.primitives.Str()

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
        if not isinstance(val, cqasm.v1.primitives.Str):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('operation must be of type cqasm.v1.primitives.Str')
            val = cqasm.v1.primitives.Str(val)
        self._attr_operation = val

    @operation.deleter
    def operation(self):
        self._attr_operation = cqasm.v1.primitives.Str()

    @property
    def operands(self):
        """Any operands attached to the annotation."""
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
        s.append(str(self.interface) + '\n')
        s.append('  '*indent)
        s.append('operation: ')
        s.append(str(self.operation) + '\n')
        s.append('  '*indent)
        s.append('operands: ')
        if not self.operands:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.operands:
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
        for el in self._attr_operands:
            el.find_reachable(id_map)
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

    def copy(self):
        """Returns a shallow copy of this node."""
        return AnnotationData(
            interface=self._attr_interface,
            operation=self._attr_operation,
            operands=self._attr_operands.copy()
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
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_interface = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_interface = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the operation field.
        field = cbor.get('operation', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field operation')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_operation = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_operation = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

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
        if hasattr(self._attr_interface, 'serialize_cbor'):
            cbor['interface'] = self._attr_interface.serialize_cbor()
        else:
            cbor['interface'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_interface)

        # Serialize the operation field.
        if hasattr(self._attr_operation, 'serialize_cbor'):
            cbor['operation'] = self._attr_operation.serialize_cbor()
        else:
            cbor['operation'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_operation)

        # Serialize the operands field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_operands:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['operands'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiAnnotationData(_Multiple):
    """Wrapper for an edge with multiple AnnotationData objects."""

    _T = AnnotationData


_typemap['AnnotationData'] = AnnotationData

class Block(Node):
    """A block of statements. Only used for API level 1.2+."""

    __slots__ = [
        '_attr_statements',
    ]

    def __init__(
        self,
        statements=None,
    ):
        super().__init__()
        self.statements = statements

    @property
    def statements(self):
        """The statements contained by the block."""
        return self._attr_statements

    @statements.setter
    def statements(self, val):
        if val is None:
            del self.statements
            return
        if not isinstance(val, MultiStatement):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('statements must be of type MultiStatement')
            val = MultiStatement(val)
        self._attr_statements = val

    @statements.deleter
    def statements(self):
        self._attr_statements = MultiStatement()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Block):
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
        s.append('Block(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('statements: ')
        if not self.statements:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.statements:
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
        for el in self._attr_statements:
            el.find_reachable(id_map)
        return id_map

    def check_complete(self, id_map=None):
        """Raises NotWellFormed if the tree rooted at this node is not
        well-formed. If id_map is specified, this tree is only a subtree in the
        context of a larger tree, and id_map must be a dict mapping from Python
        id() codes to tree indices for all reachable nodes."""
        if id_map is None:
            id_map = self.find_reachable()
        for child in self._attr_statements:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Block(
            statements=self._attr_statements.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Block(
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
        if typ != 'Block':
            raise ValueError('found node serialization for ' + typ + ', but expected Block')

        # Deserialize the statements field.
        field = cbor.get('statements', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field statements')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field statements')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_statements = MultiStatement()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_statements.append(Statement._deserialize(element, seq_to_ob, links))

        # Construct the Block node.
        node = Block(f_statements)

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
        cbor = {'@i': id_map[id(self)], '@t': 'Block'}

        # Serialize the statements field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_statements:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['statements'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiBlock(_Multiple):
    """Wrapper for an edge with multiple Block objects."""

    _T = Block


_typemap['Block'] = Block

class Statement(Annotated):
    """A statement. Unused before 1.2, as only bundles existed before then."""

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
        if typ == 'BundleExt':
            return BundleExt._deserialize(cbor, seq_to_ob, links)
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

class Bundle(Annotated):
    """A bundle of instructions, to be executed in parallel. Only used for API
    level 1.0 and 1.1; 1.2+ uses bundle_ext."""

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

class BundleExt(Statement):
    """A bundle of instructions, to be executed in parallel. Only used for API
    level 1.2+."""

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
        if not isinstance(val, MultiInstructionBase):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('items must be of type MultiInstructionBase')
            val = MultiInstructionBase(val)
        self._attr_items = val

    @items.deleter
    def items(self):
        self._attr_items = MultiInstructionBase()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, BundleExt):
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
        s.append('BundleExt(')
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
        return BundleExt(
            items=self._attr_items.copy(),
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return BundleExt(
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
        if typ != 'BundleExt':
            raise ValueError('found node serialization for ' + typ + ', but expected BundleExt')

        # Deserialize the items field.
        field = cbor.get('items', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field items')
        if field.get('@T') != '+':
            raise ValueError('unexpected edge type for field items')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_items = MultiInstructionBase()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_items.append(InstructionBase._deserialize(element, seq_to_ob, links))

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

        # Construct the BundleExt node.
        node = BundleExt(f_items, f_annotations)

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
        cbor = {'@i': id_map[id(self)], '@t': 'BundleExt'}

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


class MultiBundleExt(_Multiple):
    """Wrapper for an edge with multiple BundleExt objects."""

    _T = BundleExt


_typemap['BundleExt'] = BundleExt

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

class ErrorModel(Annotated):
    """Error model information."""

    __slots__ = [
        '_attr_model',
        '_attr_name',
        '_attr_parameters',
    ]

    def __init__(
        self,
        model=None,
        name=None,
        parameters=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.model = model
        self.name = name
        self.parameters = parameters

    @property
    def model(self):
        """Error model type as registered through the API."""
        return self._attr_model

    @model.setter
    def model(self, val):
        if val is None:
            del self.model
            return
        if not isinstance(val, cqasm.v1.error_model.ErrorModelRef):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('model must be of type cqasm.v1.error_model.ErrorModelRef')
            val = cqasm.v1.error_model.ErrorModelRef(val)
        self._attr_model = val

    @model.deleter
    def model(self):
        self._attr_model = cqasm.v1.error_model.ErrorModelRef()

    @property
    def name(self):
        """Name as it appears in the cQASM file."""
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
    def parameters(self):
        """Error model parameters."""
        return self._attr_parameters

    @parameters.setter
    def parameters(self, val):
        if val is None:
            del self.parameters
            return
        if not isinstance(val, cqasm.v1.values.MultiNode):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('parameters must be of type cqasm.v1.values.MultiNode')
            val = cqasm.v1.values.MultiNode(val)
        self._attr_parameters = val

    @parameters.deleter
    def parameters(self):
        self._attr_parameters = cqasm.v1.values.MultiNode()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, ErrorModel):
            return False
        if self.model != other.model:
            return False
        if self.name != other.name:
            return False
        if self.parameters != other.parameters:
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
        s.append('ErrorModel(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('model: ')
        s.append(str(self.model) + '\n')
        s.append('  '*indent)
        s.append('name: ')
        s.append(str(self.name) + '\n')
        s.append('  '*indent)
        s.append('parameters: ')
        if not self.parameters:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.parameters:
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
        for el in self._attr_parameters:
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
        for child in self._attr_parameters:
            child.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return ErrorModel(
            model=self._attr_model,
            name=self._attr_name,
            parameters=self._attr_parameters.copy(),
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return ErrorModel(
            model=_cloned(self._attr_model),
            name=_cloned(self._attr_name),
            parameters=_cloned(self._attr_parameters),
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
        if typ != 'ErrorModel':
            raise ValueError('found node serialization for ' + typ + ', but expected ErrorModel')

        # Deserialize the model field.
        field = cbor.get('model', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field model')
        if hasattr(cqasm.v1.error_model.ErrorModelRef, 'deserialize_cbor'):
            f_model = cqasm.v1.error_model.ErrorModelRef.deserialize_cbor(field)
        else:
            f_model = cqasm.v1.primitives.deserialize(cqasm.v1.error_model.ErrorModelRef, field)

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the parameters field.
        field = cbor.get('parameters', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field parameters')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field parameters')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_parameters = cqasm.v1.values.MultiNode()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_parameters.append(cqasm.v1.values.Node._deserialize(element, seq_to_ob, links))

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

        # Construct the ErrorModel node.
        node = ErrorModel(f_model, f_name, f_parameters, f_annotations)

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
        cbor = {'@i': id_map[id(self)], '@t': 'ErrorModel'}

        # Serialize the model field.
        if hasattr(self._attr_model, 'serialize_cbor'):
            cbor['model'] = self._attr_model.serialize_cbor()
        else:
            cbor['model'] = cqasm.v1.primitives.serialize(cqasm.v1.error_model.ErrorModelRef, self._attr_model)

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize the parameters field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_parameters:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['parameters'] = field

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


class MultiErrorModel(_Multiple):
    """Wrapper for an edge with multiple ErrorModel objects."""

    _T = ErrorModel


_typemap['ErrorModel'] = ErrorModel

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
        if not isinstance(val, SetInstruction):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('initialize must be of type SetInstruction')
            val = SetInstruction(val)
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
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
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
        if not isinstance(val, SetInstruction):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('update must be of type SetInstruction')
            val = SetInstruction(val)
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
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
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
            f_initialize = SetInstruction._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the update field.
        field = cbor.get('update', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field update')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field update')
        if field.get('@t', None) is None:
            f_update = None
        else:
            f_update = SetInstruction._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = Block._deserialize(field, seq_to_ob, links)

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
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('lhs must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
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
        if not isinstance(val, cqasm.v1.primitives.Int):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('frm must be of type cqasm.v1.primitives.Int')
            val = cqasm.v1.primitives.Int(val)
        self._attr_frm = val

    @frm.deleter
    def frm(self):
        self._attr_frm = cqasm.v1.primitives.Int()

    @property
    def to(self):
        """The last value."""
        return self._attr_to

    @to.setter
    def to(self, val):
        if val is None:
            del self.to
            return
        if not isinstance(val, cqasm.v1.primitives.Int):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('to must be of type cqasm.v1.primitives.Int')
            val = cqasm.v1.primitives.Int(val)
        self._attr_to = val

    @to.deleter
    def to(self):
        self._attr_to = cqasm.v1.primitives.Int()

    @property
    def body(self):
        """The loop body."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
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
        s.append(str(self.frm) + '\n')
        s.append('  '*indent)
        s.append('to: ')
        s.append(str(self.to) + '\n')
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
            f_lhs = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the frm field.
        field = cbor.get('frm', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field frm')
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_frm = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_frm = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Deserialize the to field.
        field = cbor.get('to', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field to')
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_to = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_to = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = Block._deserialize(field, seq_to_ob, links)

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
        if hasattr(self._attr_frm, 'serialize_cbor'):
            cbor['frm'] = self._attr_frm.serialize_cbor()
        else:
            cbor['frm'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_frm)

        # Serialize the to field.
        if hasattr(self._attr_to, 'serialize_cbor'):
            cbor['to'] = self._attr_to.serialize_cbor()
        else:
            cbor['to'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_to)

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

class InstructionBase(Annotated):
    """An instruction (a.k.a. gate)."""

    __slots__ = [
        '_attr_condition',
    ]

    def __init__(
        self,
        condition=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.condition = condition

    @property
    def condition(self):
        """Condition (c- notation). When there is no condition, this is a
        constant boolean set to true."""
        return self._attr_condition

    @condition.setter
    def condition(self, val):
        if val is None:
            del self.condition
            return
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
        self._attr_condition = val

    @condition.deleter
    def condition(self):
        self._attr_condition = None

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
        if typ == 'SetInstruction':
            return SetInstruction._deserialize(cbor, seq_to_ob, links)
        if typ == 'GotoInstruction':
            return GotoInstruction._deserialize(cbor, seq_to_ob, links)
        raise ValueError('unknown or unexpected type (@t) found in node serialization')

    def _serialize(self, id_map):
        """Serializes this node to the Python primitive representation of its
        CBOR serialization. The tree that the node belongs to must be
        well-formed. id_map must match Python id() calls for all nodes to unique
        integers, to use for the sequence number representation of links."""
        cbor = {'@i': id_map[id(self)], '@t': 'InstructionBase'}

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


class MultiInstructionBase(_Multiple):
    """Wrapper for an edge with multiple InstructionBase objects."""

    _T = InstructionBase


_typemap['InstructionBase'] = InstructionBase

class GotoInstruction(InstructionBase):
    """A version 1.2+ goto instruction."""

    __slots__ = [
        '_attr_target',
    ]

    def __init__(
        self,
        target=None,
        condition=None,
        annotations=None,
    ):
        super().__init__(condition=condition, annotations=annotations)
        self.target = target

    @property
    def target(self):
        """Link to the target subcircuit, used as a label."""
        return self._attr_target

    @target.setter
    def target(self, val):
        if val is None:
            del self.target
            return
        if not isinstance(val, Subcircuit):
            raise TypeError('target must be of type Subcircuit')
        self._attr_target = val

    @target.deleter
    def target(self):
        self._attr_target = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, GotoInstruction):
            return False
        if self.target is not other.target:
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
        s.append('GotoInstruction(')
        if annotations is None:
            annotations = []
        for key in annotations:
            if key in self:
                s.append(' # {}: {}'.format(key, self[key]))
        s.append('\n')
        indent += 1
        s.append('  '*indent)
        s.append('target --> ')
        if self.target is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            if links:
                s.append(self.target.dump(indent + 1, annotations, links - 1) + '\n')
            else:
                s.append('  '*(indent+1) + '...\n')
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
        if self._attr_target is None:
            raise NotWellFormed('target is required but not set')
        if self._attr_target is not None:
            if id(self._attr_target) not in id_map:
                raise NotWellFormed('target links to unreachable node')
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return GotoInstruction(
            target=self._attr_target,
            condition=self._attr_condition,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return GotoInstruction(
            target=self._attr_target,
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
        if typ != 'GotoInstruction':
            raise ValueError('found node serialization for ' + typ + ', but expected GotoInstruction')

        # Deserialize the target field.
        field = cbor.get('target', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field target')
        if field.get('@T') != '$':
            raise ValueError('unexpected edge type for field target')
        f_target = None
        l_target = field.get('@l', None)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

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

        # Construct the GotoInstruction node.
        node = GotoInstruction(f_target, f_condition, f_annotations)

        # Register links to be made after tree construction.
        links.append((lambda val: GotoInstruction.target.fset(node, val), l_target))

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
        cbor = {'@i': id_map[id(self)], '@t': 'GotoInstruction'}

        # Serialize the target field.
        field = {'@T': '$'}
        if self._attr_target is None:
            field['@l'] = None
        else:
            field['@l'] = id_map[id(self._attr_target)]
        cbor['target'] = field

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


class MultiGotoInstruction(_Multiple):
    """Wrapper for an edge with multiple GotoInstruction objects."""

    _T = GotoInstruction


_typemap['GotoInstruction'] = GotoInstruction

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
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('otherwise must be of type Block')
            val = Block(val)
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
            f_otherwise = Block._deserialize(field, seq_to_ob, links)

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
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
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
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
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
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = Block._deserialize(field, seq_to_ob, links)

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

class Instruction(InstructionBase):
    """A regular instruction. Until 1.2, this is the only type of instruction
    that exists."""

    __slots__ = [
        '_attr_instruction',
        '_attr_name',
        '_attr_operands',
    ]

    def __init__(
        self,
        instruction=None,
        name=None,
        condition=None,
        operands=None,
        annotations=None,
    ):
        super().__init__(condition=condition, annotations=annotations)
        self.instruction = instruction
        self.name = name
        self.operands = operands

    @property
    def instruction(self):
        """Instruction type as registered through the API."""
        return self._attr_instruction

    @instruction.setter
    def instruction(self, val):
        if val is None:
            del self.instruction
            return
        if not isinstance(val, cqasm.v1.instruction.InstructionRef):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('instruction must be of type cqasm.v1.instruction.InstructionRef')
            val = cqasm.v1.instruction.InstructionRef(val)
        self._attr_instruction = val

    @instruction.deleter
    def instruction(self):
        self._attr_instruction = cqasm.v1.instruction.InstructionRef()

    @property
    def name(self):
        """Name as it appears in the cQASM file."""
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
        """Operands for the instruction."""
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

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Instruction):
            return False
        if self.instruction != other.instruction:
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
        s.append('instruction: ')
        s.append(str(self.instruction) + '\n')
        s.append('  '*indent)
        s.append('name: ')
        s.append(str(self.name) + '\n')
        s.append('  '*indent)
        s.append('condition: ')
        if self.condition is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.condition.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
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
        for el in self._attr_operands:
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
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        for child in self._attr_operands:
            child.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Instruction(
            instruction=self._attr_instruction,
            name=self._attr_name,
            condition=self._attr_condition,
            operands=self._attr_operands.copy(),
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Instruction(
            instruction=_cloned(self._attr_instruction),
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

        # Deserialize the instruction field.
        field = cbor.get('instruction', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field instruction')
        if hasattr(cqasm.v1.instruction.InstructionRef, 'deserialize_cbor'):
            f_instruction = cqasm.v1.instruction.InstructionRef.deserialize_cbor(field)
        else:
            f_instruction = cqasm.v1.primitives.deserialize(cqasm.v1.instruction.InstructionRef, field)

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

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
        node = Instruction(f_instruction, f_name, f_condition, f_operands, f_annotations)

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

        # Serialize the instruction field.
        if hasattr(self._attr_instruction, 'serialize_cbor'):
            cbor['instruction'] = self._attr_instruction.serialize_cbor()
        else:
            cbor['instruction'] = cqasm.v1.primitives.serialize(cqasm.v1.instruction.InstructionRef, self._attr_instruction)

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize the condition field.
        field = {'@T': '1'}
        if self._attr_condition is None:
            field['@t'] = None
        else:
            field.update(self._attr_condition._serialize(id_map))
        cbor['condition'] = field

        # Serialize the operands field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_operands:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
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

class Mapping(Annotated):
    """A mapping. That is, a user-defined identifier mapping to some value."""

    __slots__ = [
        '_attr_name',
        '_attr_value',
    ]

    def __init__(
        self,
        name=None,
        value=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.name = name
        self.value = value

    @property
    def name(self):
        """The name of the mapping."""
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
    def value(self):
        """The value it maps to."""
        return self._attr_value

    @value.setter
    def value(self, val):
        if val is None:
            del self.value
            return
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('value must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
        self._attr_value = val

    @value.deleter
    def value(self):
        self._attr_value = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Mapping):
            return False
        if self.name != other.name:
            return False
        if self.value != other.value:
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
        s.append('name: ')
        s.append(str(self.name) + '\n')
        s.append('  '*indent)
        s.append('value: ')
        if self.value is None:
            s.append('!MISSING\n')
        else:
            s.append('<\n')
            s.append(self.value.dump(indent + 1, annotations, links) + '\n')
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
        if self._attr_value is not None:
            self._attr_value.find_reachable(id_map)
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
        if self._attr_value is None:
            raise NotWellFormed('value is required but not set')
        if self._attr_value is not None:
            self._attr_value.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Mapping(
            name=self._attr_name,
            value=self._attr_value,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Mapping(
            name=_cloned(self._attr_name),
            value=_cloned(self._attr_value),
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

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the value field.
        field = cbor.get('value', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field value')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field value')
        if field.get('@t', None) is None:
            f_value = None
        else:
            f_value = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

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
        node = Mapping(f_name, f_value, f_annotations)

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

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize the value field.
        field = {'@T': '1'}
        if self._attr_value is None:
            field['@t'] = None
        else:
            field.update(self._attr_value._serialize(id_map))
        cbor['value'] = field

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

class Program(Node):
    """A complete program."""

    __slots__ = [
        '_attr_version',
        '_attr_num_qubits',
        '_attr_error_model',
        '_attr_subcircuits',
        '_attr_mappings',
        '_attr_variables',
        '_attr_api_version',
    ]

    def __init__(
        self,
        version=None,
        num_qubits=None,
        error_model=None,
        subcircuits=None,
        mappings=None,
        variables=None,
        api_version=None,
    ):
        super().__init__()
        self.version = version
        self.num_qubits = num_qubits
        self.error_model = error_model
        self.subcircuits = subcircuits
        self.mappings = mappings
        self.variables = variables
        self.api_version = api_version

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
        """The required qubit register size."""
        return self._attr_num_qubits

    @num_qubits.setter
    def num_qubits(self, val):
        if val is None:
            del self.num_qubits
            return
        if not isinstance(val, cqasm.v1.primitives.Int):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('num_qubits must be of type cqasm.v1.primitives.Int')
            val = cqasm.v1.primitives.Int(val)
        self._attr_num_qubits = val

    @num_qubits.deleter
    def num_qubits(self):
        self._attr_num_qubits = cqasm.v1.primitives.Int()

    @property
    def error_model(self):
        """Error model information."""
        return self._attr_error_model

    @error_model.setter
    def error_model(self, val):
        if val is None:
            del self.error_model
            return
        if not isinstance(val, ErrorModel):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('error_model must be of type ErrorModel')
            val = ErrorModel(val)
        self._attr_error_model = val

    @error_model.deleter
    def error_model(self):
        self._attr_error_model = None

    @property
    def subcircuits(self):
        """The list of subcircuit."""
        return self._attr_subcircuits

    @subcircuits.setter
    def subcircuits(self, val):
        if val is None:
            del self.subcircuits
            return
        if not isinstance(val, MultiSubcircuit):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('subcircuits must be of type MultiSubcircuit')
            val = MultiSubcircuit(val)
        self._attr_subcircuits = val

    @subcircuits.deleter
    def subcircuits(self):
        self._attr_subcircuits = MultiSubcircuit()

    @property
    def mappings(self):
        """The list of all user-defined mappings after parsing."""
        return self._attr_mappings

    @mappings.setter
    def mappings(self, val):
        if val is None:
            del self.mappings
            return
        if not isinstance(val, MultiMapping):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('mappings must be of type MultiMapping')
            val = MultiMapping(val)
        self._attr_mappings = val

    @mappings.deleter
    def mappings(self):
        self._attr_mappings = MultiMapping()

    @property
    def variables(self):
        """This list of all user-defined variables at any point in the code.
        Only used for API level 1.1+."""
        return self._attr_variables

    @variables.setter
    def variables(self, val):
        if val is None:
            del self.variables
            return
        if not isinstance(val, MultiVariable):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('variables must be of type MultiVariable')
            val = MultiVariable(val)
        self._attr_variables = val

    @variables.deleter
    def variables(self):
        self._attr_variables = MultiVariable()

    @property
    def api_version(self):
        """API version. This may be greater than or equal to the file version.
        This controls which fields of the tree are used, where such usage
        depends on the version."""
        return self._attr_api_version

    @api_version.setter
    def api_version(self, val):
        if val is None:
            del self.api_version
            return
        if not isinstance(val, cqasm.v1.primitives.Version):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('api_version must be of type cqasm.v1.primitives.Version')
            val = cqasm.v1.primitives.Version(val)
        self._attr_api_version = val

    @api_version.deleter
    def api_version(self):
        self._attr_api_version = cqasm.v1.primitives.Version()

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Program):
            return False
        if self.version != other.version:
            return False
        if self.num_qubits != other.num_qubits:
            return False
        if self.error_model != other.error_model:
            return False
        if self.subcircuits != other.subcircuits:
            return False
        if self.mappings != other.mappings:
            return False
        if self.variables != other.variables:
            return False
        if self.api_version != other.api_version:
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
        s.append(str(self.num_qubits) + '\n')
        s.append('  '*indent)
        s.append('error_model: ')
        if self.error_model is None:
            s.append('-\n')
        else:
            s.append('<\n')
            s.append(self.error_model.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + '>\n')
        s.append('  '*indent)
        s.append('subcircuits: ')
        if not self.subcircuits:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.subcircuits:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('mappings: ')
        if not self.mappings:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.mappings:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('variables: ')
        if not self.variables:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.variables:
                s.append(child.dump(indent + 1, annotations, links) + '\n')
            s.append('  '*indent + ']\n')
        s.append('  '*indent)
        s.append('api_version: ')
        s.append(str(self.api_version) + '\n')
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
        if self._attr_error_model is not None:
            self._attr_error_model.find_reachable(id_map)
        for el in self._attr_subcircuits:
            el.find_reachable(id_map)
        for el in self._attr_mappings:
            el.find_reachable(id_map)
        for el in self._attr_variables:
            el.find_reachable(id_map)
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
        if self._attr_error_model is not None:
            self._attr_error_model.check_complete(id_map)
        for child in self._attr_subcircuits:
            child.check_complete(id_map)
        for child in self._attr_mappings:
            child.check_complete(id_map)
        for child in self._attr_variables:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Program(
            version=self._attr_version,
            num_qubits=self._attr_num_qubits,
            error_model=self._attr_error_model,
            subcircuits=self._attr_subcircuits.copy(),
            mappings=self._attr_mappings.copy(),
            variables=self._attr_variables.copy(),
            api_version=self._attr_api_version
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
            error_model=_cloned(self._attr_error_model),
            subcircuits=_cloned(self._attr_subcircuits),
            mappings=_cloned(self._attr_mappings),
            variables=_cloned(self._attr_variables),
            api_version=_cloned(self._attr_api_version)
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
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_num_qubits = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_num_qubits = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Deserialize the error_model field.
        field = cbor.get('error_model', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field error_model')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field error_model')
        if field.get('@t', None) is None:
            f_error_model = None
        else:
            f_error_model = ErrorModel._deserialize(field, seq_to_ob, links)

        # Deserialize the subcircuits field.
        field = cbor.get('subcircuits', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field subcircuits')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field subcircuits')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_subcircuits = MultiSubcircuit()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_subcircuits.append(Subcircuit._deserialize(element, seq_to_ob, links))

        # Deserialize the mappings field.
        field = cbor.get('mappings', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field mappings')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field mappings')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_mappings = MultiMapping()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_mappings.append(Mapping._deserialize(element, seq_to_ob, links))

        # Deserialize the variables field.
        field = cbor.get('variables', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field variables')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field variables')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_variables = MultiVariable()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_variables.append(Variable._deserialize(element, seq_to_ob, links))

        # Deserialize the api_version field.
        field = cbor.get('api_version', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field api_version')
        if hasattr(cqasm.v1.primitives.Version, 'deserialize_cbor'):
            f_api_version = cqasm.v1.primitives.Version.deserialize_cbor(field)
        else:
            f_api_version = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Version, field)

        # Construct the Program node.
        node = Program(f_version, f_num_qubits, f_error_model, f_subcircuits, f_mappings, f_variables, f_api_version)

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
        if hasattr(self._attr_num_qubits, 'serialize_cbor'):
            cbor['num_qubits'] = self._attr_num_qubits.serialize_cbor()
        else:
            cbor['num_qubits'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_num_qubits)

        # Serialize the error_model field.
        field = {'@T': '?'}
        if self._attr_error_model is None:
            field['@t'] = None
        else:
            field.update(self._attr_error_model._serialize(id_map))
        cbor['error_model'] = field

        # Serialize the subcircuits field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_subcircuits:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['subcircuits'] = field

        # Serialize the mappings field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_mappings:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['mappings'] = field

        # Serialize the variables field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_variables:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['variables'] = field

        # Serialize the api_version field.
        if hasattr(self._attr_api_version, 'serialize_cbor'):
            cbor['api_version'] = self._attr_api_version.serialize_cbor()
        else:
            cbor['api_version'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Version, self._attr_api_version)

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
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
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
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
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
            f_body = Block._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

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

class SetInstruction(InstructionBase):
    """A version 1.2+ assignment instruction."""

    __slots__ = [
        '_attr_lhs',
        '_attr_rhs',
    ]

    def __init__(
        self,
        lhs=None,
        rhs=None,
        condition=None,
        annotations=None,
    ):
        super().__init__(condition=condition, annotations=annotations)
        self.lhs = lhs
        self.rhs = rhs

    @property
    def lhs(self):
        """The assignment target."""
        return self._attr_lhs

    @lhs.setter
    def lhs(self, val):
        if val is None:
            del self.lhs
            return
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('lhs must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
        self._attr_lhs = val

    @lhs.deleter
    def lhs(self):
        self._attr_lhs = None

    @property
    def rhs(self):
        """The value to assign."""
        return self._attr_rhs

    @rhs.setter
    def rhs(self, val):
        if val is None:
            del self.rhs
            return
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('rhs must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
        self._attr_rhs = val

    @rhs.deleter
    def rhs(self):
        self._attr_rhs = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, SetInstruction):
            return False
        if self.lhs != other.lhs:
            return False
        if self.rhs != other.rhs:
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
        s.append('SetInstruction(')
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
        if self._attr_lhs is not None:
            self._attr_lhs.find_reachable(id_map)
        if self._attr_rhs is not None:
            self._attr_rhs.find_reachable(id_map)
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
        if self._attr_lhs is None:
            raise NotWellFormed('lhs is required but not set')
        if self._attr_lhs is not None:
            self._attr_lhs.check_complete(id_map)
        if self._attr_rhs is None:
            raise NotWellFormed('rhs is required but not set')
        if self._attr_rhs is not None:
            self._attr_rhs.check_complete(id_map)
        if self._attr_condition is None:
            raise NotWellFormed('condition is required but not set')
        if self._attr_condition is not None:
            self._attr_condition.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return SetInstruction(
            lhs=self._attr_lhs,
            rhs=self._attr_rhs,
            condition=self._attr_condition,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return SetInstruction(
            lhs=_cloned(self._attr_lhs),
            rhs=_cloned(self._attr_rhs),
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
        if typ != 'SetInstruction':
            raise ValueError('found node serialization for ' + typ + ', but expected SetInstruction')

        # Deserialize the lhs field.
        field = cbor.get('lhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field lhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field lhs')
        if field.get('@t', None) is None:
            f_lhs = None
        else:
            f_lhs = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the rhs field.
        field = cbor.get('rhs', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field rhs')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field rhs')
        if field.get('@t', None) is None:
            f_rhs = None
        else:
            f_rhs = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the condition field.
        field = cbor.get('condition', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field condition')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field condition')
        if field.get('@t', None) is None:
            f_condition = None
        else:
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

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

        # Construct the SetInstruction node.
        node = SetInstruction(f_lhs, f_rhs, f_condition, f_annotations)

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
        cbor = {'@i': id_map[id(self)], '@t': 'SetInstruction'}

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


class MultiSetInstruction(_Multiple):
    """Wrapper for an edge with multiple SetInstruction objects."""

    _T = SetInstruction


_typemap['SetInstruction'] = SetInstruction

class Subcircuit(Annotated):
    """A subcircuit. That is, a named collection of bundles, possibly repeated a
    number of times."""

    __slots__ = [
        '_attr_name',
        '_attr_iterations',
        '_attr_bundles',
        '_attr_body',
    ]

    def __init__(
        self,
        name=None,
        iterations=None,
        bundles=None,
        annotations=None,
        body=None,
    ):
        super().__init__(annotations=annotations)
        self.name = name
        self.iterations = iterations
        self.bundles = bundles
        self.body = body

    @property
    def name(self):
        """The name of the subcircuit. If the file doesn't start with a
        subcircuit definition, this is empty for the first subcircuit."""
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
    def iterations(self):
        """An optional integer expression representing the number of iterations
        for this subcircuit. This is 1 when the iteration count is not
         specified."""
        return self._attr_iterations

    @iterations.setter
    def iterations(self, val):
        if val is None:
            del self.iterations
            return
        if not isinstance(val, cqasm.v1.primitives.Int):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('iterations must be of type cqasm.v1.primitives.Int')
            val = cqasm.v1.primitives.Int(val)
        self._attr_iterations = val

    @iterations.deleter
    def iterations(self):
        self._attr_iterations = cqasm.v1.primitives.Int()

    @property
    def bundles(self):
        """The instruction bundles contained by this subcircuit. Only used for
        API level 1.0 and 1.1; use body for 1.2+."""
        return self._attr_bundles

    @bundles.setter
    def bundles(self, val):
        if val is None:
            del self.bundles
            return
        if not isinstance(val, MultiBundle):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('bundles must be of type MultiBundle')
            val = MultiBundle(val)
        self._attr_bundles = val

    @bundles.deleter
    def bundles(self):
        self._attr_bundles = MultiBundle()

    @property
    def body(self):
        """The statements contained by this subcircuit. Only used for API level
        1.2+; use bundles for 1.0 and 1.1."""
        return self._attr_body

    @body.setter
    def body(self, val):
        if val is None:
            del self.body
            return
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
        self._attr_body = val

    @body.deleter
    def body(self):
        self._attr_body = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Subcircuit):
            return False
        if self.name != other.name:
            return False
        if self.iterations != other.iterations:
            return False
        if self.bundles != other.bundles:
            return False
        if self.annotations != other.annotations:
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
        s.append(str(self.name) + '\n')
        s.append('  '*indent)
        s.append('iterations: ')
        s.append(str(self.iterations) + '\n')
        s.append('  '*indent)
        s.append('bundles: ')
        if not self.bundles:
            s.append('-\n')
        else:
            s.append('[\n')
            for child in self.bundles:
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
        s.append('  '*indent)
        s.append('body: ')
        if self.body is None:
            s.append('-\n')
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
        for el in self._attr_bundles:
            el.find_reachable(id_map)
        for el in self._attr_annotations:
            el.find_reachable(id_map)
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
        for child in self._attr_bundles:
            child.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)
        if self._attr_body is not None:
            self._attr_body.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Subcircuit(
            name=self._attr_name,
            iterations=self._attr_iterations,
            bundles=self._attr_bundles.copy(),
            annotations=self._attr_annotations.copy(),
            body=self._attr_body
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
            bundles=_cloned(self._attr_bundles),
            annotations=_cloned(self._attr_annotations),
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
        if typ != 'Subcircuit':
            raise ValueError('found node serialization for ' + typ + ', but expected Subcircuit')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the iterations field.
        field = cbor.get('iterations', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field iterations')
        if hasattr(cqasm.v1.primitives.Int, 'deserialize_cbor'):
            f_iterations = cqasm.v1.primitives.Int.deserialize_cbor(field)
        else:
            f_iterations = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Int, field)

        # Deserialize the bundles field.
        field = cbor.get('bundles', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field bundles')
        if field.get('@T') != '*':
            raise ValueError('unexpected edge type for field bundles')
        data = field.get('@d', None)
        if not isinstance(data, list):
            raise ValueError('missing serialization of Any/Many contents')
        f_bundles = MultiBundle()
        for element in data:
            if element.get('@T') != '1':
                raise ValueError('unexpected edge type for Any/Many element')
            f_bundles.append(Bundle._deserialize(element, seq_to_ob, links))

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

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '?':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = Block._deserialize(field, seq_to_ob, links)

        # Construct the Subcircuit node.
        node = Subcircuit(f_name, f_iterations, f_bundles, f_annotations, f_body)

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
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

        # Serialize the iterations field.
        if hasattr(self._attr_iterations, 'serialize_cbor'):
            cbor['iterations'] = self._attr_iterations.serialize_cbor()
        else:
            cbor['iterations'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Int, self._attr_iterations)

        # Serialize the bundles field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_bundles:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['bundles'] = field

        # Serialize the annotations field.
        field = {'@T': '*'}
        lst = []
        for el in self._attr_annotations:
            el = el._serialize(id_map)
            el['@T'] = '1'
            lst.append(el)
        field['@d'] = lst
        cbor['annotations'] = field

        # Serialize the body field.
        field = {'@T': '?'}
        if self._attr_body is None:
            field['@t'] = None
        else:
            field.update(self._attr_body._serialize(id_map))
        cbor['body'] = field

        # Serialize annotations.
        for key, val in self._annot.items():
            cbor['{%s}' % key] = _py_to_cbor(cqasm.v1.primitives.serialize(key, val))

        return cbor


class MultiSubcircuit(_Multiple):
    """Wrapper for an edge with multiple Subcircuit objects."""

    _T = Subcircuit


_typemap['Subcircuit'] = Subcircuit

class Variable(Annotated):
    """A variable."""

    __slots__ = [
        '_attr_name',
        '_attr_typ',
    ]

    def __init__(
        self,
        name=None,
        typ=None,
        annotations=None,
    ):
        super().__init__(annotations=annotations)
        self.name = name
        self.typ = typ

    @property
    def name(self):
        """The name of the variable."""
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
    def typ(self):
        """The type of the variable."""
        return self._attr_typ

    @typ.setter
    def typ(self, val):
        if val is None:
            del self.typ
            return
        if not isinstance(val, cqasm.v1.types.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('typ must be of type cqasm.v1.types.Node')
            val = cqasm.v1.types.Node(val)
        self._attr_typ = val

    @typ.deleter
    def typ(self):
        self._attr_typ = None

    def __eq__(self, other):
        """Equality operator. Ignores annotations!"""
        if not isinstance(other, Variable):
            return False
        if self.name != other.name:
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
        s.append('Variable(')
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
        if self._attr_typ is None:
            raise NotWellFormed('typ is required but not set')
        if self._attr_typ is not None:
            self._attr_typ.check_complete(id_map)
        for child in self._attr_annotations:
            child.check_complete(id_map)

    def copy(self):
        """Returns a shallow copy of this node."""
        return Variable(
            name=self._attr_name,
            typ=self._attr_typ,
            annotations=self._attr_annotations.copy()
        )

    def clone(self):
        """Returns a deep copy of this node. This mimics the C++ interface,
        deficiencies with links included; that is, links always point to the
        original tree. If you're not cloning a subtree in a context where this
        is the desired behavior, you may want to use the copy.deepcopy() from
        the stdlib instead, which should copy links correctly."""
        return Variable(
            name=_cloned(self._attr_name),
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
        if typ != 'Variable':
            raise ValueError('found node serialization for ' + typ + ', but expected Variable')

        # Deserialize the name field.
        field = cbor.get('name', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field name')
        if hasattr(cqasm.v1.primitives.Str, 'deserialize_cbor'):
            f_name = cqasm.v1.primitives.Str.deserialize_cbor(field)
        else:
            f_name = cqasm.v1.primitives.deserialize(cqasm.v1.primitives.Str, field)

        # Deserialize the typ field.
        field = cbor.get('typ', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field typ')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field typ')
        if field.get('@t', None) is None:
            f_typ = None
        else:
            f_typ = cqasm.v1.types.Node._deserialize(field, seq_to_ob, links)

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

        # Construct the Variable node.
        node = Variable(f_name, f_typ, f_annotations)

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
        cbor = {'@i': id_map[id(self)], '@t': 'Variable'}

        # Serialize the name field.
        if hasattr(self._attr_name, 'serialize_cbor'):
            cbor['name'] = self._attr_name.serialize_cbor()
        else:
            cbor['name'] = cqasm.v1.primitives.serialize(cqasm.v1.primitives.Str, self._attr_name)

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


class MultiVariable(_Multiple):
    """Wrapper for an edge with multiple Variable objects."""

    _T = Variable


_typemap['Variable'] = Variable

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
        if not isinstance(val, cqasm.v1.values.Node):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('condition must be of type cqasm.v1.values.Node')
            val = cqasm.v1.values.Node(val)
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
        if not isinstance(val, Block):
            # Try to "typecast" if this isn't an obvious mistake.
            if isinstance(val, Node):
                raise TypeError('body must be of type Block')
            val = Block(val)
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
            f_condition = cqasm.v1.values.Node._deserialize(field, seq_to_ob, links)

        # Deserialize the body field.
        field = cbor.get('body', None)
        if not isinstance(field, dict):
            raise ValueError('missing or invalid serialization of field body')
        if field.get('@T') != '1':
            raise ValueError('unexpected edge type for field body')
        if field.get('@t', None) is None:
            f_body = None
        else:
            f_body = Block._deserialize(field, seq_to_ob, links)

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

