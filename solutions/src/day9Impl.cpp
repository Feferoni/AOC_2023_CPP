#include "day9Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

auto Day9::getInstance() -> std::unique_ptr<Day9> {
    return std::make_unique<Day9Impl>();
}

auto Day9Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day9>(std::source_location::current());
    return "";
};

auto Day9Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day9>(std::source_location::current());
    return "";
};
