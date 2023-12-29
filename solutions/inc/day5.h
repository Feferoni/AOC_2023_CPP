#pragma once

#include <string>
#include <memory>

class Day5 {
public:
    virtual ~Day5() = default;
    [[nodiscard]] static std::unique_ptr<Day5> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
