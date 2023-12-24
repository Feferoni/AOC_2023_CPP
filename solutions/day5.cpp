#include <cassert>
#include <map>
#include <optional>
#include <regex>
#include <span>

#include "day5.h"

#include "helperFunctions.h"

namespace {
using namespace Interval;
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

        const auto numbers = getNumbersFromString(line);

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

[[nodiscard]] auto convertFarmingTypeNumber(const uint32_t currentValue, const ConversionMap& conversionMap, const ConversionType& conversionType) -> uint32_t {
    const auto& it = conversionMap.find(conversionType);
    if (it != conversionMap.end()) {
        for (const auto& conversionRanges : it->second) {
            const auto convertedValue = conversionRanges.getConvertValue(currentValue);
            if (convertedValue.has_value()) {
                return convertedValue.value();
            }
        }
    }
    return currentValue;
}

[[maybe_unused]][[nodiscard]] auto getLowestLocationNumber(const std::vector<std::uint32_t>& seedNumbers, const ConversionMap& conversionMap) -> std::string {
    using namespace Interval;
    std::optional<uint32_t> minLocation = std::nullopt;
    std::ranges::for_each(seedNumbers, [&](const uint32_t seedNumber) {
        const auto soilNumber = convertFarmingTypeNumber(seedNumber, conversionMap, {FarmingType::SEED, FarmingType::SOIL});
        const auto fertNumber = convertFarmingTypeNumber(soilNumber, conversionMap, {FarmingType::SOIL, FarmingType::FERTILIZER});
        const auto waterNumber = convertFarmingTypeNumber(fertNumber, conversionMap, {FarmingType::FERTILIZER, FarmingType::WATER});
        const auto tempNumber = convertFarmingTypeNumber(waterNumber, conversionMap, {FarmingType::WATER, FarmingType::LIGHT});
        const auto lightNumber = convertFarmingTypeNumber(tempNumber, conversionMap, {FarmingType::LIGHT, FarmingType::TEMPERATURE});
        const auto humiNumber = convertFarmingTypeNumber(lightNumber, conversionMap, {FarmingType::TEMPERATURE, FarmingType::HUMIDITY});
        const auto locNumber = convertFarmingTypeNumber(humiNumber, conversionMap, {FarmingType::HUMIDITY, FarmingType::LOCATION});

        if (minLocation.has_value() && minLocation > locNumber) {
            minLocation = locNumber;
        } else if (!minLocation.has_value()) {
            minLocation = locNumber;
        }
    });

    return std::to_string(minLocation.value_or(0));
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

    std::cerr << "Could not find the conversionType: " << conversionType.first << "->"
              << conversionType.second << " in the conversionMap" << '\n';
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

    seedRanges           = Interval::mergeAdjecentRanges(seedRanges);
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
