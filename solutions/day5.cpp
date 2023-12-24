#include <cassert>
#include <map>
#include <optional>
#include <regex>
#include <span>

#include "day5.h"

#include "fileHelper.h"
#include "intervalHelper.h"
#include "stringHelper.h"

namespace {
using namespace helper::interval;

enum class FarmingType : uint32_t {
    SEED,
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION
};

auto operator<<(std::ostream& os, const FarmingType ft) -> std::ostream& {
    switch (ft) {
    case FarmingType::SEED: os << "SEED"; break;
    case FarmingType::SOIL: os << "SOIL"; break;
    case FarmingType::FERTILIZER: os << "FERTILIZER"; break;
    case FarmingType::WATER: os << "WATER"; break;
    case FarmingType::LIGHT: os << "LIGHT"; break;
    case FarmingType::TEMPERATURE: os << "TEMPERATURE"; break;
    case FarmingType::HUMIDITY: os << "HUMIDITY"; break;
    case FarmingType::LOCATION: os << "LOCATION"; break;
    default:
        std::cerr << "Faulty farmingType: " << static_cast<uint32_t>(ft) << '\n';
        std::abort();
    }

    return os;
}

using ConversionType   = std::pair<FarmingType, FarmingType>;
using ConversionRanges = std::vector<ConversionRange>;
using ConversionMap    = std::map<ConversionType, std::vector<ConversionRange>>;

[[nodiscard]] auto operator<<(std::ostream& os, const std::pair<FarmingType, FarmingType>& ftPair) -> std::ostream& {
    os << ftPair.first << "->" << ftPair.second;
    return os;
}

[[nodiscard]] auto getFarmingTypeFromStr(const std::string& type) -> FarmingType {
    if (type == "seed") return FarmingType::SEED;
    if (type == "soil") return FarmingType::SOIL;
    if (type == "fertilizer") return FarmingType::FERTILIZER;
    if (type == "water") return FarmingType::WATER;
    if (type == "light") return FarmingType::LIGHT;
    if (type == "temperature") return FarmingType::TEMPERATURE;
    if (type == "humidity") return FarmingType::HUMIDITY;
    if (type == "location") return FarmingType::LOCATION;

    std::cerr << "Not a valid type: " << type << '\n';
    std::abort();
}

constexpr char seedsPattern[]      = "seeds: (.*)";
constexpr char conversionPattern[] = "(\\w+)-to-(\\w+) map:";

[[nodiscard]] auto parseSeedNumbers(const std::string& line) -> std::vector<uint32_t> {
    std::smatch match;
    if (!std::regex_search(line, match, std::regex(seedsPattern)) || match.empty()) {
        std::cerr << "Could not extract: " << seedsPattern << "\n";
        std::cerr << "From: " << line << '\n';
        std::abort();
    }

    const std::string numbersString = match[1];
    return helper::string::getNumbersFromString(numbersString);
}

[[nodiscard]] auto parseFarmingTypes(const std::string& line) -> std::optional<std::pair<FarmingType, FarmingType>> {
    std::smatch match;
    if (!std::regex_search(line, match, std::regex(conversionPattern)) || match.empty()) {
        return std::nullopt;
    }

    const auto fromType = getFarmingTypeFromStr(match[1]);
    const auto toType   = getFarmingTypeFromStr(match[2]);

    return std::make_pair(fromType, toType);
}

[[nodiscard]] auto parseConversionData(const auto& input) -> ConversionMap {
    ConversionMap conversionMap;

    std::pair<FarmingType, FarmingType> currentConversion;
    for (const auto& line : input) {
        if (line.empty()) {
            continue;
        }

        const auto newFarmingType = parseFarmingTypes(line);
        if (newFarmingType.has_value()) {
            currentConversion = newFarmingType.value();
            continue;
        }

        const auto numbers = helper::string::getNumbersFromString(line);

        const auto      minDest = numbers.at(0);
        const auto      minSrc  = numbers.at(1);
        const auto      range   = numbers.at(2);
        ConversionRange conversionRange;
        conversionRange.to   = {minDest, minDest + range - 1};
        conversionRange.from = {minSrc, minSrc + range - 1};

        auto it = conversionMap.find(currentConversion);
        if (it == conversionMap.end()) {
            conversionMap[currentConversion] = {conversionRange};
        } else {
            it->second.push_back(conversionRange);
        }
    }

    for (auto& [_, conversionRanges] : conversionMap) {
        std::sort(conversionRanges.begin(), conversionRanges.end());
    }

    return conversionMap;
}

[[nodiscard]] auto getSeedRanges(const std::vector<uint32_t>& seedNumbers) -> std::vector<Range> {
    assert(seedNumbers.size() % 2 == 0);

    std::vector<Range> seedRanges;

    for (uint32_t i = 0; i < seedNumbers.size(); i += 2) {
        const auto& startNumber = seedNumbers.at(i);
        const auto& range       = seedNumbers.at(i + 1);
        seedRanges.push_back({startNumber, startNumber + range - 1});
    }

    return mergeAdjecentRanges(seedRanges);
}

[[nodiscard]] auto getConversionRangesFromMap(const ConversionType& conversionType, const ConversionMap& conversionMap) -> const ConversionRanges& {
    const auto& it = conversionMap.find(conversionType);
    if (it != conversionMap.end()) {
        return it->second;
    }

    std::cerr << "Could not find the conversionType: " << conversionType << " in the conversionMap" << '\n';
    std::abort();
}

[[nodiscard]] auto getLocationRanges(const std::vector<Range>& seedRanges, const ConversionMap& conversionMap) -> std::vector<Range> {
    const auto soilRanges = getConvertedRanges(seedRanges, getConversionRangesFromMap({FarmingType::SEED, FarmingType::SOIL}, conversionMap));
    const auto fertRanges = getConvertedRanges(soilRanges, getConversionRangesFromMap({FarmingType::SOIL, FarmingType::FERTILIZER}, conversionMap));
    const auto waterRanges = getConvertedRanges(fertRanges, getConversionRangesFromMap({FarmingType::FERTILIZER, FarmingType::WATER}, conversionMap));
    const auto lightRanges = getConvertedRanges(waterRanges, getConversionRangesFromMap({FarmingType::WATER, FarmingType::LIGHT}, conversionMap));
    const auto tempRanges = getConvertedRanges(lightRanges, getConversionRangesFromMap({FarmingType::LIGHT, FarmingType::TEMPERATURE}, conversionMap));
    const auto humidityRanges = getConvertedRanges(tempRanges, getConversionRangesFromMap({FarmingType::TEMPERATURE, FarmingType::HUMIDITY}, conversionMap));
    const auto locRanges = getConvertedRanges(humidityRanges, getConversionRangesFromMap({FarmingType::HUMIDITY, FarmingType::LOCATION}, conversionMap));
    return locRanges;
}
}  // namespace

auto Day5::part1() -> std::string {
    const auto input          = getInput<Day5>(std::source_location::current());
    const auto seedNumbers    = parseSeedNumbers(input.at(0));
    const auto conversionData = parseConversionData(
        std::span(input.begin() + 2, input.end()));

    std::vector<Range> seedRanges;
    std::transform(seedNumbers.begin(), seedNumbers.end(), std::back_inserter(seedRanges), [](const uint32_t nr) -> Range {
        return Range{nr, nr};
    });

    seedRanges           = helper::interval::mergeAdjecentRanges(seedRanges);
    const auto locRanges = getLocationRanges(seedRanges, conversionData);
    return std::to_string(locRanges.front().min);
};

auto Day5::part2() -> std::string {
    const auto input          = getInput<Day5>(std::source_location::current());
    const auto seedNumbers    = parseSeedNumbers(input.at(0));
    const auto conversionData = parseConversionData(
        std::span(input.begin() + 2, input.end()));
    const auto seedRange = getSeedRanges(seedNumbers);
    const auto locRanges = getLocationRanges(seedRange, conversionData);
    return std::to_string(locRanges.front().min);
};
