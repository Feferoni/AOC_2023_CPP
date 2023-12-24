#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <cxxabi.h>

#include "helperFunctions.h"

namespace {
constexpr char
    functionNamePatternString[] = "static std::string Day\\d+::(.*)\\(\\)";

// could fetch the class name and function here, but to lazy to fix
auto extractDayFromfunctionName(const std::string& functionSignature) -> std::string {
    std::smatch matches;
    if (std::regex_search(functionSignature, matches, std::regex(functionNamePatternString)) &&
        !matches.empty()) {
        return matches[1];
    }

    std::cerr << "Could not extract a day from the function: " << functionSignature << " with regex: " << functionNamePatternString << '\n';
    std::abort();
}
}  // namespace

auto getFilePath(const std::source_location& location, const std::string& day) -> std::string {
    const auto part = extractDayFromfunctionName(location.function_name());
    const auto currentPath = std::string(std::filesystem::current_path());

    if (IsTest::isTest) {
        return currentPath + "/../input/" + day + "_" + part + "_example.txt";
    } else {
        return currentPath + "/../input/" + day + ".txt";
    }
}

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string> {
    std::vector<std::string> lines;
    std::ifstream            file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filePath << '\n';
        std::abort();
    }

    return lines;
}

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

namespace Interval {
auto operator<<(std::ostream& os, const FarmingType ft) -> std::ostream& {
    switch (ft) {
    case FarmingType::SEED: os << "SEED"; break;
    case FarmingType::SOIL: os << "SOIL"; break;
    case FarmingType::FERTILIZER: os << "FERTILIZER"; break;
    case FarmingType::WATER: os << "WATER"; break;
    case FarmingType::LIGHT: os << "LIGHT"; break;
    case FarmingType::TEMPERATURE: os << "TEMPERATURE"; break;
    case FarmingType::HUMIDITY: os << "HUMIDITY"; break;
    case FarmingType::LOCATION: os << "LOCATION"; break;
    default:
        std::cerr << "Faulty farmingType: " << static_cast<uint32_t>(ft) << '\n';
        std::abort();
    }

    return os;
}

[[nodiscard]] auto Range::isAdjecentTo(const Range& other) const -> bool {
    return (max + 1 == other.min) || (min == other.max + 1);
}

[[nodiscard]] auto Range::hasOverlap(const Range& other) const -> bool {
    const bool thisRightBound  = min <= other.min && other.min <= max;
    const bool thisLeftbound   = min <= other.max && other.max <= max;
    const bool otherRightBound = other.min <= min && min <= other.max;
    const bool otherLeftBound  = other.min <= max && max <= other.max;
    return thisRightBound || thisLeftbound || otherRightBound || otherLeftBound;
}

[[nodiscard]] auto Range::isSuperSetTo(const Range& other) const -> bool {
    return min <= other.min && other.min <= max && min <= other.max && other.max <= max;
}

[[nodiscard]] auto Range::isSubSetOf(const Range& other) const -> bool {
    return other.min <= min && min <= other.max && other.min <= max &&
           max <= other.max;
}

[[nodiscard]] auto Range::rightIntersctionWith(const Range& other) const -> bool {
    return min < other.min && other.min <= max && min < other.max && max < other.max;
}

[[nodiscard]] auto Range::leftIntersctionWith(const Range& other) const -> bool {
    return other.min < min && min <= other.max && other.min < max && other.max < max;
}

[[nodiscard]] auto Range::isValueInRange(const uint32_t value) const -> bool {
    return min <= value && value <= max;
}

[[nodiscard]] auto Range::mergeRanges(const Range& other) const -> std::optional<Range> {
    if (hasOverlap(other) || isAdjecentTo(other)) {
        return Range{std::min(min, other.min), std::max(max, other.max)};
    }

    return std::nullopt;
}

[[nodiscard]] auto Range::operator==(const Range& other) const -> bool {
    return min == other.min && max == other.max;
}

[[nodiscard]] auto Range::operator!=(const Range& other) const -> bool {
    return !(*this == other);
}

[[nodiscard]] auto Range::operator<(const Range& other) const -> bool {
    return min < other.min;
}

[[nodiscard]] auto Range::operator>(const Range& other) const -> bool {
    return max > other.max;
}

[[nodiscard]] auto ConversionRange::getConvertValue(const uint32_t value) const -> std::optional<uint32_t> {
    if (from.isValueInRange(value)) {
        const auto diff = value - from.min;
        return to.min + diff;
    }

    return std::nullopt;
}

[[nodiscard]] auto ConversionRange::getConvertedRange(const Range& other) const -> std::optional<Range> {
    if (!from.hasOverlap(other)) {
        return std::nullopt;
    }

    const auto min = std::max(from.min, other.min);
    const auto max = std::min(from.max, other.max);

    const auto minDiff = min - from.min;
    const auto maxDiff = from.max - max;

    return Range{to.min + minDiff, to.max - maxDiff};
}

[[nodiscard]] auto ConversionRange::operator<(const ConversionRange& cr) const -> bool {
    return from < cr.from;
}

[[nodiscard]] auto mergeAdjecentRanges(std::vector<Range>& ranges) -> std::vector<Range> {
    std::sort(ranges.begin(), ranges.end());
    std::vector<Range> mergedRanges;

    for (uint32_t i = 0; i < ranges.size(); i++) {
        const auto& currentRange = ranges.at(i);
        if (!mergedRanges.empty()) {
            const auto newRange = currentRange.mergeRanges(mergedRanges.back());
            if (newRange.has_value()) {
                mergedRanges.back() = newRange.value();
                continue;
            }
        }
        if (i + 1 < ranges.size()) {
            const auto& nextRange = ranges.at(i + 1);
            const auto  newRange  = currentRange.mergeRanges(nextRange);
            if (newRange.has_value()) {
                mergedRanges.push_back(newRange.value());
                i++;
            } else {
                mergedRanges.push_back(currentRange);
            }
        } else {
            mergedRanges.push_back(currentRange);
        }
    }

    return mergedRanges;
}

template <typename T>
[[nodiscard]] auto getIndex(const std::vector<T>& vec, const uint32_t idx) -> std::optional<T> {
    return (idx < vec.size()) ? std::make_optional(vec.at(idx)) : std::nullopt;
}

[[nodiscard]] auto getConvertedRanges(std::vector<Range> fromNumberRanges, const ConversionRanges& conversionRanges) -> std::vector<Range> {
    std::vector<Range> convertedRanges;
    uint32_t           numberIdx     = 0;
    uint32_t           conversionIdx = 0;

    while (numberIdx < fromNumberRanges.size()) {
        const auto fromNumberRange = getIndex(fromNumberRanges, numberIdx);
        const auto conversionRange = getIndex(conversionRanges, conversionIdx);

        if (fromNumberRange.has_value() && !conversionRange.has_value()) {
            // just plain conversion left
            convertedRanges.push_back(fromNumberRange.value());
            numberIdx++;
            continue;
        }

        const auto convertedRange = conversionRange->getConvertedRange(
            fromNumberRange.value());
        if (!convertedRange.has_value()) {
            if (fromNumberRange->max >= conversionRange->from.max) {
                conversionIdx++;
            } else {
                convertedRanges.push_back(fromNumberRange.value());
                numberIdx++;
            }
            continue;
        }

        if (fromNumberRange.value() == conversionRange->from) {
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
            conversionIdx++;
        } else if (fromNumberRange->isSubSetOf(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
            if (fromNumberRange->max == conversionRange->from.max) {
                conversionIdx++;
            }
        } else if (fromNumberRange->isSuperSetTo(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            convertedRanges.push_back(Range{fromNumberRange->min, conversionRange->from.min - 1});
            fromNumberRanges.at(numberIdx) = {conversionRange->from.max + 1, fromNumberRange.value().max};
            conversionIdx++;
            if (fromNumberRange->max == conversionRange->from.max) {
                numberIdx++;
            }
        } else if (fromNumberRange->rightIntersctionWith(conversionRange->from)) {
            convertedRanges.push_back(Range{fromNumberRange->min, conversionRange->from.min - 1});
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
        } else if (fromNumberRange->leftIntersctionWith(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            fromNumberRanges.at(numberIdx) = {conversionRange->from.max + 1, fromNumberRange.value().max};
            conversionIdx++;
        } else {
            std::cerr << "Should never arrive here. \n";
            std::abort();
        }
    }

    return mergeAdjecentRanges(convertedRanges);
}

}  // namespace Interval
