#define CATCH_CONFIG_MAIN
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "helperFunctions.h"

#include "catch2/catch_all.hpp"  // IWYU pragma: keep

bool IsTest::isTest = true;

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
    REQUIRE(Day5::part2() == "");
}

