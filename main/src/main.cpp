#include <chrono>
#include <concepts>  // IWYU pragma: keep
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "common/inc/inputHelper.h"
#include "solutions/inc/day1.h"
#include "solutions/inc/day2.h"
#include "solutions/inc/day3.h"
#include "solutions/inc/day4.h"
#include "solutions/inc/day5.h"
#include "solutions/inc/day6.h"
#include "solutions/inc/day7.h"
#include "solutions/inc/day8.h"
#include "solutions/inc/day9.h"

namespace {
template <class Day>
concept Solution = requires(Day day) {
    { Day::getInstance() } -> std::same_as<std::unique_ptr<Day>>;
    { Day::getInstance()->part1() } -> std::same_as<std::string>;
    { Day::getInstance()->part2() } -> std::same_as<std::string>;
    { day.part1() } -> std::same_as<std::string>;
    { day.part2() } -> std::same_as<std::string>;
};

static_assert(Solution<Day1>, "Must fulfill concept");
static_assert(Solution<Day2>, "Must fulfill concept");
static_assert(Solution<Day3>, "Must fulfill concept");
static_assert(Solution<Day4>, "Must fulfill concept");
static_assert(Solution<Day5>, "Must fulfill concept");
static_assert(Solution<Day6>, "Must fulfill concept");
static_assert(Solution<Day7>, "Must fulfill concept");
static_assert(Solution<Day8>, "Must fulfill concept");
static_assert(Solution<Day9>, "Must fulfill concept");

struct RunData {
    std::string               answer;
    std::chrono::microseconds runTime;

    friend std::ostream& operator<<(std::ostream& os, const RunData& rd) {
        auto micros = rd.runTime.count();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(rd.runTime)
                          .count();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(rd.runTime).count();
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(rd.runTime).count();
        auto hours = std::chrono::duration_cast<std::chrono::hours>(rd.runTime).count();

        micros %= 1000;  // Remaining microseconds
        millis %= 1000;  // Remaining milliseconds
        secs %= 60;      // Remaining seconds
        mins %= 60;      // Remaining minutes

        os << "Answer: " << rd.answer;
        os << " Elapsed time: " << std::setfill('0') << std::setw(2) << hours << ":";
        os << std::setfill('0') << std::setw(2) << mins << ":";
        os << std::setfill('0') << std::setw(2) << secs << ":";
        os << std::setfill('0') << std::setw(3) << millis << ":";
        os << std::setfill('0') << std::setw(3) << micros;
        return os;
    }
};

auto executePart(const std::function<std::string()>& function) -> RunData {
    const auto start        = std::chrono::high_resolution_clock::now();
    const auto partSolution = function();
    const auto end          = std::chrono::high_resolution_clock::now();
    return {partSolution, std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

template <Solution Day>
auto printSolutionFor() -> void {
    const auto day_up    = Day::getInstance();
    const auto className = demangle<Day>();

    std::cout << className << "::part1 "
              << executePart([&day_up]() { return (*day_up.get()).part1(); }) << '\n';
    std::cout << className << "::part2 "
              << executePart([&day_up]() { return (*day_up.get()).part2(); }) << '\n';
}
}  // namespace

auto main() -> int {
    printSolutionFor<Day1>();
    printSolutionFor<Day2>();
    printSolutionFor<Day3>();
    printSolutionFor<Day4>();
    printSolutionFor<Day5>();
    printSolutionFor<Day6>();
    printSolutionFor<Day7>();
    printSolutionFor<Day8>();
    printSolutionFor<Day9>();

    return 0;
}
