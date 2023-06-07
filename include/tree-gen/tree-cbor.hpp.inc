/** \file
 * Generalized contents of tree-cbor.hpp.
 */

#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <stack>

TREE_NAMESPACE_BEGIN

/**
 * Namespace for CBOR utilities.
 */
namespace cbor {

// Forward declaration to Reader so we can do the ArrayReader and MapReader
// typedefs.
class Reader;

/**
 * Used to read a CBOR array. Use the at() method to query indices with bounds
 * checking.
 */
using ArrayReader = TREE_VECTOR(Reader);

/**
 * Used to read a CBOR map. Use the at() method to query keys with bounds
 * checking.
 */
using MapReader = TREE_MAP(std::string, Reader);

/**
 * Utility class for reading RFC7049 CBOR objects.
 */
class Reader {
private:

    /**
     * Pointer to the complete Cbor object.
     */
    std::shared_ptr<std::string> data;

    /**
     * Start offset of the represented slice within data.
     */
    size_t slice_offset;

    /**
     * Length of the represented slice within data.
     */
    size_t slice_length;

public:

    /**
     * Turns the given std::string that consists of an RFC7049 CBOR object into
     * a Reader representation that may be used to parse it.
     */
    explicit Reader(const std::string &data);

    /**
     * Turns the given std::string that consists of an RFC7049 CBOR object into
     * a Reader representation that may be used to parse it.
     */
    explicit Reader(std::string &&data);

private:

    /**
     * Constructs a subslice of this slice.
     */
    Reader(const Reader &parent, size_t offset, size_t length);

    /**
     * Returns a subslice of this slice.
     */
    Reader slice(size_t offset, size_t length) const;

    /**
     * Returns the byte at the given offset after range-checking.
     */
    uint8_t read_at(size_t offset) const;

    /**
     * Parses the additional information and reads any additional bytes it specifies
     * the existence of, and returns the encoded integer. offset should point to the
     * byte immediately following the initial byte. offset is moved forward past the
     * integer data.
     */
    uint64_t read_intlike(uint8_t info, size_t &offset) const;

    /**
     * Reads the string representation of this slice for both binary and UTF8
     * strings alike. Assumes that the slice is actually a string. offset must start
     * at 0, and is moved to the end of the string. The string is written to the
     * output stream.
     */
    void read_stringlike(size_t &offset, std::ostream &s) const;

    /**
     * Checks validity of the object at the given offset and seeks past it by
     * moving offset to the byte immediately following the object.
     */
    void check_and_seek(size_t &offset) const;

    /**
     * Tests whether the structure is valid CBOR for as far as we know about
     * it. Throws a TREE_RUNTIME_ERROR with an appropriate message if not.
     */
    void check() const;

    /**
     * Returns the name of the type corresponding to this CBOR object slice. This
     * returns one of:
     *  - "null"
     *  - "boolean"
     *  - "integer"
     *  - "float"
     *  - "binary string"
     *  - "UTF8 string"
     *  - "array"
     *  - "map"
     *  - "unknown type"
     */
    const char *get_type_name() const;

public:

    /**
     * Checks whether the object represented by this slice is null.
     */
    bool is_null() const;

    /**
     * Throws an unexpected value type error through a TREE_RUNTIME_ERROR if
     * the object represented by this slice is not null.
     */
    void as_null() const;

    /**
     * Checks whether the object represented by this slice is a boolean.
     */
    bool is_bool() const;

    /**
     * Returns the boolean representation of this slice. If it's not a boolean,
     * an unexpected value type error is thrown through a TREE_RUNTIME_ERROR.
     */
    bool as_bool() const;

    /**
     * Checks whether the object represented by this slice is an integer.
     */
    bool is_int() const;

    /**
     * Returns the integer representation of this slice. If it's not an
     * integer, an unexpected value type error is thrown through a
     * TREE_RUNTIME_ERROR.
     */
    int64_t as_int() const;

    /**
     * Checks whether the object represented by this slice is a float. Only
     * double precision is supported.
     */
    bool is_float() const;

    /**
     * Returns the floating point representation of this slice. If it's not a
     * float, an unexpected value type error is thrown through a
     * TREE_RUNTIME_ERROR. Only double precision is supported.
     */
    double as_float() const;

