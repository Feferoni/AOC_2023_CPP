#pragma once

#include <string>
#include <memory>

class Day8 {
public:
    virtual ~Day8() = default;
    [[nodiscard]] static std::unique_ptr<Day8> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
