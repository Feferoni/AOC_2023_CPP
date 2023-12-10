#include <chrono>
#include <concepts>
#include <functional>
#include <iostream>
#include <string>

#include "day1.h"
#include "day2.h"
#include "helperFunctions.h"

bool IsTest::isTest = false;

template <class Day>
concept Solution = requires() {
    { Day::part1() } -> std::same_as<std::string>;
    { Day::part2() } -> std::same_as<std::string>;
};

struct RunData
{
    std::string               answer;
    std::chrono::milliseconds runTime;

    friend std::ostream &
    operator<<(std::ostream &os, const RunData &rd)
    {
        os << "Answer: " << rd.answer << " Elapsed time (ms): " << rd.runTime.count();
        return os;
    }
};

auto
executePart(const std::function<std::string()> &function) -> RunData
{
    const auto start         = std::chrono::high_resolution_clock::now();
    const auto part1Solution = function();
    const auto end           = std::chrono::high_resolution_clock::now();
    return {part1Solution, std::chrono::duration_cast<std::chrono::milliseconds>(end - start)};
}

template <Solution Day>
auto
printSolutionFor() -> void
{
    const auto className = demangle<Day>();
    std::cout << className << "::part1 " << executePart(Day::part1) << '\n';
    std::cout << className << "::part2 " << executePart(Day::part2) << '\n';
}

static_assert(&printSolutionFor<Day1>, "Must fulfill concept");

auto
main() -> int
{
    printSolutionFor<Day1>();
    printSolutionFor<Day2>();

    return 0;
}
