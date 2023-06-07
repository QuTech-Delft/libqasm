/** \file
 * Implementation for \ref include/cqasm-version.hpp "cqasm-version.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-version.hpp"
#include "../include/cqasm-version-parser.hpp"
#include "../include/cqasm-version-lexer.hpp"

#include <memory>

namespace cqasm::version {

/**
 * Constructs a version object from a string, defaulting to 1.0.
 */
Version::Version(const std::string &version) {
    if (version.empty()) {
        return;
    }
    size_t next = 0;
    size_t last = 0;
    while ((next = version.find('.', last)) != std::string::npos) {
        push_back(std::stoi(version.substr(last, next - last)));
        last = next + 1;
    }
    push_back(std::stoi(version.substr(last)));
    for (auto component : *this) {
        if (component < 0) {
            throw std::invalid_argument("version component below zero");
        }
    }
}

/**
 * Compares this version against the other version.
 * Returns:
 *   1 if this version is newer than the other,
 *   -1 if this version is older than the other, or
 *   0 if both versions are the same.
 * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
 */
int Version::compare(const Version &other) const {
    for (size_t i = 0; i < this->size() || i < other.size(); i++) {
        auto lhs = (i < this->size())
            ? (*this)[i]
            : 0;
        auto rhs = (i < other.size())
            ? other[i]
            : 0;
        if (lhs > rhs) {
            return 1;
        }
        if (lhs < rhs) {
            return -1;
        }
    }
    return 0;
}

/**
 * Compares this version against the other version.
 * Returns:
 *   1 if this version is newer than the other,
 *   -1 if this version is older than the other, or
 *   0 if both versions are the same.
 * When there is a mismatch in the number of components between the versions, missing components are interpreted as 0.
 */
int Version::compare(const std::string &other) const {
    return compare(Version(other));
}

/**
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object) {
    bool first = true;
    for (auto item : object) {
        if (first) {
            first = false;
        } else {
            os << ".";
        }
        os << item;
    }
    return os;
}


ScannerFlexBison::ScannerFlexBison() {
    int result = cqasm_version_lex_init(static_cast<yyscan_t*>(&scanner_));
    if (result != 0) {
        throw error::AnalysisError(std::string("ScannerFlexBison failed to initialize lexer: ") + strerror(result));
    }
}

ScannerFlexBison::~ScannerFlexBison() {
    if (scanner_) {
        cqasm_version_lex_destroy(static_cast<yyscan_t>(scanner_));
    }
}

void ScannerFlexBison::parse_(const std::string &filename, Version &version) const {
    auto result = cqasm_version_parse(static_cast<yyscan_t>(scanner_), filename, version);
    if (result == 2) {
        throw error::AnalysisError(
            std::string("ScannerFlexBison::parse_: out of memory while parsing '") + filename + "'.");
    } else if (result != 0) {
        throw error::AnalysisError(
            std::string("ScannerFlexBison::parse_: failed to parse '") + filename + "'.");
    }
}


ScannerFlexBisonFile::ScannerFlexBisonFile(FILE *fp
) : fp_{ fp } {
    if (!fp_) {
        throw error::AnalysisError("ScannerFlexBisonFile couldn't access file.");
    }
}

void ScannerFlexBisonFile::parse(const std::string &filename, Version &version) const {
    cqasm_version_set_in(fp_, static_cast<yyscan_t>(scanner_));
    parse_(filename, version);
}


ScannerFlexBisonString::ScannerFlexBisonString(const char *data
) : data_{ data } {}

void ScannerFlexBisonString::parse(const std::string &filename, Version &version) const {
    auto buffer = cqasm_version__scan_string(data_, static_cast<yyscan_t>(scanner_));
    if (!buffer) {
        throw error::AnalysisError("ScannerFlexBisonString failed to scan input data string.");
    }
    parse_(filename, version);
    cqasm_version__delete_buffer(buffer, static_cast<yyscan_t>(scanner_));
}


/**
 * Parse the given file to get its version number.
 * Throws an AnalysisError if this fails.
 */
Version parse_file(const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp) {
        throw error::AnalysisError(
            std::string("parse_file failed to open input file '") + filename + "': " + strerror(errno) + ".");
    }
    auto scanner_up = std::make_unique<ScannerFlexBisonFile>(fp);
    auto version = ParseHelper(filename, std::move(scanner_up)).parse();
    fclose(fp);
    return version;
}

/**
 * Parse using the given file pointer to get its version number.
 * Throws an AnalysisError if this fails.
 * The file is rewound back to the start when parsing completes.
 */
Version parse_file(FILE *fp, const std::string &filename) {
    auto scanner_up = std::make_unique<ScannerFlexBisonFile>(fp);
    auto version = ParseHelper(filename, std::move(scanner_up)).parse();
    rewind(fp);
    return version;
}

/**
 * Parse the given string as a file to get its version number.
 */
Version parse_string(const std::string &data, const std::string &filename) {
    auto scanner_up = std::make_unique<ScannerFlexBisonString>(data.c_str());
    return ParseHelper(filename, std::move(scanner_up)).parse();
}


ParseHelper::ParseHelper(std::string filename, std::unique_ptr<ScannerAdaptor> scanner_up
) : filename(std::move(filename)), scanner_up_(std::move(scanner_up)) {}

/**
 * Does the actual parsing.
 */
Version ParseHelper::parse() {
    Version version;
    scanner_up_->parse(filename, version);
    if (version.empty()) {
        throw error::AnalysisError("ParseHelper::parse: no version info nor error info was returned by version parser.");
    }
    return version;
}

} // namespace cqasm::version
