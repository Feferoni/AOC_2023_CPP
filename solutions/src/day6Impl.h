#pragma once

#include <string>

#include "../inc/day6.h"

class Day6Impl : public Day6 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
