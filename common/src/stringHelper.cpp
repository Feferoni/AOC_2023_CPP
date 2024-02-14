#include <iostream>
#include "../inc/stringHelper.h"

namespace helper::string {
[[nodiscard]] auto splitString(const std::string& str, const std::string& delimiter) -> std::vector<std::string> {
    std::vector<std::string> result;
    size_t                   start = 0;
    size_t                   end   = str.find(delimiter);

    while (end != std::string::npos) {
        auto token = std::string(str.data() + start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
        end   = str.find(delimiter, start);
    }

    result.push_back(std::string(str.data() + start, str.length() - start));
    return result;
}

[[nodiscard]] auto splitStrToStrViews(const std::string_view str, const std::string_view delimiter) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    size_t                        start = 0;
    size_t                        end   = str.find(delimiter);

    while (end != std::string::npos) {
        std::string_view token = std::string_view(str.data() + start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
        end   = str.find(delimiter, start);
    }

    result.push_back(std::string_view(str.data() + start, str.length() - start));

    return result;
}

[[nodiscard]] auto stripStrView(const std::string_view input_str_view) -> std::string_view {
    const auto first = input_str_view.find_first_not_of(" ");
    const auto last = input_str_view.find_last_not_of(" ");
    return std::string_view(input_str_view.data() + first, (last - first) + 1);
}
}  // namespace helper::string
