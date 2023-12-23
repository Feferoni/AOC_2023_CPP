#include <cassert>
#include <map>
#include <optional>
#include <ranges>
#include <regex>
#include <set>
#include <span>

#include "day5.h"

#include "helperFunctions.h"

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

struct ConversionRange {
    uint32_t fromMin;
    uint32_t fromMax;
    uint32_t toMin;
    uint32_t toMax;

    friend auto operator<<(std::ostream& os, const ConversionRange& cr) -> std::ostream& {
        os << "From: " << cr.fromMin << "->" << cr.fromMax;
        os << " to: " << cr.toMin << "->" << cr.toMax;
        return os;
    }
};

using ConversionType     = std::pair<FarmingType, FarmingType>;
using ConversionMultiMap = std::multimap<ConversionType, ConversionRange>;

namespace {
constexpr char seedsPattern[]      = "seeds: (.*)";
constexpr char conversionPattern[] = "(\\w+)-to-(\\w+) map:";

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

[[nodiscard]] auto parseSeedNumbers(const std::string& line) -> std::vector<uint32_t> {
    std::smatch match;
    if (!std::regex_search(line, match, std::regex(seedsPattern)) || match.empty()) {
        std::cerr << "Could not extract: " << seedsPattern << "\n";
        std::cerr << "From: " << line << '\n';
        std::abort();
    }

    const std::string numbersString = match[1];
    return getNumbersFromString(numbersString);
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

[[nodiscard]] auto parseConversionData(const auto& input) -> ConversionMultiMap {
    ConversionMultiMap farmingTypeConversion;

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

        const auto numbers = getNumbersFromString(line);

        const auto      minDest = numbers.at(0);
        const auto      minSrc  = numbers.at(1);
        const auto      range   = numbers.at(2);
        ConversionRange conversionRange;
        conversionRange.toMin   = minDest;
        conversionRange.toMax   = minDest + range - 1;
        conversionRange.fromMin = minSrc;
        conversionRange.fromMax = minSrc + range - 1;
        farmingTypeConversion.insert(std::make_pair(currentConversion, conversionRange));
    }

    return farmingTypeConversion;
}

[[nodiscard]] auto convertFarmingTypeNumber(const uint32_t currentValue, const ConversionMultiMap& conversionMultiMap, const ConversionType& conversionType) -> uint32_t {
    auto multiMapRange = conversionMultiMap.equal_range(conversionType);
    for (auto it = multiMapRange.first; it != multiMapRange.second; ++it) {
        if (it->second.fromMin <= currentValue && currentValue <= it->second.fromMax) {
            const auto diff = currentValue - it->second.fromMin;
            return it->second.toMin + diff;
        }
    }

    return currentValue;
}

[[nodiscard]] auto getLowestLocationNumber(const std::vector<std::uint32_t>& seedNumbers, const ConversionMultiMap& farmingTypeConversionData) -> std::string {
    std::optional<uint32_t> minLocation = std::nullopt;
    std::ranges::for_each(seedNumbers, [&](const uint32_t seedNumber) {
        const auto soilNumber = convertFarmingTypeNumber(seedNumber, farmingTypeConversionData, {FarmingType::SEED, FarmingType::SOIL});
        const auto fertNumber = convertFarmingTypeNumber(soilNumber, farmingTypeConversionData, {FarmingType::SOIL, FarmingType::FERTILIZER});
        const auto waterNumber = convertFarmingTypeNumber(fertNumber, farmingTypeConversionData, {FarmingType::FERTILIZER, FarmingType::WATER});
        const auto tempNumber = convertFarmingTypeNumber(waterNumber, farmingTypeConversionData, {FarmingType::WATER, FarmingType::LIGHT});
        const auto lightNumber = convertFarmingTypeNumber(tempNumber, farmingTypeConversionData, {FarmingType::LIGHT, FarmingType::TEMPERATURE});
        const auto humiNumber = convertFarmingTypeNumber(lightNumber, farmingTypeConversionData, {FarmingType::TEMPERATURE, FarmingType::HUMIDITY});
        const auto locNumber = convertFarmingTypeNumber(humiNumber, farmingTypeConversionData, {FarmingType::HUMIDITY, FarmingType::LOCATION});

        if (minLocation.has_value() && minLocation > locNumber) {
            minLocation = locNumber;
        } else if (!minLocation.has_value()) {
            minLocation = locNumber;
        }
    });

    return std::to_string(minLocation.value_or(0));
}

[[nodiscard]] auto getModifiedSeedList(const std::vector<uint32_t>& seedNumbers) -> std::vector<uint32_t> {
    assert(seedNumbers.size() % 2 == 0);

    std::vector<uint32_t> newSeedNumbers;
    for (uint32_t i = 0; i < seedNumbers.size(); i += 2) {
        const auto& startNumber = seedNumbers.at(i);
        const auto& range       = seedNumbers.at(i + 1) - 1;

        for (auto j : std::views::iota(startNumber, startNumber + range - 1)) {
            newSeedNumbers.push_back(j);
        }
    }

    return newSeedNumbers;
}

}  // namespace

auto Day5::part1() -> std::string {
    const auto input          = getInput<Day5>(std::source_location::current());
    const auto seedNumbers    = parseSeedNumbers(input.at(0));
    const auto conversionData = parseConversionData(
        std::span(input.begin() + 2, input.end()));

    return getLowestLocationNumber(seedNumbers, conversionData);
};

auto Day5::part2() -> std::string {
    const auto input          = getInput<Day5>(std::source_location::current());
    const auto seedNumbers    = parseSeedNumbers(input.at(0));
    const auto conversionData = parseConversionData(
        std::span(input.begin() + 2, input.end()));
    const auto modifiedSeedNumbers = getModifiedSeedList(seedNumbers);
    return getLowestLocationNumber(modifiedSeedNumbers, conversionData);
};
