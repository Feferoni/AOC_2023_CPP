#pragma once

#include <string>

#include "../inc/day2.h"

class Day2Impl : public Day2 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
