/** \file
 * Implementation for \ref include/cqasm-version.hpp "cqasm-version.hpp".
 */

#include "cqasm-annotations-constants.hpp"
#include "cqasm-error.hpp"
#include "cqasm-version.hpp"
#include "cqasm-version-parser.hpp"
#include "cqasm-version-lexer.hpp"
#include "flex_bison_parser.hpp"

#include <fmt/format.h>
#include <memory>

namespace cqasm::version {

/**
 * Constructs a version object from a string.
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
 * Constructs a version object from an array of chars.
 */
Version::Version(const char *version)
: Version(std::string{ version }) {}

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
 * Stream << overload for version nodes.
 */
std::ostream &operator<<(std::ostream &os, const Version &object) {
    return os << fmt::format("{}", fmt::join(object, "."));
}


ScannerAdaptor::~ScannerAdaptor() {}


ScannerFlexBison::ScannerFlexBison() {
    if (int result = cqasm_version_lex_init(static_cast<yyscan_t*>(&scanner_)); result != 0) {
        throw error::ParseError(std::string("ScannerFlexBison failed to initialize lexer: ") + strerror(result));
    }
}

ScannerFlexBison::~ScannerFlexBison() {
    if (scanner_) {
        cqasm_version_lex_destroy(static_cast<yyscan_t>(scanner_));
    }
}

void ScannerFlexBison::parse_(const std::string &file_name, Version &version) const {
    if (auto result = cqasm_version_parse(static_cast<yyscan_t>(scanner_), file_name, version);
        result == cqasm::parser::flex_bison_parser::error_memory_exhausted) {
        throw error::ParseError(
            std::string("ScannerFlexBison::parse_: out of memory while parsing '") + file_name + "'.");
    } else if (result != 0) {
        throw error::ParseError(
            std::string("ScannerFlexBison::parse_: failed to parse '") + file_name + "'.");
    }
}


ScannerFlexBisonFile::ScannerFlexBisonFile(FILE *fp)
: fp_{ fp } {
    if (!fp_) {
        throw error::ParseError("ScannerFlexBisonFile couldn't access file.");
    }
}

ScannerFlexBisonFile::~ScannerFlexBisonFile() {}

void ScannerFlexBisonFile::parse(const std::string &file_name, Version &version) const {
    cqasm_version_set_in(fp_, static_cast<yyscan_t>(scanner_));
    parse_(file_name, version);
}


ScannerFlexBisonString::ScannerFlexBisonString(const char *data)
: data_{ data } {}

ScannerFlexBisonString::~ScannerFlexBisonString() {}

void ScannerFlexBisonString::parse(const std::string &file_name, Version &version) const {
    auto buffer = cqasm_version__scan_string(data_, static_cast<yyscan_t>(scanner_));
    if (!buffer) {
        throw error::ParseError("ScannerFlexBisonString failed to scan input data string.");
    }
    parse_(file_name, version);
    cqasm_version__delete_buffer(buffer, static_cast<yyscan_t>(scanner_));
}


/**
 * Parse the given file path to get its version number.
 * Throws a ParseError if this fails.
 */
Version parse_file(const std::string &file_path) {
    FILE *fp = fopen(file_path.c_str(), "r");
    if (!fp) {
        throw error::ParseError(
            std::string("parse_file failed to open input file '") + file_path + "': " + strerror(errno) + ".");
    }
    auto scanner_up = std::make_unique<ScannerFlexBisonFile>(fp);
    auto version = ParseHelper(std::move(scanner_up), file_path).parse();
    fclose(fp);
    return version;
}

/**
 * Parse using the given file pointer to get its version number.
 * Throws a ParseError if this fails.
 * The file is rewound back to the start when parsing completes.
 */
Version parse_file(FILE *fp, const std::optional<std::string> &file_name) {
    auto scanner_up = std::make_unique<ScannerFlexBisonFile>(fp);
    auto version = ParseHelper(std::move(scanner_up), file_name).parse();
    rewind(fp);
    return version;
}

/**
 * Parse the given string as a file to get its version number.
 */
Version parse_string(const std::string &data, const std::optional<std::string> &file_name) {
    auto scanner_up = std::make_unique<ScannerFlexBisonString>(data.c_str());
    return ParseHelper(std::move(scanner_up), file_name).parse();
}


ParseHelper::ParseHelper(std::unique_ptr<ScannerAdaptor> scanner_up, const std::optional<std::string> &file_name)
: scanner_up_{ std::move(scanner_up) }
, file_name{ file_name.value_or(annotations::unknown_file_name) }
{}

/**
 * Does the actual parsing.
 */
Version ParseHelper::parse() {
    Version version;
    scanner_up_->parse(file_name, version);
    if (version.empty()) {
        throw error::ParseError{
            "ParseHelper::parse: no version info nor error info was returned by version parser." };
    }
    return version;
}

} // namespace cqasm::version
