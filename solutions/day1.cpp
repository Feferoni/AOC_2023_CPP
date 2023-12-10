#include <cctype>

#include "day1.h"

#include "helperFunctions.h"

#include "map"

auto
Day1::part1() -> std::string
{
    const auto input = getInput<Day1>(std::source_location::current());

    std::uint32_t sum = 0;
    for (const auto &line : input)
    {
        auto isDigit = [](const char c) { return std::isdigit(c); };

        char        first  = *std::find_if(line.begin(), line.end(), isDigit);
        char        last   = *std::find_if(line.rbegin(), line.rend(), isDigit);
        std::string number = {first, last};
        sum += std::stoul(number);
    }

    return std::to_string(sum);
};

std::map<std::string, std::int32_t> conversionTable = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

std::string
findFirstDigit(const std::string &line)
{
    for (uint32_t i = 0; i < line.size(); i++)
    {
        auto previous = line.substr(0, i);
        for (const auto &[alphaNumber, digit] : conversionTable)
        {
            auto it = previous.find(alphaNumber);
            if (it != std::string::npos)
            {
                return std::to_string(digit);
            }
        }

        char c = line.at(i);
        if (std::isdigit(c))
        {
            return std::string{c};
        }
    }

    return "";
}

std::string
findLastDigit(const std::string &line)
{
    for (uint32_t i = line.size() - 1; 0 <= i; i--)
    {
        auto previous = line.substr(i, line.size() - 1);
        for (const auto &[alphaNumber, digit] : conversionTable)
        {
            auto it = previous.find(alphaNumber);
            if (it != std::string::npos)
            {
                return std::to_string(digit);
            }
        }

        char c = line.at(i);
        if (std::isdigit(c))
        {
            return std::string{c};
        }
    }

    return "";
}

auto
Day1::part2() -> std::string
{
    const auto input = getInput<Day1>(std::source_location::current());

    std::uint32_t sum = 0;
    for (const auto &line : input)
    {
        const auto first  = findFirstDigit(line);
        const auto last   = findLastDigit(line);
        const auto number = first + last;
        sum += std::stoul(number);
    }

    return std::to_string(sum);
};
