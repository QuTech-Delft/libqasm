#include <filesystem>
#include <string>

namespace cqasm::test {

namespace fs = std::filesystem;

/**
 * Reads the given file into the given string buffer and
 * returns true if it exists,
 * otherwise do nothing with the buffer and return false.
 */
bool read_file(const fs::path& file_path, std::string& output);

/**
 * Overwrites or creates the given file with the given string.
 */
void write_file(const fs::path& file_path, const std::string& input);

}  // namespace cqasm::test
