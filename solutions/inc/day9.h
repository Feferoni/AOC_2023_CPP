#pragma once

#include <string>
#include <memory>

class Day9
{
public:
    virtual ~Day9() = default;
    [[nodiscard]] static std::unique_ptr<Day9> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
