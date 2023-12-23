#define CATCH_CONFIG_MAIN
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "helperFunctions.h"

#include "catch2/catch_all.hpp"  // IWYU pragma: keep

bool IsTest::isTest = true;

TEST_CASE("Range_hasOverlap", "hasOverlap") {
    Range r1{1, 10};
    Range r2{11, 12};
    REQUIRE(r1.hasOverlap(r2) == false);
    REQUIRE(r2.hasOverlap(r1) == false);

    r1 = {1, 10};
    r2 = {10, 12};
    REQUIRE(r1.hasOverlap(r2) == true);
    REQUIRE(r2.hasOverlap(r1) == true);

    r1 = {5, 10};
    r2 = {1, 6};
    REQUIRE(r1.hasOverlap(r2) == true);
    REQUIRE(r2.hasOverlap(r1) == true);

    r1 = {1, 10};
    r2 = {2, 6};
    REQUIRE(r1.hasOverlap(r2) == true);
    REQUIRE(r2.hasOverlap(r1) == true);
}

TEST_CASE("Range_merge", "Merge") {
    Range r1{1, 10};
    Range r2{11, 20};
    REQUIRE(r1.mergeRanges(r2) == std::nullopt);
    REQUIRE(r2.mergeRanges(r1) == std::nullopt);

    r1 = {1, 10};
    r2 = {10, 20};
    REQUIRE(r1.mergeRanges(r2).value() == Range{1, 20});
    REQUIRE(r2.mergeRanges(r1).value() == Range{1, 20});

    r1 = {1, 10};
    r2 = {5, 6};
    REQUIRE(r1.mergeRanges(r2).value() == Range{1, 10});
    REQUIRE(r2.mergeRanges(r1).value() == Range{1, 10});
}

TEST_CASE("ConversionRange", "ConversionRange") {
    Range from{56, 92};
    Range to{60, 96};
    ConversionRange cr{from, to};

    REQUIRE(cr.convertValue(78) == 82);
}

TEST_CASE("Day1_part1", "part1") {
    REQUIRE(Day1::part1() == "142");
}

TEST_CASE("Day1_part2", "part2") {
    REQUIRE(Day1::part2() == "281");
}

TEST_CASE("Day2_part1", "part1") {
    REQUIRE(Day2::part1() == "8");
}

TEST_CASE("Day2_part2", "part2") {
    REQUIRE(Day2::part2() == "2286");
}

TEST_CASE("Day3_part1", "part1") {
    REQUIRE(Day3::part1() == "4361");
}

TEST_CASE("Day3_part2", "part2") {
    REQUIRE(Day3::part2() == "467835");
}

TEST_CASE("Day4_part1", "part1") {
    REQUIRE(Day4::part1() == "13");
}

TEST_CASE("Day4_part2", "part2") {
    REQUIRE(Day4::part2() == "30");
}

TEST_CASE("Day5_part1", "part1") {
    REQUIRE(Day5::part1() == "35");
}

TEST_CASE("Day5_part2", "part2") {
    REQUIRE(Day5::part2() == "46");
}

