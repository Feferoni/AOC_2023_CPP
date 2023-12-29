#pragma once

#include <string>
#include <memory>

class Day3 {
public:
    virtual ~Day3() = default;
    [[nodiscard]] static std::unique_ptr<Day3> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