    /**
     * Checks whether the object represented by this slice is a Unicode string.
     */
    bool is_string() const;

    /**
     * Returns the string representation of this slice. If it's not a Unicode
     * string, an unexpected value type error is thrown through a
     * TREE_RUNTIME_ERROR.
     */
    std::string as_string() const;

    /**
     * Checks whether the object represented by this slice is a binary string.
     */
    bool is_binary() const;

    /**
     * Returns the string representation of this slice. If it's not a binary
     * string, an unexpected value type error is thrown through a
     * TREE_RUNTIME_ERROR.
     */
    std::string as_binary() const;

    /**
    * Checks whether the object represented by this slice is an array.
    */
    bool is_array() const;

private:

    /**
     * Reads the array item at the given offset into the array and advances the
     * offset past the item data.
     */
    void read_array_item(size_t &offset, ArrayReader &ar) const;

public:

    /**
     * Returns the array representation of this slice. If it's not an array,
     * an unexpected value type error is thrown through a TREE_RUNTIME_ERROR.
     */
    ArrayReader as_array() const;

    /**
     * Checks whether the object represented by this slice is a map/object.
     */
    bool is_map() const;

private:

    /**
     * Reads the map item at the given offset into the array and advances the
     * offset past the item data.
     */
    void read_map_item(size_t &offset, MapReader &map) const;

public:

    /**
     * Returns the map/object representation of this slice. If it's not a map,
     * an unexpected value type error is thrown through a TREE_RUNTIME_ERROR.
     */
    MapReader as_map() const;

    /**
     * Returns a copy of the CBOR slice in the form of a binary string.
     */
    std::string get_contents() const;

};

// Forward declarations for the writer classes, so we can use them in friend
// declarations.
class Writer;
class ArrayWriter;
class MapWriter;

/**
 * Base class for writing RFC7049 CBOR arrays and maps in streaming fashion.
 */
class StructureWriter {
private:

    /**
     * Pointer to the Writer object we belong to.
     */
    Writer *writer;

    /**
     * Our identifier as given by the Writer at construction time. If our ID is
     * not at the top of the stack, we're not allowed to write.
     */
    size_t id;

protected:

    /**
     * Constructs a structure writer and makes it the active writer.
     */
    explicit StructureWriter(Writer &writer);

    /**
     * Returns a reference to the underlying output stream if and only if we're
     * the active writer. Otherwise an exception is thrown.
     */
    std::ostream &stream();

    /**
     * Writes a null value to the structure.
     */
    void write_null();

    /**
     * Writes a boolean value to the structure.
     */
    void write_bool(bool value);

    /**
     * Writes an integer value to the structure. The major code can be
     * overridden from 0/1 to something else to specify a length, but in that
     * case value should be positive.
     */
    void write_int(int64_t value, uint8_t major=0);

    /**
     * Writes a float value to the structure. Only doubles are supported.
     */
    void write_float(double value);

    /**
     * Writes a Unicode string value to the structure.
     */
    void write_string(const std::string &value);

    /**
     * Writes a binary string value to the structure.
     */
    void write_binary(const std::string &value);

    /**
     * Starts writing an array to the structure. The array is constructed in a
     * streaming fashion using the return value. It must be close()d or go out
     * of scope before the next value can be written to this structure.
     */
    ArrayWriter write_array();

    /**
     * Starts writing a map to the structure. The map is constructed in a
     * streaming fashion using the return value. It must be close()d or go out
     * of scope before the next value can be written to this structure.
     */
    MapWriter write_map();

public:

    /**
     * Virtual destructor. This calls close() if we're the active writer, but
     * assumes close() was called manually if not.
     */
    virtual ~StructureWriter();

    // Delete copy constructor; deletion of one of these objects closes the
    // structure, so any copies would become invalid.
    StructureWriter(StructureWriter &src) = delete;
    StructureWriter &operator=(const StructureWriter &src) = delete;

    // Move constructor/assignment is fine though. The original will be made
    // invalid by clearing the ID field, so it doesn't close the structure upon
    // deletion.
    StructureWriter(StructureWriter &&src);
    StructureWriter &operator=(StructureWriter &&src);

