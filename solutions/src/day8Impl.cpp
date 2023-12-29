#include "day8Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

auto Day8::getInstance() -> std::unique_ptr<Day8> {
    return std::make_unique<Day8Impl>();
}

auto Day8Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day8>(std::source_location::current());
    return "";
};

auto Day8Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day8>(std::source_location::current());
    return "";
};
