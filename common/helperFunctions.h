#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <source_location>
#include <string>
#include <vector>

#include <cxxabi.h>

#include "helperFunctionTemplates.h"

// TODO: do this in a better way if you ever get time.
struct IsTest {
    static bool isTest;
};

struct Position2D {
    int32_t x;
    int32_t y;

    [[nodiscard]] inline auto isWithinBounds(const Position2D& lower, const Position2D& upper) -> bool {
        bool isXwithinBound = lower.x <= x && x <= upper.x;
        bool isYwithinBound = lower.y <= y && y <= upper.y;
        return isXwithinBound && isYwithinBound;
    }

    Position2D operator+(const Position2D& other) const {
        return {x + other.x, y + other.y};
    }

    friend std::ostream& operator<<(std::ostream& os, const Position2D& p) {
        os << "x: " << std::to_string(p.x) << " y: " << std::to_string(p.y);
        return os;
    }
};

namespace Interval {
struct Range {
    uint32_t min;
    uint32_t max;

    [[nodiscard]] auto isSubSetOf(const Range& other) const -> bool;
    [[nodiscard]] auto isSuperSetTo(const Range& other) const -> bool;
    [[nodiscard]] auto rightIntersctionWith(const Range& other) const -> bool;
    [[nodiscard]] auto leftIntersctionWith(const Range& other) const -> bool;
    [[nodiscard]] auto isAdjecentTo(const Range& other) const -> bool;
    [[nodiscard]] auto hasOverlap(const Range& other) const -> bool;
    [[nodiscard]] auto isValueInRange(const uint32_t value) const -> bool;
    [[nodiscard]] auto mergeRanges(const Range& other) const -> std::optional<Range>;
    [[nodiscard]] auto operator==(const Range& other) const -> bool;
    [[nodiscard]] auto operator!=(const Range& other) const -> bool;
    [[nodiscard]] auto operator<(const Range& other) const -> bool;
    [[nodiscard]] auto operator>(const Range& other) const -> bool;
    friend inline auto operator<<(std::ostream& os, const Range& r) -> std::ostream& {
        os << r.min << "->" << r.max;
        return os;
    }
};

struct ConversionRange {
    Range from;
    Range to;

    [[nodiscard]] auto getConvertedRange(const Range& other) const -> std::optional<Range>;
    [[nodiscard]] auto getConvertValue(const uint32_t value) const -> std::optional<uint32_t>;
    [[nodiscard]] auto operator<(const ConversionRange& cr) const -> bool;
    friend inline auto operator<<(std::ostream& os, const ConversionRange& cr) -> std::ostream& {
        os << cr.from << " to " << cr.to;
        return os;
    }
};

enum class FarmingType : uint32_t {
    SEED,
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION
};
auto operator<<(std::ostream& os, const Interval::FarmingType ft) -> std::ostream&;

using ConversionType   = std::pair<FarmingType, FarmingType>;
using ConversionRanges = std::vector<ConversionRange>;
using ConversionMap    = std::map<ConversionType, ConversionRanges>;

[[nodiscard]] auto mergeAdjecentRanges(std::vector<Range>& ranges) -> std::vector<Range>;
[[nodiscard]] auto getConvertedRanges(std::vector<Range> fromNumberRanges, const ConversionRanges& conversionRanges) -> std::vector<Range>;

}  // namespace Interval

auto getFilePath(const std::source_location& location, const std::string& day) -> std::string;

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string>;

template <class Day>
auto getInput(const std::source_location location) -> std::vector<std::string> {
    const auto filePath = getFilePath(location, demangle<Day>());
    return getInputFromFile(filePath);
}

[[nodiscard]] auto splitString(const std::string& str, const std::string& delimiter) -> std::vector<std::string>;

[[nodiscard]] auto splitStrToStrViews(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view>;

[[nodiscard]] auto getNumbersFromString(const std::string& str) -> std::vector<std::uint32_t>;
