#include <intervalHelper.h>

namespace helper::interval {
[[nodiscard]] auto Range::isAdjecentTo(const Range& other) const -> bool {
    return (max + 1 == other.min) || (min == other.max + 1);
}

[[nodiscard]] auto Range::hasOverlap(const Range& other) const -> bool {
    const bool thisRightBound  = min <= other.min && other.min <= max;
    const bool thisLeftbound   = min <= other.max && other.max <= max;
    const bool otherRightBound = other.min <= min && min <= other.max;
    const bool otherLeftBound  = other.min <= max && max <= other.max;
    return thisRightBound || thisLeftbound || otherRightBound || otherLeftBound;
}

[[nodiscard]] auto Range::isSuperSetTo(const Range& other) const -> bool {
    return min <= other.min && other.min <= max && min <= other.max && other.max <= max;
}

[[nodiscard]] auto Range::isSubSetOf(const Range& other) const -> bool {
    return other.min <= min && min <= other.max && other.min <= max &&
           max <= other.max;
}

[[nodiscard]] auto Range::rightIntersctionWith(const Range& other) const -> bool {
    return min < other.min && other.min <= max && min < other.max && max < other.max;
}

[[nodiscard]] auto Range::leftIntersctionWith(const Range& other) const -> bool {
    return other.min < min && min <= other.max && other.min < max && other.max < max;
}

[[nodiscard]] auto Range::isValueInRange(const uint32_t value) const -> bool {
    return min <= value && value <= max;
}

[[nodiscard]] auto Range::mergeRanges(const Range& other) const -> std::optional<Range> {
    if (hasOverlap(other) || isAdjecentTo(other)) {
        return Range{std::min(min, other.min), std::max(max, other.max)};
    }

    return std::nullopt;
}

[[nodiscard]] auto Range::operator==(const Range& other) const -> bool {
    return min == other.min && max == other.max;
}

[[nodiscard]] auto Range::operator!=(const Range& other) const -> bool {
    return !(*this == other);
}

[[nodiscard]] auto Range::operator<(const Range& other) const -> bool {
    return min < other.min;
}

[[nodiscard]] auto Range::operator>(const Range& other) const -> bool {
    return max > other.max;
}

[[nodiscard]] auto ConversionRange::getConvertValue(const uint32_t value) const -> std::optional<uint32_t> {
    if (from.isValueInRange(value)) {
        const auto diff = value - from.min;
        return to.min + diff;
    }

    return std::nullopt;
}

[[nodiscard]] auto ConversionRange::getConvertedRange(const Range& other) const -> std::optional<Range> {
    if (!from.hasOverlap(other)) {
        return std::nullopt;
    }

    const auto min = std::max(from.min, other.min);
    const auto max = std::min(from.max, other.max);

    const auto minDiff = min - from.min;
    const auto maxDiff = from.max - max;

    return Range{to.min + minDiff, to.max - maxDiff};
}

[[nodiscard]] auto ConversionRange::operator<(const ConversionRange& cr) const -> bool {
    return from < cr.from;
}

[[nodiscard]] auto mergeAdjecentRanges(std::vector<Range>& ranges) -> std::vector<Range> {
    std::sort(ranges.begin(), ranges.end());
    std::vector<Range> mergedRanges;

    for (uint32_t i = 0; i < ranges.size(); i++) {
        const auto& currentRange = ranges.at(i);
        if (!mergedRanges.empty()) {
            const auto newRange = currentRange.mergeRanges(mergedRanges.back());
            if (newRange.has_value()) {
                mergedRanges.back() = newRange.value();
                continue;
            }
        }
        if (i + 1 < ranges.size()) {
            const auto& nextRange = ranges.at(i + 1);
            const auto  newRange  = currentRange.mergeRanges(nextRange);
            if (newRange.has_value()) {
                mergedRanges.push_back(newRange.value());
                i++;
            } else {
                mergedRanges.push_back(currentRange);
            }
        } else {
            mergedRanges.push_back(currentRange);
        }
    }

    return mergedRanges;
}

template <typename T>
[[nodiscard]] auto getOptionalIndex(const std::vector<T>& vec, const uint32_t idx) -> std::optional<T> {
    return (idx < vec.size()) ? std::make_optional(vec.at(idx)) : std::nullopt;
}

[[nodiscard]] auto getConvertedRanges(std::vector<Range> fromNumberRanges, const std::vector<ConversionRange>& conversionRanges) -> std::vector<Range> {
    std::vector<Range> convertedRanges;
    uint32_t           numberIdx     = 0;
    uint32_t           conversionIdx = 0;

    while (numberIdx < fromNumberRanges.size()) {
        const auto fromNumberRange = getOptionalIndex(fromNumberRanges, numberIdx);
        const auto conversionRange = getOptionalIndex(conversionRanges, conversionIdx);

        if (fromNumberRange.has_value() && !conversionRange.has_value()) {
            // just plain conversion left
            convertedRanges.push_back(fromNumberRange.value());
            numberIdx++;
            continue;
        }

        const auto convertedRange = conversionRange->getConvertedRange(
            fromNumberRange.value());
        if (!convertedRange.has_value()) {
            if (fromNumberRange->max >= conversionRange->from.max) {
                conversionIdx++;
            } else {
                convertedRanges.push_back(fromNumberRange.value());
                numberIdx++;
            }
            continue;
        }

        if (fromNumberRange.value() == conversionRange->from) {
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
            conversionIdx++;
        } else if (fromNumberRange->isSubSetOf(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
            if (fromNumberRange->max == conversionRange->from.max) {
                conversionIdx++;
            }
        } else if (fromNumberRange->isSuperSetTo(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            convertedRanges.push_back(Range{fromNumberRange->min, conversionRange->from.min - 1});
            fromNumberRanges.at(numberIdx) = {conversionRange->from.max + 1, fromNumberRange.value().max};
            conversionIdx++;
            if (fromNumberRange->max == conversionRange->from.max) {
                numberIdx++;
            }
        } else if (fromNumberRange->rightIntersctionWith(conversionRange->from)) {
            convertedRanges.push_back(Range{fromNumberRange->min, conversionRange->from.min - 1});
            convertedRanges.push_back(convertedRange.value());
            numberIdx++;
        } else if (fromNumberRange->leftIntersctionWith(conversionRange->from)) {
            convertedRanges.push_back(convertedRange.value());
            fromNumberRanges.at(numberIdx) = {conversionRange->from.max + 1, fromNumberRange.value().max};
            conversionIdx++;
        } else {
            std::cerr << "Should never arrive here. \n";
            std::cerr << "Idx: " << numberIdx << " conversionIdx: " << conversionIdx <<'\n';
            std::cerr << "FromNumberRange: " << fromNumberRange.value() <<'\n';
            std::cerr << "ConversionRange: " << convertedRange.value() << '\n';
            std::abort();
        }
    }

    return mergeAdjecentRanges(convertedRanges);
}

}  // namespace Interval

