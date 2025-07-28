#pragma once

namespace cqasm {

static const char* version{ "1.2.1" };
static const char* release_year{ "2025" };

[[nodiscard]] [[maybe_unused]] static const char* get_version() {
    return version;
}

[[nodiscard]] [[maybe_unused]] static const char* get_release_year() {
    return release_year;
}

}  // namespace cqasm
