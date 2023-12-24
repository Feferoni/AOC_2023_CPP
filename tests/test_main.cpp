#define CATCH_CONFIG_MAIN
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "intervalHelper.h"
#include "fileHelper.h"

#include "catch2/catch_all.hpp"  // IWYU pragma: keep

TEST_CASE("Range_hasOverlap", "hasOverlap") {
    helper::interval::Range r1{1, 10};
    helper::interval::Range r2{11, 12};
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

TEST_CASE("IsAdjecent", "IsAdjecent") {
    helper::interval::Range r1{1, 10};
    helper::interval::Range r2{11, 12};
    REQUIRE(r1.isAdjecentTo(r2) == true);
    REQUIRE(r2.isAdjecentTo(r1) == true);

    r1 = {1, 10};
    r2 = {10, 12};
    REQUIRE(r1.isAdjecentTo(r2) == false);
    REQUIRE(r2.isAdjecentTo(r1) == false);
}

TEST_CASE("intersectLeftRight", "LeftRight") {
    helper::interval::Range r1{5, 20};
    helper::interval::Range r2{18, 22};
    REQUIRE(r1.rightIntersctionWith(r2) == true);
    REQUIRE(r2.rightIntersctionWith(r1) == false);
    REQUIRE(r1.leftIntersctionWith(r2) == false);
    REQUIRE(r2.leftIntersctionWith(r1) == true);

    r1 = {5, 10};
    r2 = {3, 7};
    REQUIRE(r1.rightIntersctionWith(r2) == false);
    REQUIRE(r2.rightIntersctionWith(r1) == true);
    REQUIRE(r1.leftIntersctionWith(r2) == true);
    REQUIRE(r2.leftIntersctionWith(r1) == false);

    r1 = {3, 3};
    r2 = {3, 7};
    REQUIRE(r1.rightIntersctionWith(r2) == false);
    REQUIRE(r2.rightIntersctionWith(r1) == false);
    REQUIRE(r1.leftIntersctionWith(r2) == false);
    REQUIRE(r2.leftIntersctionWith(r1) == false);

    r1 = {0, 10};
    r2 = {3, 7};
    REQUIRE(r1.rightIntersctionWith(r2) == false);
    REQUIRE(r2.rightIntersctionWith(r1) == false);
    REQUIRE(r1.leftIntersctionWith(r2) == false);
    REQUIRE(r2.leftIntersctionWith(r1) == false);
}

TEST_CASE("IsSubOrSuper", "SubOrSuper") {
    helper::interval::Range r1{1, 20};
    helper::interval::Range r2{1, 12};
    REQUIRE(r1.isSuperSetTo(r2) == true);
    REQUIRE(r2.isSuperSetTo(r1) == false);
    REQUIRE(r1.isSubSetOf(r2) == false);
    REQUIRE(r2.isSubSetOf(r1) == true);

    r1 = {1, 20};
    r2 = {2, 20};
    REQUIRE(r1.isSuperSetTo(r2) == true);
    REQUIRE(r2.isSuperSetTo(r1) == false);
    REQUIRE(r1.isSubSetOf(r2) == false);
    REQUIRE(r2.isSubSetOf(r1) == true);

    r1 = {1, 20};
    r2 = {20, 22};
    REQUIRE(r1.isSuperSetTo(r2) == false);
    REQUIRE(r2.isSuperSetTo(r1) == false);
    REQUIRE(r1.isSubSetOf(r2) == false);
    REQUIRE(r2.isSubSetOf(r1) == false);

    r1 = {10, 20};
    r2 = {9, 10};
    REQUIRE(r1.isSuperSetTo(r2) == false);
    REQUIRE(r2.isSuperSetTo(r1) == false);
    REQUIRE(r1.isSubSetOf(r2) == false);
    REQUIRE(r2.isSubSetOf(r1) == false);
}


TEST_CASE("Range_merge", "Merge") {
    helper::interval::Range r1{1, 10};
    helper::interval::Range r2{12, 20};
    REQUIRE(r1.mergeRanges(r2) == std::nullopt);
    REQUIRE(r2.mergeRanges(r1) == std::nullopt);

    r1 = {1, 10};
    r2 = {11, 20};
    REQUIRE(r1.mergeRanges(r2) == helper::interval::Range{1,20});
    REQUIRE(r2.mergeRanges(r1) == helper::interval::Range{1,20});

    r1 = {1, 10};
    r2 = {10, 20};
    REQUIRE(r1.mergeRanges(r2).value() == helper::interval::Range{1, 20});
    REQUIRE(r2.mergeRanges(r1).value() == helper::interval::Range{1, 20});

    r1 = {1, 10};
    r2 = {5, 6};
    REQUIRE(r1.mergeRanges(r2).value() == helper::interval::Range{1, 10});
    REQUIRE(r2.mergeRanges(r1).value() == helper::interval::Range{1, 10});
}

TEST_CASE("ConversionRangeGetConvertedValue", "ConversionRange") {
    helper::interval::Range from{56, 92};
    helper::interval::Range to{60, 96};
    helper::interval::ConversionRange cr{from, to};

    REQUIRE(cr.getConvertValue(78) == 82);
}

TEST_CASE("ConversionRangeGetConvertedRange", "ConversionRange") {
    helper::interval::Range from{10, 20};
    helper::interval::Range to{30, 40};
    helper::interval::ConversionRange cr{from, to};

    helper::interval::Range toBeConverted{18, 100};

    auto convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == true);
    REQUIRE(convertedRange.value() == helper::interval::Range{38, 40});

    toBeConverted = {0, 12};
    convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == true);
    REQUIRE(convertedRange.value() == helper::interval::Range{30, 32});

    toBeConverted = {10, 20};
    convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == true);
    REQUIRE(convertedRange.value() == helper::interval::Range{30, 40});

    toBeConverted = {0, 100};
    convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == true);
    REQUIRE(convertedRange.value() == helper::interval::Range{30, 40});

    toBeConverted = {50, 100};
    convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == false);

    toBeConverted = {0, 9};
    convertedRange = cr.getConvertedRange(toBeConverted);
    REQUIRE(convertedRange.has_value() == false);
}

TEST_CASE("MergeAdjecentRanges", "Merging") {
    helper::interval::Range r0{1, 3};
    helper::interval::Range r1{5, 10};
    helper::interval::Range r2{10, 15};
    helper::interval::Range r3{10, 25};
    helper::interval::Range r4{30, 35};
    helper::interval::Range r5{36, 40};

    std::vector<helper::interval::Range> rangesBeforeMerge = {r4, r3, r2, r1, r0, r5};
    const auto rangesAfterMerge = helper::interval::mergeAdjecentRanges(rangesBeforeMerge);

    std::cout << "Before: " << rangesBeforeMerge << '\n';
    std::cout << "After: " << rangesAfterMerge << '\n';

    REQUIRE(rangesAfterMerge.size() == 3);
    REQUIRE(rangesAfterMerge.at(0) == helper::interval::Range{1,3});
    REQUIRE(rangesAfterMerge.at(1) == helper::interval::Range{5,25});
    REQUIRE(rangesAfterMerge.at(2) == helper::interval::Range{30,40});
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

