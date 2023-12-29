#pragma once

#include <string>
#include <memory>

class Day6 {
public:
    virtual ~Day6() = default;
    [[nodiscard]] static std::unique_ptr<Day6> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
