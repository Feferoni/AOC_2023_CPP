#include <benchmark/benchmark.h>

#include "common/inc/SolutionConcept.h"
#include "solutions/inc/day1.h"
#include "solutions/inc/day2.h"
#include "solutions/inc/day3.h"
#include "solutions/inc/day4.h"
#include "solutions/inc/day5.h"
#include "solutions/inc/day6.h"
#include "solutions/inc/day7.h"
#include "solutions/inc/day8.h"
#include "solutions/inc/day9.h"

template <Solution Day>
static void Part1(benchmark::State& state) {
    auto solution = Day::getInstance();
    for (auto _ : state) {
        auto result = solution->part1();
    }
}

template <Solution Day>
static void Part2(benchmark::State& state) {
    auto solution = Day::getInstance();
    for (auto _ : state) {
        auto result = solution->part2();
    }
}

BENCHMARK(Part1<Day1>);
BENCHMARK(Part2<Day1>);
BENCHMARK(Part1<Day2>);
BENCHMARK(Part2<Day2>);
BENCHMARK(Part1<Day3>);
BENCHMARK(Part2<Day3>);
BENCHMARK(Part1<Day4>);
BENCHMARK(Part2<Day4>);
BENCHMARK(Part1<Day5>);
BENCHMARK(Part2<Day5>);
BENCHMARK(Part1<Day6>);
BENCHMARK(Part2<Day6>);
BENCHMARK(Part1<Day7>);
BENCHMARK(Part2<Day7>);
BENCHMARK(Part1<Day8>);
BENCHMARK(Part2<Day8>);
BENCHMARK(Part1<Day9>);
BENCHMARK(Part2<Day9>);

BENCHMARK_MAIN();
