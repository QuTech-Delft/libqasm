/** \file
 * Implementation for \ref include/cqasm-version.hpp "cqasm-version.hpp".
 */

#include "cqasm-version.hpp"
#include "cqasm-error.hpp"
#include "cqasm-version-parser.hpp"
#include "cqasm-version-lexer.hpp"

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
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
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
 * Compares this version against the other version. Returns 1 if this version
 * is newer than the other, returns -1 if this version is older than the other,
 * or returns 0 if both versions are the same. When there is a mismatch in the
 * number of components between the versions, missing components are interpreted
 * as 0.
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

/**
 * Parse the given file to get its version number. Throws an AnalysisError if
 * this fails.
 */
Version parse_file(const std::string &filename) {
    return std::move(ParseHelper(filename, "", true).version);
}

/**
 * Parse using the given file pointer to get its version number. Throws an
 * AnalysisError if this fails. The file is rewound back to the start when
 * parsing completes.
 */
Version parse_file(FILE *file, const std::string &filename) {
    auto retval = std::move(ParseHelper(filename, file).version);
    if (fseek(file, 0, SEEK_SET)) {
        throw error::AnalysisError("failed to rewind file pointer");
    }
    return retval;
}

/**
 * Parse the given string as a file to get its version number. A filename may
 * be given in addition for use within the AnalysisError thrown when version
 * parsing fails.
 */
Version parse_string(const std::string &data, const std::string &filename) {
    return std::move(ParseHelper(filename, data, false).version);
}

/**
 * Parse a string or file with flex/bison. If use_file is set, the file
 * specified by filename is read and data is ignored. Otherwise, filename
 * is used only for error messages, and data is read instead. Don't use
 * this directly, use parse().
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    const std::string &data,
    bool use_file
) : filename(filename), version("") {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    if (use_file) {
        fptr = fopen(filename.c_str(), "r");
        if (!fptr) {
            std::ostringstream sb;
            sb << "Failed to open input file " << filename << ": "
               << strerror(errno);
            push_error(sb.str());
            return;
        }
        cqasm_versionset_in(fptr, (yyscan_t)scanner);
    } else {
        buf = cqasm_version_scan_string(data.c_str(), (yyscan_t)scanner);
    }

    // Do the actual parsing.
    parse();

}

/**
 * Construct the analyzer internals for the given filename, and analyze
 * the file.
 */
ParseHelper::ParseHelper(
    const std::string &filename,
    FILE *fptr
) : filename(filename), version("") {

    // Create the scanner.
    if (!construct()) return;

    // Open the file or pass the data buffer to flex.
    cqasm_versionset_in(fptr, (yyscan_t)scanner);

    // Do the actual parsing.
    parse();

}

/**
 * Initializes the scanner. Returns whether this was successful.
 */
bool ParseHelper::construct() {
    int retcode = cqasm_versionlex_init((yyscan_t*)&scanner);
    if (retcode) {
        std::ostringstream sb;
        sb << "Failed to construct scanner: " << strerror(retcode);
        push_error(sb.str());
        return false;
    } else {
        return true;
    }
}

/**
 * Does the actual parsing.
 */
void ParseHelper::parse() {
    int retcode = cqasm_versionparse((yyscan_t) scanner, *this);
    if (retcode == 2) {
        std::ostringstream sb;
        sb << "Out of memory while parsing " << filename;
        push_error(sb.str());
        return;
    } else if (retcode) {
        std::ostringstream sb;
        sb << "Failed to parse " << filename;
        push_error(sb.str());
        return;
    }
    if (version.empty()) {
        throw error::AnalysisError(
            "Internal error; no version info nor error info was returned "
            "by version parser");
    }
}

/**
 * Destroys the analyzer.
 */
ParseHelper::~ParseHelper() {
    if (fptr) {
        fclose(fptr);
    }
    if (buf) {
        cqasm_version_delete_buffer((YY_BUFFER_STATE)buf, (yyscan_t)scanner);
    }
    if (scanner) {
        cqasm_versionlex_destroy((yyscan_t)scanner);
    }
}

/**
 * Pushes an error.
 */
void ParseHelper::push_error(const std::string &error) {
    throw error::AnalysisError(std::string(error));
}

} // namespace version
} // namespace cqasm
