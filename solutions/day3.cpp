#include <algorithm>
#include <optional>

#include "day3.h"
#include "helperFunctions.h"

namespace {
// clang-format off
constexpr std::array<Position, 8> directions = {Position{ 1,  0},  // right
                                                Position{-1,  0},  // left
                                                Position{ 0,  1},  // up
                                                Position{ 0, -1},  // down
                                                Position{ 1,  1},  // up; right
                                                Position{-1,  1},  // up, left
                                                Position{ 1, -1},  // down; right
                                                Position{-1, -1}}; // down, left
// clang-format on
}  // namespace

[[nodiscard]] auto getNewPosition(const Position& current, const Position& direction, const Position& positionLimit) -> std::optional<Position> {
    Position newPosition{current.x + direction.x, current.y + direction.y};
    if (0 <= newPosition.x && newPosition.x <= positionLimit.x &&
        0 <= newPosition.y && newPosition.y <= positionLimit.y) {
        return newPosition;
    }
    return std::nullopt;
}

[[nodiscard]] auto isCharValidSymbol(const char c) -> bool {
    constexpr std::array<std::pair<uint32_t, uint32_t>, 3> validRanges = {std::pair<uint32_t, uint32_t>{33u, 45u}, std::pair<uint32_t, uint32_t>{47u, 47u}, std::pair<uint32_t, uint32_t>{58u, 64u}};

    if (c == 46) {
        return false;
    }

    return std::ranges::any_of(validRanges, [&c](const std::pair<uint32_t, uint32_t>& minMax) {
        return minMax.first <= c && c <= minMax.second;
    });
}

[[nodiscard]] auto getCharFromInput(const std::vector<std::string>& input, const Position& position, const Position& positionLimit, const std::source_location& caller) -> char {
    try {
        return input.at(position.y).at(position.x);
    } catch (std::out_of_range ex) {
        std::cerr << ex.what() << ": " << caller.function_name() << " current position: " << positionLimit << " limit: " << positionLimit << '\n';
        std::abort();
    }
}

[[nodiscard]] auto isPositionAdjecentToSymbol(const std::vector<std::string>& input, const Position& position, const Position& positionLimit) -> bool {
    for (const auto& direction : directions) {
        const auto adjecentPosition = getNewPosition(position, direction, positionLimit);
        if (!adjecentPosition.has_value()) {
            continue;
        }
        const auto adjecentSymbol = getCharFromInput(input, adjecentPosition.value(), positionLimit, std::source_location::current());
        if (isCharValidSymbol(adjecentSymbol)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] auto sumOfPartNumbers(const std::vector<std::string>& input) -> std::string {
    const Position positionLimit = {static_cast<int32_t>(input.front().size() - 1),
                                    static_cast<int32_t>(input.size() - 1)};

    uint32_t    sum                      = 0;
    bool        foundAdjecentValidSymbol = false;
    std::string currentNumber            = "";
    for (size_t y = 0; y <= positionLimit.y; y++) {
        const auto& currentLine = input.at(y);
        for (size_t x = 0; x <= positionLimit.x; x++) {
            const Position currentPosition{static_cast<int32_t>(x), static_cast<int32_t>(y)};
            const char currentChar = getCharFromInput(input, currentPosition, positionLimit, std::source_location::current());
            if (std::isdigit(currentChar)) {
                currentNumber.push_back(currentChar);
                if (isPositionAdjecentToSymbol(input, currentPosition, positionLimit)) {
                    foundAdjecentValidSymbol = true;
                }
            } else {
                if (foundAdjecentValidSymbol) {
                    foundAdjecentValidSymbol = false;
                    sum += std::stoul(currentNumber);
                }
                currentNumber = "";
            }
        }
        if (foundAdjecentValidSymbol) {
            foundAdjecentValidSymbol = false;
            sum += std::stoul(currentNumber);
        }
        currentNumber = "";
    }
    return std::to_string(sum);
}

auto Day3::part1() -> std::string {
    const auto input = getInput<Day3>(std::source_location::current());
    return sumOfPartNumbers(input);
};

struct NumberData {
    Position startPosition;
    Position endPosition;
    uint32_t number;

    friend std::ostream& operator<<(std::ostream& os, const NumberData& nd) {
        os << "NumberData startPos: " << nd.startPosition << " endPos: " << nd.endPosition << " Number: " << nd.number;
        return os;
    }
};

[[nodiscard]] auto getPositionsOfGears(const std::vector<std::string>& input) -> std::vector<Position> {
    const Position positionLimit = {static_cast<int32_t>(input.front().size() - 1),
                                    static_cast<int32_t>(input.size() - 1)};
    std::vector<Position> gearPositions;

    for (size_t y = 0; y <= positionLimit.y; y++) {
        for (size_t x = 0; x <= positionLimit.x; x++) {
            const Position position{static_cast<int32_t>(x), static_cast<int32_t>(y)};
            const auto currentChar = getCharFromInput(input, position, positionLimit, std::source_location::current());
            if (currentChar == 42) {
                gearPositions.push_back(position);
            }
        }
    }

    return gearPositions;
}

[[nodiscard]] auto getNumberDatas(const std::vector<std::string>& input) -> std::vector<NumberData> {
    const Position positionLimit = {static_cast<int32_t>(input.front().size() - 1),
                                    static_cast<int32_t>(input.size() - 1)};
    std::vector<NumberData> numberDatas;

    std::optional<NumberData> currentNumberData = std::nullopt;
    for (size_t y = 0; y <= positionLimit.y; y++) {
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

        for (size_t x = 0; x <= positionLimit.x; x++) {
            const Position position{static_cast<int32_t>(x), static_cast<int32_t>(y)};
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

[[nodiscard]] auto isNumberAdjecentToGear(const Position& gearPosition, const Position& numberStart, const Position& numberEnd) -> bool {
    const int32_t xMin = numberStart.x - 1;
    const int32_t xMax = numberEnd.x + 1;
    const int32_t yMin = numberStart.y - 1;
    const int32_t yMax = numberEnd.y + 1;

    const bool isYInRange = yMin <= gearPosition.y && gearPosition.y <= yMax;
    const bool isXInRange = xMin <= gearPosition.x && gearPosition.x <= xMax;
    return isYInRange && isXInRange;
}

[[nodiscard]] auto getNumbersAdjecentToGear(const Position& gearPosition, const std::vector<NumberData>& numbers) -> std::vector<NumberData> {
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

auto Day3::part2() -> std::string {
    const auto input = getInput<Day3>(std::source_location::current());
    return sumOfGearRatios(input);
};
