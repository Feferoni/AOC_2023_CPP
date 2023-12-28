#include <cassert>

#include "day6.h"

#include "errorMsg.h"
#include "inputHelper.h"
#include "stringHelper.h"

struct RaceData {
    int64_t time;
    int64_t distance;

    friend auto operator<<(std::ostream& os, const RaceData& rd) -> std::ostream& {
        os << "T: " << rd.time << " D: " << rd.distance;
        return os;
    }
};

namespace {
[[nodiscard]] auto parseInputPart1(const std::string& inputStr) -> std::vector<int64_t> {
    const auto splitStr = helper::string::splitString(inputStr, ":");
    assert(splitStr.size() == 2 && "Size of split string is wrong.");
    return helper::string::getNumbersFromString<int64_t>(splitStr.at(1));
}
[[nodiscard]] auto parseInputPart1(const std::vector<std::string>& input) -> std::vector<RaceData> {
    assert(input.size() == 2 && "Size of input is wrong.");
    const auto timeInput = parseInputPart1(input.at(0));
    const auto distInput = parseInputPart1(input.at(1));

    std::vector<RaceData> parsedInput;
    for (int64_t i = 0; i < static_cast<int64_t>(timeInput.size()); i++) {
        parsedInput.push_back(RaceData{timeInput.at(i), distInput.at(i)});
    }
    return parsedInput;
}

[[nodiscard]] auto parseNumber(const std::string& inputStr) -> int64_t {
    const auto splitStr = helper::string::splitString(inputStr, ":");
    assert(splitStr.size() == 2 && "Size of split string is wrong.");
    const auto splitNumbers = helper::string::splitString(splitStr.at(1), " ");

    std::string numberStr;
    std::ranges::for_each(splitNumbers, [&](const std::string& str) {
        if (!str.empty()) {
            numberStr += str;
        }
    });

    try {
        return std::stoll(numberStr);
    } catch (const std::invalid_argument& ex) {
        ERROR_MSG_AND_EXIT(ex.what() << " - " << numberStr);
    } catch (const std::out_of_range& ex) {
        ERROR_MSG_AND_EXIT(ex.what() << " - " << numberStr);
    }
}

[[nodiscard]] auto parseInputPart2(const std::vector<std::string>& input) -> RaceData {
    assert(input.size() == 2 && "Size of input is wrong.");
    const auto time = parseNumber(input.at(0));
    const auto dist = parseNumber(input.at(1));
    return RaceData{time, dist};
}

[[nodiscard]] auto countNumberOfWinningScenarios(const std::vector<RaceData>& raceDatas) -> int64_t {
    const auto calculateDistance = [](const int64_t chargeTime, const int64_t timeLimit) -> int64_t {
        return chargeTime * timeLimit - chargeTime * chargeTime;
    };

    int64_t sum = 1;
    for (const auto& raceData : raceDatas) {
        [[maybe_unused]] int64_t count      = 0;
        int64_t                  chargeTime = 1;
        while (true) {
            const auto distance = calculateDistance(chargeTime, raceData.time);
            if (distance > raceData.distance) {
                count++;
            } else if (distance < 0) {
                break;
            }
            chargeTime += 1;
        }
        if (count > 0) {
            sum *= count;
        }
    }
    return sum;
}
}  // namespace

auto Day6::part1() -> std::string {
    const auto input = helper::input::getInput<Day6>(std::source_location::current());
    const auto parsedInput = parseInputPart1(input);
    return std::to_string(countNumberOfWinningScenarios(parsedInput));
};

auto Day6::part2() -> std::string {
    const auto input = helper::input::getInput<Day6>(std::source_location::current());
    const auto parsedInput = parseInputPart2(input);
    return std::to_string(countNumberOfWinningScenarios({parsedInput}));
};
