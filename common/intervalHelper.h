#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

namespace helper::interval {
struct Range {
    uint32_t min;
    uint32_t max;

    [[nodiscard]] auto isSubSetOf(const Range& other) const -> bool;
    [[nodiscard]] auto isSuperSetTo(const Range& other) const -> bool;
    [[nodiscard]] auto rightIntersctionWith(const Range& other) const -> bool;
    [[nodiscard]] auto leftIntersctionWith(const Range& other) const -> bool;
    [[nodiscard]] auto isAdjecentTo(const Range& other) const -> bool;
    [[nodiscard]] auto hasOverlap(const Range& other) const -> bool;
    [[nodiscard]] auto isValueInRange(const uint32_t value) const -> bool;
    [[nodiscard]] auto mergeRanges(const Range& other) const -> std::optional<Range>;
    [[nodiscard]] auto operator==(const Range& other) const -> bool;
    [[nodiscard]] auto operator!=(const Range& other) const -> bool;
    [[nodiscard]] auto operator<(const Range& other) const -> bool;
    [[nodiscard]] auto operator>(const Range& other) const -> bool;
    friend inline auto operator<<(std::ostream& os, const Range& r) -> std::ostream& {
        os << r.min << "->" << r.max;
        return os;
    }
};

struct ConversionRange {
    Range from;
    Range to;

    [[nodiscard]] auto getConvertedRange(const Range& other) const -> std::optional<Range>;
    [[nodiscard]] auto getConvertValue(const uint32_t value) const -> std::optional<uint32_t>;
    [[nodiscard]] auto operator<(const ConversionRange& cr) const -> bool;
    friend inline auto operator<<(std::ostream& os, const ConversionRange& cr) -> std::ostream& {
        os << cr.from << " to " << cr.to;
        return os;
    }
};

[[nodiscard]] auto mergeAdjecentRanges(std::vector<Range>& ranges) -> std::vector<Range>;
[[nodiscard]] auto getConvertedRanges(std::vector<Range> fromNumberRanges, const std::vector<ConversionRange>& conversionRanges) -> std::vector<Range>;

}  // namespace helper::interval
