#define CATCH_CONFIG_MAIN
#include "solutions/inc/day1.h"
#include "solutions/inc/day2.h"
#include "solutions/inc/day3.h"
#include "solutions/inc/day4.h"
#include "solutions/inc/day5.h"
#include "solutions/inc/day6.h"
#include "solutions/inc/day7.h"
#include "solutions/inc/day8.h"

#include "catch2/catch_all.hpp"  // IWYU pragma: keep

TEST_CASE("Day1_part1", "part1") {
    REQUIRE(Day1::getInstance()->part1() == "142");
}

TEST_CASE("Day1_part2", "part2") {
    REQUIRE(Day1::getInstance()->part2() == "281");
}

TEST_CASE("Day2_part1", "part1") {
    REQUIRE(Day2::getInstance()->part1() == "8");
}

TEST_CASE("Day2_part2", "part2") {
    REQUIRE(Day2::getInstance()->part2() == "2286");
}

TEST_CASE("Day3_part1", "part1") {
    REQUIRE(Day3::getInstance()->part1() == "4361");
}

TEST_CASE("Day3_part2", "part2") {
    REQUIRE(Day3::getInstance()->part2() == "467835");
}

TEST_CASE("Day4_part1", "part1") {
    REQUIRE(Day4::getInstance()->part1() == "13");
}

TEST_CASE("Day4_part2", "part2") {
    REQUIRE(Day4::getInstance()->part2() == "30");
}

TEST_CASE("Day5_part1", "part1") {
    REQUIRE(Day5::getInstance()->part1() == "35");
}

TEST_CASE("Day5_part2", "part2") {
    REQUIRE(Day5::getInstance()->part2() == "46");
}

TEST_CASE("Day6_part1", "part1") {
    REQUIRE(Day6::getInstance()->part1() == "288");
}

TEST_CASE("Day6_part2", "part2") {
    REQUIRE(Day6::getInstance()->part2() == "71503");
}

TEST_CASE("Day7_part1", "part1") {
    REQUIRE(Day7::getInstance()->part1() == "6440");
}

TEST_CASE("Day7_part2", "part2") {
    REQUIRE(Day7::getInstance()->part2() == "5905");
}

TEST_CASE("Day8_part1", "part1") {
    REQUIRE(Day8::getInstance()->part1() == "6");
}

TEST_CASE("Day8_part2", "part2") {
    REQUIRE(Day8::getInstance()->part2() == "6");
}
