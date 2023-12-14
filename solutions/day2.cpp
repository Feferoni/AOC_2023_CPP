#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <string_view>

#include "day2.h"

#include "helperFunctions.h"

namespace {
constexpr char saveGrabbedCubesPattern[] = "Game (\\d+): (.*)";
constexpr char getColorInfoPattern[]     = "\\W*?(\\d+) (\\S+)";

struct ColoredCubes {
    uint32_t blue  = 0;
    uint32_t red   = 0;
    uint32_t green = 0;

    [[maybe_unused]] friend std::ostream& operator<<(std::ostream& os, const ColoredCubes& g) {
        os << "{blue: " << g.blue << " red: " << g.red << " green: " << g.green << "}";
        return os;
    }
};

struct Game {
    std::vector<ColoredCubes> grab;
    uint32_t                  nr = 0;


    [[maybe_unused]] friend std::ostream& operator<<(std::ostream& os, const Game& g) {
        os << "\nGameNumber: " << g.nr << " grabs: " << g.grab;
        return os;
    }
};

enum class Color : int {
    GREEN,
    BLUE,
    RED
};

[[nodiscard]] auto getColor(const std::string str) -> Color {
    if (str == "green") return Color::GREEN;
    if (str == "blue") return Color::BLUE;
    if (str == "red") return Color::RED;

    std::cerr << "Faulty color: " << str << '\n';
    std::abort();
}

[[nodiscard]] auto splitGameAndGrabs(const std::string& line) -> std::pair<uint32_t, std::string> {
    std::smatch grabbedCubesMatch;
    if (!std::regex_search(line, grabbedCubesMatch, std::regex(saveGrabbedCubesPattern)) ||
        grabbedCubesMatch.empty()) {
        std::cerr << "Could not extract the grabbed cubes:\n"
                  << line << "\nwith regex:\n"
                  << saveGrabbedCubesPattern << '\n';
        std::abort();
    }

    return {std::stoul(grabbedCubesMatch[1]), grabbedCubesMatch[2]};
}

[[nodiscard]] auto getColorCount(const std::string_view str) -> std::pair<uint32_t, Color> {
    std::string colorInfo(str);
    std::regex  regex(getColorInfoPattern);

    std::smatch match;
    if (!std::regex_search(colorInfo, match, std::regex(getColorInfoPattern)) ||
        match.empty()) {
        std::cerr << "Could not extract color and number from:\n"
                  << str << "\nwith regex:\n"
                  << getColorInfoPattern << '\n';
        std::abort();
    }

    auto count = std::stoul(match[1]);
    auto color = getColor(match[2]);

    return {count, color};
}

[[nodiscard]] auto convertToGrabbedCubes(const std::string_view str) -> ColoredCubes {
    const auto   colorsInfo = splitStrToStrView(str, ",");
    ColoredCubes cubes{};

    for (const auto& colorInfo : colorsInfo) {
        const auto [count, color] = getColorCount(colorInfo);
        switch (color) {
        case Color::GREEN: cubes.green += count; break;
        case Color::RED: cubes.red += count; break;
        case Color::BLUE: cubes.blue += count; break;
        default:
            std::cerr << "Faulty enum color: " << static_cast<uint32_t>(color) << '\n';
            std::abort();
        }
    }

    return cubes;
}

[[nodiscard]] auto parseLineIntoGameInfo(const std::string& line) -> Game {
    const auto [gameNr, gameInfo] = splitGameAndGrabs(line);
    const auto eachGrab           = splitStrToStrView(gameInfo, ";");

    Game currentGame{{}, gameNr};
    std::transform(
        eachGrab.begin(), eachGrab.end(), std::back_inserter(currentGame.grab), convertToGrabbedCubes);

    return currentGame;
}

[[nodiscard]] auto getGames(const std::vector<std::string>& input) -> std::vector<Game> {
    std::vector<Game> games;
    std::ranges::transform(input, std::back_inserter(games), [](const std::string& line) -> Game {
        return parseLineIntoGameInfo(line);
    });
    return games;
}

[[nodiscard]] auto isGamePossible(const Game& game, const ColoredCubes& gameConfiguration) -> std::optional<uint32_t> {
    const bool isAllGrabsValid = std::ranges::all_of(game.grab, [&gameConfiguration](const auto& grab) {
        const auto greenIsValid = grab.green <= gameConfiguration.green;
        const auto redIsValid   = grab.red <= gameConfiguration.red;
        const auto blueIsValid  = grab.blue <= gameConfiguration.blue;
        return greenIsValid && redIsValid && blueIsValid;
    });
    return isAllGrabsValid ? std::optional<uint32_t>(game.nr) : std::nullopt;
}

[[nodiscard]] auto playGamePart1(const std::vector<Game>& games, const ColoredCubes& gameConfiguration) -> uint32_t {
    uint32_t result = 0;
    std::ranges::for_each(games, [&result, &gameConfiguration](const auto& game) {
        if (const auto gameResult = isGamePossible(game, gameConfiguration);
            gameResult.has_value()) {
            result += *gameResult;
        }
    });
    return result;
}

auto setValueIfLarger(std::optional<uint32_t>& valueToSet, const auto currentValue) -> void {
    if (currentValue > valueToSet.value_or(0)) {
        valueToSet = currentValue;
    }
}

[[nodiscard]] auto getSmallestConfiguration(const Game& game) -> ColoredCubes {
    std::optional<uint32_t> minRed, minGreen, minBlue;

    std::ranges::for_each(game.grab, [&minRed, &minGreen, &minBlue](const ColoredCubes& coloredCubes) {
        setValueIfLarger(minBlue, coloredCubes.blue);
        setValueIfLarger(minRed, coloredCubes.red);
        setValueIfLarger(minGreen, coloredCubes.green);
    });

    return ColoredCubes{minBlue.value_or(0), minRed.value_or(0), minGreen.value_or(0)};
}

[[nodiscard]] auto playGamePart2(const std::vector<Game>& games) -> uint64_t {
    uint64_t result = 0;
    std::ranges::for_each(games, [&result](const auto& game) {
        const auto smallestConfiguration = getSmallestConfiguration(game);
        result += smallestConfiguration.red * smallestConfiguration.green *
                  smallestConfiguration.blue;
    });
    return result;
}
}  // namespace


auto Day2::part1() -> std::string {
    const auto input = getInput<Day2>(std::source_location::current());
    const auto games = getGames(input);

    const ColoredCubes gameConfiguration{13, 12, 14};
    return std::to_string(playGamePart1(games, gameConfiguration));
};


auto Day2::part2() -> std::string {
    const auto input  = getInput<Day2>(std::source_location::current());
    const auto games  = getGames(input);
    const auto result = playGamePart2(games);
    return std::to_string(result);
};
