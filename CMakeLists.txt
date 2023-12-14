cmake_minimum_required(VERSION 3.10)
project(AdventOfCode2023)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/cached_dependencies)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug) # Debug, Release, RelWithDebInfo, MinSizeRel

add_compile_options(-Wall -Wpedantic -Wextra -Werror)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/solutions
)

file(GLOB SOLUTION_SOURCES "${CMAKE_SOURCE_DIR}/solutions/*.cpp")
file(GLOB COMMON_SOURCES "${CMAKE_SOURCE_DIR}/common/*.cpp")

set(SOURCE_FILES
    src/main.cpp
    ${COMMON_SOURCES}
    ${SOLUTION_SOURCES}
)

add_subdirectory(tests)
add_subdirectory(solutions)
add_subdirectory(common)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(COMPILER_FLAGS -O3)
set(IGNORE_FLAGS)

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_FLAGS} ${IGNORE_FLAGS})

enable_testing()

add_test(NAME test COMMAND tests)

set(MEMORYCHECK_COMMAND "/usr/bin/valgrind")
set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
add_custom_target(valgrind_test
    COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure --test-action memcheck
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)
