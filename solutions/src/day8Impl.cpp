#include <algorithm>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
#include <span>

#include "day8Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

// LLR

// AAA = (BBB, BBB)
// BBB = (AAA, ZZZ)
// ZZZ = (ZZZ, ZZZ

enum class Direction : uint32_t {
    Left,
    Right
};

auto operator<<(std::ostream& os, const Direction direction) -> std::ostream& {
    switch (direction) {
    case Direction::Left: os << "Left"; break;
    case Direction::Right: os << "Right"; break;
    default:
        ERROR_MSG_AND_EXIT("Non valid direction encoutered: " << static_cast<uint32_t>(direction));
    }
    return os;
}

struct Node {
    std::string left;
    std::string right;

    [[nodiscard]] auto getNeighbour(const Direction direction) -> std::string {
        switch (direction) {
        case Direction::Left: return left;
        case Direction::Right: return right;
        default:
            ERROR_MSG_AND_EXIT("Non valid direction encoutered: " << static_cast<uint32_t>(direction));
        }
    }
};

namespace {
[[nodiscard]] auto getDirectionsFromInput(const std::string& input) -> std::vector<Direction> {
    std::vector<Direction> directions;
    std::ranges::for_each(input, [&directions, &input](const char c) {
        if (c == 'L') directions.push_back(Direction::Left);
        else if (c == 'R') directions.push_back(Direction::Right);
        else
            ERROR_MSG_AND_EXIT("Not a valid character: " << c << " found in input: " << input);
    });
    return directions;
}

constexpr char nodePattern[] = "(\\w{3}) = \\((\\w{3}), (\\w{3})\\)";

[[nodiscard]] auto getNodesFromInput(const auto input) -> std::map<std::string, Node> {
    std::map<std::string, Node> nodes;

    std::ranges::for_each(input, [&nodes](const std::string& line) {
        std::smatch match;
        if (!std::regex_search(line, match, std::regex(nodePattern)) || match.empty()) {
            ERROR_MSG_AND_EXIT("Could not extract: " << nodePattern << " from: " << line);
        }
        std::string nodeName = match[1];
        if (nodes.count(nodeName) > 0)
            ERROR_MSG_AND_EXIT("Node: " << nodeName << " already exist in the graph.");

        std::string leftNodeName  = match[2];
        std::string rightNodeName = match[3];

        nodes[nodeName] = {leftNodeName, rightNodeName};
    });

    return nodes;
}

[[nodiscard]] auto doesNodeNameEndWith(const std::string& nodeName, const char c) -> bool {
    if (nodeName.empty()) ERROR_MSG_AND_EXIT("NodeName is empty.");

    const auto nodeName_it = nodeName.crbegin();
    return c == *nodeName_it;
}

[[nodiscard]] auto findStepsToPath(const std::vector<Direction>& directions, const std::map<std::string, Node>& nodes, const std::string& startNodeName, const std::function<bool(const std::string&)>& isFinishedFunc) -> uint32_t {
    uint32_t    steps           = 0;
    auto        direction_it    = directions.cbegin();
    std::string currentNodeName = startNodeName;
    while (!isFinishedFunc(currentNodeName)) {
        if (direction_it == directions.cend()) {
            direction_it = directions.cbegin();
        }

        const auto& currentNode = nodes.find(currentNodeName);
        switch (*direction_it) {
        case Direction::Left: currentNodeName = currentNode->second.left; break;
        case Direction::Right:
            currentNodeName = currentNode->second.right;
            break;
        default:
            ERROR_MSG_AND_EXIT("Non valid direction encoutered: " << static_cast<uint32_t>(*direction_it));
        }

        steps++;
        direction_it++;
    }

    return steps;
}

}  // namespace

auto Day8::getInstance() -> std::unique_ptr<Day8> {
    return std::make_unique<Day8Impl>();
}

auto Day8Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day8>(std::source_location::current());
    const auto directions = getDirectionsFromInput(input.at(0));
    const auto nodes = getNodesFromInput(std::span(input.begin() + 2, input.end()));
    const auto steps = findStepsToPath(directions, nodes, "AAA", [](const std::string& nodeName) -> bool {
        return nodeName == "ZZZ";
    });
    return std::to_string(steps);
};

auto Day8Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day8>(std::source_location::current());
    const auto directions = getDirectionsFromInput(input.at(0));
    const auto nodes = getNodesFromInput(std::span(input.begin() + 2, input.end()));

    std::vector<std::pair<std::string, uint32_t>> currentNodeNames;
    std::ranges::for_each(nodes, [&currentNodeNames](const auto& nodePair) {
        const auto& [nodeName, _] = nodePair;
        if (doesNodeNameEndWith(nodeName, 'A')) {
            currentNodeNames.push_back({nodeName, 0});
        }
    });

    std::ranges::for_each(currentNodeNames, [&directions, &nodes](auto& currentNodeNamePair) {
        auto& [startNodeName, stepsUntilFinish] = currentNodeNamePair;
        const auto& finishFunc = [] (const std::string& nodeName) { return doesNodeNameEndWith(nodeName, 'Z'); };
        stepsUntilFinish = findStepsToPath(directions, nodes, startNodeName, finishFunc);
    });

    uint64_t stepMultiple = 1;
    for (const auto& [_, steps] : currentNodeNames) {
        stepMultiple = std::lcm(stepMultiple, steps);
    }

    return std::to_string(stepMultiple);
};
