#pragma once

#include <string>
#include <memory>

class Day1 {
public:
    virtual ~Day1() = default;
    [[nodiscard]] static std::unique_ptr<Day1> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
