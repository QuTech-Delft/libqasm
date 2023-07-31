/** \file
 * Defines utilities for detecting and dealing with cQASM language versions.
 */

#pragma once

#include "cqasm-error.hpp"

#include <string>
#include <cstdint>
#include <complex>
#include <memory>
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
    virtual ~ScannerAdaptor() = default;

    virtual void parse(const std::string &filename, Version &version) const = 0;
};


class ScannerFlexBison : public ScannerAdaptor {
protected:
    void *scanner_{ nullptr };
    void parse_(const std::string &filename, Version &version) const;
public:
    ScannerFlexBison();
    ~ScannerFlexBison() override;
    void parse(const std::string &filename, Version &version) const override = 0;
};


class ScannerFlexBisonFile : public ScannerFlexBison {
    FILE *fp_{ nullptr };
public:
    explicit ScannerFlexBisonFile(FILE *fp);
    ~ScannerFlexBisonFile() override = default;
    void parse(const std::string &filename, Version &version) const override;
};


class ScannerFlexBisonString : public ScannerFlexBison {
    const char *data_{ nullptr };
public:
    explicit ScannerFlexBisonString(const char *data);
    ~ScannerFlexBisonString() override = default;
    void parse(const std::string &filename, Version &version) const override;
};


/**
 * Parse the given file to get its version number.
 * Throws an AnalysisError if this fails.
 */
Version parse_file(const std::string &filename);

/**
 * Parse using the given file pointer to get its version number.
 * Throws an AnalysisError if this fails.
 * The file is rewound back to the start when parsing completes.
 * A filename may be given in addition for use within the AnalysisError thrown when version parsing fails.
 */
Version parse_file(FILE *file, const std::string &filename = "<unknown>");

/**
 * Parse the given string as a file to get its version number.
 * A filename may be given in addition for use within the AnalysisError thrown when version parsing fails.
 */
Version parse_string(const std::string &data, const std::string &filename = "<unknown>");


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
    std::string filename;

public:
    /**
     * Parse a file with flex/bison.
     */
    explicit ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, std::string filename = "<unknown>");

    /**
     * Does the actual parsing.
     */
    Version parse();
};


} // namespace cqasm::version