    /**
     * Terminates the structure that we were writing with a break code, and
     * hands over control to the parent writer (if any).
     */
    void close();

};

/**
 * Class to handle writing RFC7049 CBOR arrays in streaming fashion.
 */
class ArrayWriter : public StructureWriter {
protected:

    /**
     * StructureWriters can nest ArrayWriters.
     */
    friend class StructureWriter;

    /**
     * Constructs a new array writer, makes it the active writer, and writes the
     * array header.
     */
    explicit ArrayWriter(Writer &writer);

public:

    /**
     * Writes a null value to the array.
     */
    void append_null();

    /**
     * Writes a boolean value to the array.
     */
    void append_bool(bool value);

    /**
     * Writes an integer value to the array.
     */
    void append_int(int64_t value);

    /**
     * Writes a float value to the array. Only doubles are supported.
     */
    void append_float(double value);

    /**
     * Writes a Unicode string value to the array.
     */
    void append_string(const std::string &value);

    /**
     * Writes a binary string value to the array.
     */
    void append_binary(const std::string &value);

    /**
     * Starts writing a nested array to the array. The array is constructed in a
     * streaming fashion using the return value. It must be close()d or go out
     * of scope before the next value can be written to this array.
     */
    ArrayWriter append_array();

    /**
     * Starts writing a map to the array. The map is constructed in a streaming
     * fashion using the return value. It must be close()d or go out of scope
     * before the next value can be written to this array.
     */
    MapWriter append_map();

};

/**
 * Class to handle writing RFC7049 CBOR maps in streaming fashion.
 */
class MapWriter : public StructureWriter {
protected:

    /**
     * The toplevel Writer is allowed to construct a MapWriter for the toplevel.
     */
    friend class Writer;

    /**
     * StructureWriters can nest MapWriters.
     */
    friend class StructureWriter;

    /**
     * Constructs a new map writer, makes it the active writer, and writes the
     * map header.
     */
    explicit MapWriter(Writer &writer);

public:

    /**
     * Writes a null value to the map with the given key.
     */
    void append_null(const std::string &key);

    /**
     * Writes a boolean value to the map with the given key.
     */
    void append_bool(const std::string &key, bool value);

    /**
     * Writes an integer value to the map with the given key.
     */
    void append_int(const std::string &key, int64_t value);

    /**
     * Writes a float value to the map with the given key. Only doubles are
     * supported.
     */
    void append_float(const std::string &key, double value);

    /**
     * Writes a Unicode string value to the map with the given key.
     */
    void append_string(const std::string &key, const std::string &value);

    /**
     * Writes a binary string value to the map with the given key.
     */
    void append_binary(const std::string &key, const std::string &value);

    /**
     * Starts writing an array to the map with the given key. The array is
     * constructed in a streaming fashion using the return value. It must be
     * close()d or go out of scope before the next value can be written to this
     * map.
     */
    ArrayWriter append_array(const std::string &key);

    /**
     * Starts writing a nested map to the map with the given key. The map is
     * constructed in a streaming fashion using the return value. It must be
     * close()d or go out of scope before the next value can be written to this
     * map.
     */
    MapWriter append_map(const std::string &key);

};

/**
 * Utility class for writing RFC7049 CBOR objects.
 */
class Writer {
private:

    /**
     * StructureWriter needs to be able to query the stack.
     */
    friend StructureWriter;

    /**
     * The stream we're writing to.
     */
    std::ostream &stream;

    /**
     * Stack of writer identifiers. The only writer that's allowed to write to
     * the output stream is the one with the ID at the top of this stack. This
     * allows incorrect usage of the writer to be detected.
     */
    std::stack<size_t> stack;

    /**
     * Sequence counter for generating writer IDs.
     */
    size_t id_counter;

public:

    /**
     * Creates a CBOR writer that writes to the given stream.
     */
    Writer(std::ostream &stream);

    /**
     * Returns the toplevel map writer. This can only be done when no other
     * writer is active. It is technically legal to call this multiple times to
     * write multiple structures back-to-back, but this is not used for
     * serializing trees.
     */
    MapWriter start();

};

} // namespace cbor
TREE_NAMESPACE_END