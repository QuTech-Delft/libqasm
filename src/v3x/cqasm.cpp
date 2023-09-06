/**
 * \dir
 * Contains the non-generated source files and private header files for libqasm.
 *
 * \file
 * Implementation for \ref include/v3x/cqasm.hpp "v3x/cqasm.hpp".
 */

#include "v3x/cqasm.hpp"

#include <stdexcept>  // runtime_error


namespace cqasm {
namespace v3x {

/**
 * Parses and analyzes the given file with the default analyzer, dumping error
 * messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 */
void analyze(
    const std::string & /* filename */,
    const std::string & /* api_version */
) {
    throw std::runtime_error("Unimplemented");
}

/**
 * Parses and analyzes the given file pointer with the default analyzer, dumping
 * error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional filename is only used for error messages.
 */
void analyze(
    FILE * /* file */,
    const std::string & /* filename */,
    const std::string & /* api_version */
) {
    throw std::runtime_error("Unimplemented");
}

/**
 * Parses and analyzes the given string with the default analyzer, dumping
 * error messages to stderr and throwing an analyzer::AnalysisFailed on failure.
 * The optional filename is only used for error messages.
 */
void analyze_string(
    const std::string & /* data */,
    const std::string & /* filename */,
    const std::string & /* api_version */
) {
    throw std::runtime_error("Unimplemented");
}

} // namespace v3x
} // namespace cqasm
