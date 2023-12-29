#pragma once

#include <string>
#include <memory>

class Day7 {
public:
    virtual ~Day7() = default;
    [[nodiscard]] static std::unique_ptr<Day7> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
