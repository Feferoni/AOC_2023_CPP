#pragma once

#include <iostream>
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

struct Position {
    int32_t x;
    int32_t y;

    [[nodiscard]] inline auto isWithinBounds(const Position& lower, const Position& upper) -> bool {
        bool isXwithinBound = lower.x <= x && x <= upper.x;
        bool isYwithinBound = lower.y <= y && y <= upper.y;
        return isXwithinBound && isYwithinBound;
    }

    Position operator+(const Position& other) const {
        return {x + other.x, y + other.y};
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& p) {
        os << "x: " << std::to_string(p.x) << " y: " << std::to_string(p.y);
        return os;
    }
};

struct Range {
    uint32_t min;
    uint32_t max;

    [[nodiscard]] inline auto hasOverlap(const Range& other) const -> bool {
        const bool thisRightBound = min <= other.min && other.min <= max;
        const bool thisLeftbound = min <= other.max && other.max <= max;
        const bool otherRightBound = other.min <= min && min <= other.max;
        const bool otherLeftBound = other.min <= max && max <= other.max;
        return thisRightBound || thisLeftbound || otherRightBound || otherLeftBound;
    }

    [[nodiscard]] inline auto isValueInRange(const uint32_t value) const -> bool {
        return min <= value && value <= max;
    }

    [[nodiscard]] inline auto mergeRanges(const Range& other) const -> std::optional<Range> {
        if (hasOverlap(other)) {
            return Range{std::min(min, other.min), std::max(max, other.max)};
        }

        return std::nullopt;
    }

    [[nodiscard]] inline auto operator==(const Range& other) const -> bool {
        return min == other.min && max == other.max;
    }

    [[nodiscard]] inline auto operator!=(const Range& other) const -> bool {
        return !(*this == other);
    }

    [[nodiscard]] inline auto operator<(const Range& other) const -> bool {
        return min < other.min;
    }

    [[nodiscard]] inline auto operator>(const Range& other) const -> bool {
        return max > other.max;
    }

    friend auto operator<<(std::ostream& os, const Range& r) -> std::ostream& {
        os << r.min << "->" << r.max;
        return os;
    }
};

struct ConversionRange {
    Range from;
    Range to;

    [[nodiscard]] inline auto convertValue(const uint32_t value) const -> std::optional<uint32_t> {
        if (from.isValueInRange(value)) {
            const auto diff = value - from.min;
            return to.min + diff;
        }

        return std::nullopt;
    }

    [[nodiscard]] inline auto operator<(const ConversionRange& cr) const -> bool {
        return from < cr.from;
    }

    friend auto operator<<(std::ostream& os, const ConversionRange& cr) -> std::ostream& {
        os << cr.from << " to " << cr.to;
        return os;
    }
};

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
