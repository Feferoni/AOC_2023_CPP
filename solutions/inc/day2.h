#pragma once

#include <string>
#include <memory>

class Day2 {
public:
    virtual ~Day2() = default;
    [[nodiscard]] static std::unique_ptr<Day2> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
