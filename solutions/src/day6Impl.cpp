#include "day6Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"
#include "common/inc/stringHelper.h"

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
    if (splitStr.size() != 2)
        ERROR_MSG_AND_EXIT("Sie of split string is not 2. Size: " << splitStr.size());
    return helper::string::getNumbersFromString<int64_t>(splitStr.at(1));
}
[[nodiscard]] auto parseInputToRacesDataPart1(const std::vector<std::string>& input) -> std::vector<RaceData> {
    if (input.size() != 2)
        ERROR_MSG_AND_EXIT("Size of input is not 2. Size: " << input.size());
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
    if (splitStr.size() != 2)
        ERROR_MSG_AND_EXIT("Sie of split string is not 2. Size: " << splitStr.size());
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

[[nodiscard]] auto parseInputToRaceDataPart2(const std::vector<std::string>& input) -> RaceData {
    if (input.size() != 2)
        ERROR_MSG_AND_EXIT("Size of input is not 2. Size: " << input.size());
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

auto Day6::getInstance() -> std::unique_ptr<Day6> {
    return std::make_unique<Day6Impl>();
}

auto Day6Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day6>(std::source_location::current());
    const auto racesData = parseInputToRacesDataPart1(input);
    return std::to_string(countNumberOfWinningScenarios(racesData));
};

auto Day6Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day6>(std::source_location::current());
    const auto raceData = parseInputToRaceDataPart2(input);
    return std::to_string(countNumberOfWinningScenarios({raceData}));
};
