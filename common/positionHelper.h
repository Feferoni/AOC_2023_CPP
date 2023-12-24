#pragma once

#include <cstdint>
#include <ostream>

namespace helper::position {
struct Position2D {
    int32_t x;
    int32_t y;

    [[nodiscard]] auto isWithinBounds(const Position2D& lower, const Position2D& upper) const -> bool;
    Position2D           operator+(const Position2D& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Position2D& p);
};
}  // namespace helper::position
