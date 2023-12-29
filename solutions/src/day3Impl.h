#pragma once

#include <string>

#include "../inc/day3.h"

class Day3Impl : public Day3 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
