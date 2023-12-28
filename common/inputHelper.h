#pragma once

#include <source_location>
#include <string>
#include <vector>

#include <cxxabi.h>

#include "errorMsg.h" // IWYU pragma: keep
#include "helperTemplates.h"

namespace helper::input {
auto getFilePath(const std::source_location& location, const std::string& day) -> std::string;

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string>;

template <class Day>
auto getInput(const std::source_location location) -> std::vector<std::string> {
    const auto filePath = getFilePath(location, demangle<Day>());
    return getInputFromFile(filePath);
}
}  // namespace helper::file
