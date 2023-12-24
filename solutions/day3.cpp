#include <algorithm>
#include <optional>

#include "day3.h"

#include "fileHelper.h"
#include "positionHelper.h"

namespace {
using namespace helper::position;
// clang-format off
constexpr std::array<Position2D, 8> DIRECTIONS = {
    Position2D{ 1,  0},  // right
    Position2D{-1,  0},  // left
    Position2D{ 0,  1},  // up
    Position2D{ 0, -1},  // down
    Position2D{ 1,  1},  // up; right
    Position2D{-1,  1},  // up, left
    Position2D{ 1, -1},  // down; right
    Position2D{-1, -1}
}; // down, left

constexpr std::array<std::pair<uint32_t, uint32_t>, 3> VALID_SYMBOL_RANGES = {
    std::pair<uint32_t, uint32_t>{33u, 45u},
    std::pair<uint32_t, uint32_t>{47u, 47u},
    std::pair<uint32_t, uint32_t>{58u, 64u}
};
// clang-format on

struct NumberData {
    Position2D startPosition;
    Position2D endPosition;
    uint32_t   number;

    [[maybe_unused]] friend std::ostream& operator<<(std::ostream& os, const NumberData& nd) {
        os << "NumberData startPos: " << nd.startPosition << " endPos: " << nd.endPosition << " Number: " << nd.number;
        return os;
    }
};

[[nodiscard]] auto getUpperLimitsPosition(const std::vector<std::string>& input) -> Position2D {
    return {static_cast<int32_t>(input.front().size() - 1),
            static_cast<int32_t>(input.size() - 1)};
}

constexpr char GEAR_SYMBOL    = '*';
constexpr char INVALID_SYMBOL = '.';

[[nodiscard]] auto getNewPosition(const Position2D& currentPosition, const Position2D& direction, const Position2D& positionLimit) -> std::optional<Position2D> {
    Position2D newPosition = currentPosition + direction;
    if (newPosition.isWithinBounds({0, 0}, positionLimit)) {
        return newPosition;
    }
    return std::nullopt;
}

[[nodiscard]] auto isCharValidSymbol(const char c) -> bool {
    if (c == INVALID_SYMBOL) {
        return false;
    }

    return std::ranges::any_of(VALID_SYMBOL_RANGES, [&c](const std::pair<uint32_t, uint32_t>& minMax) {
        return minMax.first <= static_cast<uint32_t>(c) &&
               static_cast<uint32_t>(c) <= minMax.second;
    });
}

[[nodiscard]] auto getCharFromInput(const std::vector<std::string>& input, const Position2D& position, const Position2D& positionLimit, const std::source_location& caller) -> char {
    try {
        return input.at(static_cast<uint32_t>(position.y))
            .at(static_cast<uint32_t>(position.x));
    } catch (const std::out_of_range& ex) {
        std::cerr << ex.what() << ": " << caller.function_name() << " current position: " << positionLimit << " limit: " << positionLimit << '\n';
        std::abort();
    }
}

[[nodiscard]] auto isPositionAdjecentToSymbol(const std::vector<std::string>& input, const Position2D& position, const Position2D& positionLimit) -> bool {
    for (const auto& direction : DIRECTIONS) {
        const auto adjecentPosition = getNewPosition(position, direction, positionLimit);
        if (!adjecentPosition.has_value()) {
            continue;
        }
        if (isCharValidSymbol(getCharFromInput(input, adjecentPosition.value(), positionLimit, std::source_location::current()))) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] auto addIfNumberIsValid(std::string& number, bool& adjacentToValidSymbol) -> uint32_t {
    uint32_t numberSum = 0;
    if (!number.empty() && adjacentToValidSymbol) {
        numberSum = std::stoul(number);
    }
    number.clear();
    adjacentToValidSymbol = false;
    return numberSum;
}

[[nodiscard]] auto sumOfPartNumbers(const std::vector<std::string>& input) -> std::string {
    const Position2D positionLimit = getUpperLimitsPosition(input);

    uint32_t    sum                      = 0;
    bool        foundAdjecentValidSymbol = false;
    std::string currentNumber            = "";
    for (int32_t y = 0; y <= positionLimit.y; y++) {
        for (int32_t x = 0; x <= positionLimit.x; x++) {
            const Position2D currentPosition{x, y};

            const char currentChar = getCharFromInput(input, currentPosition, positionLimit, std::source_location::current());
            if (std::isdigit(currentChar)) {
                currentNumber.push_back(currentChar);
                if (isPositionAdjecentToSymbol(input, currentPosition, positionLimit)) {
                    foundAdjecentValidSymbol = true;
                }
            } else {
                sum += addIfNumberIsValid(currentNumber, foundAdjecentValidSymbol);
            }
        }
        sum += addIfNumberIsValid(currentNumber, foundAdjecentValidSymbol);
    }
    return std::to_string(sum);
}

[[nodiscard]] auto getPositionsOfGears(const std::vector<std::string>& input) -> std::vector<Position2D> {
    const Position2D        positionLimit = getUpperLimitsPosition(input);
    std::vector<Position2D> gearPositions;

    for (int32_t y = 0; y <= positionLimit.y; y++) {
        for (int32_t x = 0; x <= positionLimit.x; x++) {
            const Position2D position{static_cast<int32_t>(x), static_cast<int32_t>(y)};
            const auto currentChar = getCharFromInput(input, position, positionLimit, std::source_location::current());
            if (currentChar == GEAR_SYMBOL) {
                gearPositions.push_back(position);
            }
        }
    }

    return gearPositions;
}

[[nodiscard]] auto getNumberDatas(const std::vector<std::string>& input) -> std::vector<NumberData> {
    const Position2D        positionLimit = getUpperLimitsPosition(input);
    std::vector<NumberData> numberDatas;

    std::optional<NumberData> currentNumberData = std::nullopt;
    for (int32_t y = 0; y <= positionLimit.y; y++) {
        const auto checkNumberDataEnd = [&currentNumberData, &input, &numberDatas]() {
            if (currentNumberData.has_value()) {
                const auto& currentLine = input.at(
                    currentNumberData->startPosition.y);
                const auto& startPos = currentNumberData->startPosition.x;
                const auto& length   = currentNumberData->endPosition.x -
                                     currentNumberData->startPosition.x + 1;
                currentNumberData->number = std::stoul(currentLine.substr(startPos, length));
                numberDatas.push_back(currentNumberData.value());
                currentNumberData = std::nullopt;
            }
        };

        for (int32_t x = 0; x <= positionLimit.x; x++) {
            const Position2D position{x, y};
            const auto currentChar = getCharFromInput(input, position, positionLimit, std::source_location::current());
            if (std::isdigit(currentChar)) {
                if (currentNumberData.has_value()) {
                    currentNumberData->endPosition.x = x;
                } else {
                    NumberData newData;
                    newData.startPosition = position;
                    newData.endPosition   = position;
                    currentNumberData     = newData;
                }
            } else {
                checkNumberDataEnd();
            }
        }
        checkNumberDataEnd();
    }
    return numberDatas;
}

[[nodiscard]] auto isNumberAdjecentToGear(const Position2D& gearPosition, const Position2D& numberStart, const Position2D& numberEnd) -> bool {
    const int32_t xMin = numberStart.x - 1;
    const int32_t xMax = numberEnd.x + 1;
    const int32_t yMin = numberStart.y - 1;
    const int32_t yMax = numberEnd.y + 1;

    const bool isYInRange = yMin <= gearPosition.y && gearPosition.y <= yMax;
    const bool isXInRange = xMin <= gearPosition.x && gearPosition.x <= xMax;
    return isYInRange && isXInRange;
}

[[nodiscard]] auto getNumbersAdjecentToGear(const Position2D& gearPosition, const std::vector<NumberData>& numbers) -> std::vector<NumberData> {
    std::vector<NumberData> adjecentNumbers;

    for (const auto& number : numbers) {
        if (isNumberAdjecentToGear(gearPosition, number.startPosition, number.endPosition)) {
            adjecentNumbers.push_back(number);
        }
    }

    return adjecentNumbers;
}

[[nodiscard]] auto sumOfGearRatios(const std::vector<std::string>& input) -> std::string {
    const auto gearPositions = getPositionsOfGears(input);
    const auto numberDatas   = getNumberDatas(input);

    uint32_t sum = 0;
    for (const auto& gearPosition : gearPositions) {
        const auto adjecentNumbers = getNumbersAdjecentToGear(gearPosition, numberDatas);
        if (adjecentNumbers.size() == 2) {
            const auto& number1 = adjecentNumbers.at(0).number;
            const auto& number2 = adjecentNumbers.at(1).number;
            sum += (number1 * number2);
        }
    }

    return std::to_string(sum);
}
}  // namespace

auto Day3::part1() -> std::string {
    const auto input = getInput<Day3>(std::source_location::current());
    return sumOfPartNumbers(input);
};

auto Day3::part2() -> std::string {
    const auto input = getInput<Day3>(std::source_location::current());
    return sumOfGearRatios(input);
};
