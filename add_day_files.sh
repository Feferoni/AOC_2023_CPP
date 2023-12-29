#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]] || [ "$1" -lt 1 ] || [ "$1" -gt 24 ]; then
    echo "Error: Argument must be a number between 1 and 24"
    exit 1
fi

day_number=$1
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

source_impl_file_path=${script_dir}/solutions/src/day${day_number}Impl.cpp
if ! [ -f "$source_impl_file_path" ]; then
    cat <<-EOF > "$source_impl_file_path"
#include "day${day_number}Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

auto Day${day_number}::getInstance() -> std::unique_ptr<Day${day_number}> {
    return std::make_unique<Day${day_number}>();
}

auto Day${day_number}Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day${day_number}>(std::source_location::current());
    return "";
};

auto Day${day_number}Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day${day_number}>(std::source_location::current());
    return "";
};
EOF
fi

source_impl_header_path=${script_dir}/solutions/src/day${day_number}Impl.h
if ! [ -f "$source_impl_header_path" ]; then
    cat <<-EOF > "$source_impl_header_path"
#pragma once

#include <string>

#include "../inc/day${day_number}"

class Day${day_number}Impl : public Day${day_number}
{
public:
    [[nodiscard]] auto part1() -> std::string override;
    [[nodiscard]] auto part2() -> std::string override;
};
EOF
fi

source_header_path=${script_dir}/solutions/inc/day${day_number}.h
if ! [ -f "$source_header_path" ]; then
    cat <<-EOF > "$source_header_path"
#pragma once

#include <string>
#include <memory>

class Day${day_number}
{
public:
    virtual ~Day${day_number}() = default;
    [[nodiscard]] static std::unique_ptr<Day${day_number}> getInstance();
    [[nodiscard]] virtual auto part1() -> std::string = 0;
    [[nodiscard]] virtual auto part2() -> std::string = 0;
};
EOF
fi

input_file_path=${script_dir}/input/Day${day_number}.txt
if ! [ -f "$input_file_path" ]; then
    touch $input_file_path
fi

input_part1_file_path=${script_dir}/input/Day${day_number}_part1_example.txt
if ! [ -f "$input_part1_file_path" ]; then
    touch $input_part1_file_path
fi

input_part2_file_path=${script_dir}/input/Day${day_number}_part2_example.txt
if ! [ -f "$input_part2_file_path" ]; then
    touch $input_part2_file_path
fi

