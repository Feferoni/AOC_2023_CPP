#pragma once

#include <string>
#include <memory>

class Day4 {
public:
    virtual ~Day4() = default;
    [[nodiscard]] static std::unique_ptr<Day4> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
