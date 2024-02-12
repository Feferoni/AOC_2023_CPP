#pragma once

#include <concepts>  // IWYU pragma: keep
#include <string>
#include <memory>

template <class Day>
concept Solution = requires(Day day) {
    { Day::getInstance() } -> std::same_as<std::unique_ptr<Day>>;
    { Day::getInstance()->part1() } -> std::same_as<std::string>;
    { Day::getInstance()->part2() } -> std::same_as<std::string>;
    { day.part1() } -> std::same_as<std::string>;
    { day.part2() } -> std::same_as<std::string>;
};

