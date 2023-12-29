#pragma once

#include <string>

#include "../inc/day5.h"

class Day5Impl : public Day5 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
