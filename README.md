# Advent of Code 2023
Doing it in c++23 with catch2 as test fwk.

## Build
To build with Ninja use the following command from repo root:
```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

## Executing
Target code:
```
./AdventOfCode2023
```

Test code:
```
ctest --output-on-failure
```

## Generation
To generate the initial files for a specific day:
```
./add_day_files.sh <day>
```

## Code style
The code style is defined in the .clang-format file. To run clang formatting over all c++ files:
```
./format_all_files.sh
```

