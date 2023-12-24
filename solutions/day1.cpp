#include <cctype>
#include <ranges>

#include "day1.h"

#include "fileHelper.h"

namespace {
constexpr std::array<std::pair<std::string_view, std::string_view>, 15> conversionTable = {std::pair{"eightwo", "82"},   std::pair{"eighthree", "83"},
                                                                                           std::pair{"oneight", "18"},   std::pair{"fiveight", "58"},
                                                                                           std::pair{"threeight", "38"}, std::pair{"twone", "21"},
                                                                                           std::pair{"one", "1"},        std::pair{"two", "2"},
                                                                                           std::pair{"three", "3"},      std::pair{"four", "4"},
                                                                                           std::pair{"five", "5"},       std::pair{"six", "6"},
                                                                                           std::pair{"seven", "7"},      std::pair{"eight", "8"},
                                                                                           std::pair{"nine", "9"}};

[[nodiscard]] auto getSumOfNumbers(const auto& input) -> std::string {
    std::uint32_t sum = 0;
    for (const auto& line : input) {
        auto isDigit = [](const char c) { return std::isdigit(c); };

        char        first  = *std::find_if(line.begin(), line.end(), isDigit);
        char        last   = *std::find_if(line.rbegin(), line.rend(), isDigit);
        std::string number = {first, last};
        sum += std::stoul(number);
    }

    return std::to_string(sum);
}

[[nodiscard]] auto replaceAlpahNumbers(const std::string& input) {
    std::string result = input;
    for (const auto& [searchFor, replaceWith] : conversionTable) {
        size_t startPos = 0;
        while ((startPos = result.find(searchFor, startPos)) != std::string::npos) {
            result.replace(startPos, searchFor.length(), replaceWith);
            startPos += replaceWith.length();
        }
    }
    return result;
}
}  // namespace

auto Day1::part1() -> std::string {
    const auto input = getInput<Day1>(std::source_location::current());
    return getSumOfNumbers(input);
};

auto Day1::part2() -> std::string {
    const auto input = getInput<Day1>(std::source_location::current());
    const auto convertedInput = input | std::views::transform(replaceAlpahNumbers);
    return getSumOfNumbers(convertedInput);
};
