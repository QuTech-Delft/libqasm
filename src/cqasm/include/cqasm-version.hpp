/** \file
 * Defines utilities for detecting and dealing with cQASM language versions.
 */

#pragma once

#include <string>
#include <cstdint>
#include <complex>
#include <vector>

namespace cqasm {

/**
 * Namespace for detecting and dealing with cQASM language versions.
 */
namespace version {

/**
 * Version number primitive used within the AST and semantic trees.
 */
class Version : public std::vector<std::int64_t> {
public:

    /**
     * Constructs a version object from a string, defaulting to 1.0.
     */
    explicit Version(const std::string &version = "1.0");

    /**
     * Compares this version against the other version. Returns 1 if this version
     * is newer than the other, returns -1 if this version is older than the other,
     * or returns 0 if both versions are the same. When there is a mismatch in the
     * number of components between the versions, missing components are interpreted
     * as 0.
     */
    int compare(const Version &other) const;

    /**
     * Compares this version against the other version. Returns 1 if this version
     * is newer than the other, returns -1 if this version is older than the other,
     * or returns 0 if both versions are the same. When there is a mismatch in the
     * number of components between the versions, missing components are interpreted
     * as 0.
     */
    int compare(const std::string &other) const;

};

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object);

/**
 * Parse the given file to get its version number. Throws an AnalysisError if
 * this fails.
 */
Version parse_file(const std::string &filename);

/**
 * Parse using the given file pointer to get its version number. Throws an
 * AnalysisError if this fails. The file is rewound back to the start when
 * parsing completes.
 */
Version parse_file(FILE *file, const std::string &filename = "<unknown>");

/**
 * Parse the given string as a file to get its version number. A filename may
 * be given in addition for use within the AnalysisError thrown when version
 * parsing fails.
 */
Version parse_string(const std::string &data, const std::string &filename="<unknown>");

/**
 * Internal helper class for parsing cQASM file versions.
 */
class ParseHelper {
public:

    /**
     * File pointer being scanned, if no data was specified.
     */
    FILE *fptr = nullptr;

    /**
     * Flex data buffer, if data was specified.
     */
    void *buf = nullptr;

    /**
     * Flex reentrant scanner data.
     */
    void *scanner = nullptr;

    /**
     * Name of the file being parsed.
     */
    std::string filename;

    /**
     * The parse result.
     */
    Version version;

private:
    friend Version parse_file(const std::string &filename);
    friend Version parse_file(FILE *file, const std::string &filename);
    friend Version parse_string(const std::string &data, const std::string &filename);

    /**
     * Parse a string or file with flex/bison. If use_file is set, the file
     * specified by filename is read and data is ignored. Otherwise, filename
     * is used only for error messages, and data is read instead. Don't use
     * this directly, use parse().
     */
    ParseHelper(const std::string &filename, const std::string &data, bool use_file);

    /**
     * Construct the analyzer internals for the given filename, and analyze
     * the file.
     */
    ParseHelper(const std::string &filename, FILE *fptr);

    /**
     * Initializes the scanner. Returns whether this was successful.
     */
    bool construct();

    /**
     * Does the actual parsing.
     */
    void parse();

public:

    /**
     * Destroys the parse helper.
     */
    virtual ~ParseHelper();

    /**
     * Pushes an error.
     */
    static void push_error(const std::string &error);

};

} // namespace version
} // namespace cqasm
