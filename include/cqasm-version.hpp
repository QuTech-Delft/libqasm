/** \file
 * Defines utilities for detecting and dealing with cQASM language versions.
 */

#pragma once

#include "cqasm-error.hpp"

#include <cstdint>  // int64_t
#include <cstdio>  // FILE*
#include <fmt/ostream.h>
#include <memory>  // unique_ptr
#include <ostream>
#include <string>
#include <vector>

/**
 * Namespace for detecting and dealing with cQASM language versions.
 */
namespace cqasm::version {

/**
 * Version number primitive used within the AST and semantic trees.
 */
class Version : public std::vector<std::int64_t> {
public:
    /**
     * Default constructor.
     */
    Version() = default;

    /**
     * Constructs a version object from a string.
     */
    explicit Version(const std::string &version);

    /**
     * Compares this version against the other version.
     * Returns:
     *   1 if this version is newer than the other,
     *   -1 if this version is older than the other, or
     *   0 if both versions are the same.
     * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
     */
    [[nodiscard]] int compare(const Version &other) const;

    /**
     * Compares this version against the other version.
     * Returns:
     *   1 if this version is newer than the other,
     *   -1 if this version is older than the other, or
     *   0 if both versions are the same.
     * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
     */
    [[nodiscard]] int compare(const std::string &other) const;
};

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object);


struct ScannerAdaptor {
    virtual ~ScannerAdaptor();

    virtual void parse(const std::string &file_name, Version &version) const = 0;
};


class ScannerFlexBison : public ScannerAdaptor {
protected:
    void *scanner_{ nullptr };
    void parse_(const std::string &file_name, Version &version) const;
public:
    ScannerFlexBison();
    ~ScannerFlexBison() override;
    void parse(const std::string &file_name, Version &version) const override = 0;
};


class ScannerFlexBisonFile : public ScannerFlexBison {
    FILE *fp_{ nullptr };
public:
    explicit ScannerFlexBisonFile(FILE *fp);
    ~ScannerFlexBisonFile() override;
    void parse(const std::string &file_name, Version &version) const override;
};


class ScannerFlexBisonString : public ScannerFlexBison {
    const char *data_{ nullptr };
public:
    explicit ScannerFlexBisonString(const char *data);
    ~ScannerFlexBisonString() override;
    void parse(const std::string &file_name, Version &version) const override;
};


/**
 * Parse the given file path to get its version number.
 * Throws an AnalysisError if this fails.
 */
Version parse_file(const std::string &file_path);

/**
 * Parse using the given file pointer to get its version number.
 * Throws an AnalysisError if this fails.
 * A file_name may be given in addition for use within the AnalysisError thrown when version parsing fails.
 */
Version parse_file(FILE* fp, const std::string &file_name = "<unknown>");

/**
 * Parse the given string as a file to get its version number.
 * A file_name may be given in addition for use within the AnalysisError thrown when version parsing fails.
 */
Version parse_string(const std::string &data, const std::string &file_name = "<unknown>");


/**
 * Internal helper class for parsing cQASM file versions.
 */
class ParseHelper {
    /**
     * Scanner doing the actual parsing.
     */
    std::unique_ptr<ScannerAdaptor> scanner_up_;

    /**
     * Name of the file being parsed.
     */
    std::string file_name;

public:
    explicit ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, std::string file_name = "<unknown>");

    /**
     * Does the actual parsing.
     */
    Version parse();
};

} // namespace cqasm::version

/**
 * std::ostream support via fmt (uses operator<<).
 */
template <> struct fmt::formatter<cqasm::version::Version> : ostream_formatter {};
