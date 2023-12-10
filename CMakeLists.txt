cmake_minimum_required(VERSION 3.10)
project(AdventOfCode2023)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

target_compile_options(${PROJECT_NAME} PRIVATE -O3)

enable_testing()
