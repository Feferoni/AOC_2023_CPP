#include <algorithm>
#include <iostream>

#include "stringHelper.h"

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

[[nodiscard]] auto splitStrToStrViews(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view> {
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

[[nodiscard]] auto getNumbersFromString(const std::string& str) -> std::vector<std::uint32_t> {
    const auto                 splitNumbers = splitString(str, " ");
    std::vector<std::uint32_t> numbers;
    std::ranges::for_each(splitNumbers, [&numbers](const std::string& str) {
        try {
            numbers.push_back(std::stoul(str));
        } catch (const std::invalid_argument& ex) {
            std::cerr << ex.what() << " - " << str << '\n';
            std::abort();
        } catch (const std::out_of_range& ex) {
            std::cerr << ex.what() << " - " << str << '\n';
            std::abort();
        }
    });
    return numbers;
}

}  // namespace helper::string
