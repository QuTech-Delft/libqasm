/** \file
 * Implementation for \ref include/cqasm-version.hpp "cqasm-version.hpp".
 */

#include "cqasm-error.hpp"
#include "cqasm-version.hpp"
#include "cqasm-version-parser.hpp"
#include "cqasm-version-lexer.hpp"

#include <memory>

namespace cqasm {
namespace version {

/**
 * Constructs a version object from a string, defaulting to 1.0.
 */
Version::Version(const std::string &version) {
    if (version.empty()) {
        return;
    }
    size_t next, last = 0;
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
        auto lhs = i < this->size() ? (*this)[i] : 0;
        auto rhs = i < other.size() ? other[i] : 0;
        if (lhs > rhs) return 1;
        if (lhs < rhs) return -1;
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
        throw error::AnalysisError(std::string{ "Failed to construct scanner: " } + strerror(result));
    }
}

ScannerFlexBison::~ScannerFlexBison() {
    if (scanner_) {
        cqasm_version_lex_destroy(static_cast<yyscan_t>(scanner_));
    }
}


ScannerFlexBisonFile::ScannerFlexBisonFile(FILE *fp
) : fp_{ fp } {}

ScannerFlexBisonFile::~ScannerFlexBisonFile() {
    if (fp_) {
        fclose(fp_);
    }
}

int ScannerFlexBisonFile::parse(const std::string &filename, Version &version) const {
    cqasm_version_set_in(fp_, static_cast<yyscan_t>(scanner_));
    return cqasm_version_parse(static_cast<yyscan_t>(scanner_), filename, version);
}


ScannerFlexBisonString::ScannerFlexBisonString(const char *data
) : data_{ data } {}

int ScannerFlexBisonString::parse(const std::string &filename, Version &version) const {
    auto buffer = cqasm_version__scan_string(data_, static_cast<yyscan_t>(scanner_));
    if (!buffer) {
        throw error::AnalysisError("Failed to scan input data string.");
    }
    int result = cqasm_version_parse(static_cast<yyscan_t>(scanner_), filename, version);
    cqasm_version__delete_buffer(buffer, static_cast<yyscan_t>(scanner_));
    return result;
}


/**
 * Parse the given file to get its version number.
 * Throws an AnalysisError if this fails.
 */
Version parse_file(const std::string &filename) {
    // Open the file or pass the data buffer to flex.
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp) {
        throw error::AnalysisError(std::string{ "Failed to open input file " } + filename + ": " + strerror(errno));
    }
    auto scanner = std::unique_ptr<ScannerFlexBisonFile>(new ScannerFlexBisonFile(fp));
    return ParseHelper(filename, std::move(scanner)).parse();
}

/**
 * Parse using the given file pointer to get its version number.
 * Throws an AnalysisError if this fails.
 * The file is rewound back to the start when parsing completes.
 */
Version parse_file(FILE *fp, const std::string &filename) {
    auto scanner = std::unique_ptr<ScannerFlexBisonFile>(new ScannerFlexBisonFile(fp));
    auto version = ParseHelper(filename, std::move(scanner)).parse();
    if (fseek(fp, 0, SEEK_SET)) {
        throw error::AnalysisError("failed to rewind file pointer");
    }
    return version;
}

/**
 * Parse the given string as a file to get its version number.
 */
Version parse_string(const std::string &data, const std::string &filename) {
    auto scanner = std::unique_ptr<ScannerFlexBisonString>(new ScannerFlexBisonString(data.c_str()));
    return ParseHelper(filename, std::move(scanner)).parse();
}


ParseHelper::ParseHelper(const std::string &filename, std::unique_ptr<ScannerAdaptor> scanner_up
) : filename(filename), scanner_up_(std::move(scanner_up)) {}

/**
 * Does the actual parsing.
 */
Version ParseHelper::parse() {
    Version version{ "" };
    int result = scanner_up_->parse(filename, version);
    if (result == 2) {
        throw error::AnalysisError(std::string{"Out of memory while parsing '" } + filename + "'.");
    } else if (result != 0) {
        throw error::AnalysisError(std::string{ "Failed to parse '" } + filename + "'.");
    }
    if (version.empty()) {
        throw error::AnalysisError("Internal error: no version info nor error info was returned by version parser.");
    }
    return version;
}

} // namespace version
} // namespace cqasm
