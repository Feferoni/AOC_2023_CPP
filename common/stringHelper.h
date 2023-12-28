#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include "errorMsg.h"

namespace helper::string {
[[nodiscard]] auto splitString(const std::string& str, const std::string& delimiter) -> std::vector<std::string>;
[[nodiscard]] auto splitStrToStrViews(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view>;

template <typename T>
concept Numeric = std::is_arithmetic<T>::value;

template <Numeric T>
[[nodiscard]] auto getNumbersFromString(const std::string& str) -> std::vector<T> {
    const auto splitNumbers = splitString(str, " ");

    std::vector<T> numbers;
    std::ranges::for_each(splitNumbers, [&](const std::string& str) {
        if (!str.empty()) {
            try {
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>) {
                    numbers.push_back(std::stoi(str));
                } else if constexpr (std::is_same_v<T, long>) {
                    numbers.push_back(std::stol(str));
                } else if constexpr (std::is_same_v<T, unsigned long> || std::is_same_v<T, uint32_t>) {
                    numbers.push_back(std::stoul(str));
                } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, int64_t>) {
                    numbers.push_back(std::stoll(str));
                } else if constexpr (std::is_same_v<T, unsigned long long> || std::is_same_v<T, uint64_t>) {
                    numbers.push_back(std::stoull(str));
                } else if constexpr (std::is_same_v<T, float>) {
                    numbers.push_back(std::stof(str));
                } else if constexpr (std::is_same_v<T, double>) {
                    numbers.push_back(std::stod(str));
                } else if constexpr (std::is_same_v<T, long double>) {
                    numbers.push_back(std::stold(str));
                } else {
                    ERROR_MSG_AND_EXIT("Unsupported numeric type.");
                }
            } catch (const std::invalid_argument& ex) {
                ERROR_MSG_AND_EXIT(ex.what() << " - " << str);
            } catch (const std::out_of_range& ex) {
                ERROR_MSG_AND_EXIT(ex.what() << " - " << str);
            }
        }
    });
    return numbers;
}
}  // namespace helper::string
