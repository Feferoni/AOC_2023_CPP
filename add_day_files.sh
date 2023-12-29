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

source_file_path=${script_dir}/solutions/day${day_number}.cpp
if ! [ -f "$source_file_path" ]; then
    cat <<-EOF > "$source_file_path"
#include "day${day_number}.h"
#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

auto Day${day_number}::part1() -> std::string {
    const auto input = helper::input::getInput<Day${day_number}>(std::source_location::current());
    return "";
};

auto Day${day_number}::part2() -> std::string {
    const auto input = helper::input::getInput<Day${day_number}>(std::source_location::current());
    return "";
};
EOF
fi


source_header_path=${script_dir}/solutions/day${day_number}.h
if ! [ -f "$source_header_path" ]; then
    cat <<-EOF > "$source_header_path"
#pragma once

#include <string>

class Day${day_number}
{
public:
    [[nodiscard]] static auto part1() -> std::string;
    [[nodiscard]] static auto part2() -> std::string;
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

