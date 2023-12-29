#pragma once

#include <string>

#include "../inc/day4.h"

class Day4Impl : public Day4 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
