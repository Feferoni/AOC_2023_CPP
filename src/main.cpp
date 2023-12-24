#include <chrono>
#include <concepts>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "inputHelper.h"

namespace {
template <class Day>
concept Solution = requires() {
    { Day::part1() } -> std::same_as<std::string>;
    { Day::part2() } -> std::same_as<std::string>;
};

static_assert(Solution<Day1>, "Must fulfill concept");

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
    const auto start         = std::chrono::high_resolution_clock::now();
    const auto part1Solution = function();
    const auto end           = std::chrono::high_resolution_clock::now();
    return {part1Solution, std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

template <Solution Day>
auto printSolutionFor() -> void {
    const auto className = demangle<Day>();
    std::cout << className << "::part1 " << executePart(Day::part1) << '\n';
    std::cout << className << "::part2 " << executePart(Day::part2) << '\n';
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

    return 0;
}
