#pragma once

#include <iostream>
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
