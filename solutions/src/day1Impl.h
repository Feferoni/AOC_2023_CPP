#pragma once

#include <string>

#include "../inc/day1.h"

class Day1Impl : public Day1 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
