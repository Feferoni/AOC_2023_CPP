#include <string>

#include "positionHelper.h"

namespace helper::position {
    [[nodiscard]] auto Position2D::isWithinBounds(const Position2D& lower, const Position2D& upper) const -> bool {
        bool isXwithinBound = lower.x <= x && x <= upper.x;
        bool isYwithinBound = lower.y <= y && y <= upper.y;
        return isXwithinBound && isYwithinBound;
    }

    Position2D Position2D::operator+(const Position2D& other) const {
        return {x + other.x, y + other.y};
    }

    std::ostream& operator<<(std::ostream& os, const Position2D& p) {
        os << "x: " << std::to_string(p.x) << " y: " << std::to_string(p.y);
        return os;
    }

}
