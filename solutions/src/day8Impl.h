#pragma once

#include <string>

#include "../inc/day8.h"

class Day8Impl : public Day8 {
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
