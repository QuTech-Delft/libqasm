#include "utils.hpp"

#include <fstream>  // ifstream, ofstream
#include <iterator>  // istreambuf_iterator

namespace cqasm::test {

namespace fs = std::filesystem;

/**
 * Reads the given file into the given string buffer and
 * returns true if it exists,
 * otherwise do nothing with the buffer and return false.
 */
bool read_file(const fs::path& file_path, std::string& output) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        return false;
    }
    output.clear();
    ifs.seekg(0, std::ios::end);
    output.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    output.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    return true;
}

/**
 * Overwrites or creates the given file with the given string.
 */
void write_file(const fs::path& file_path, const std::string& input) {
    std::ofstream stream(file_path, std::ios::binary | std::ios::out);  // always write LF, i.e., avoid CR+LF in Windows
    stream << input;
}

}  // namespace cqasm::test
