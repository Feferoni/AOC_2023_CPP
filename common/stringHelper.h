#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace helper::string {
[[nodiscard]] auto splitString(const std::string& str, const std::string& delimiter) -> std::vector<std::string>;
[[nodiscard]] auto splitStrToStrViews(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view>;
[[nodiscard]] auto getNumbersFromString(const std::string& str) -> std::vector<std::uint32_t>;
}  // namespace helper::string
