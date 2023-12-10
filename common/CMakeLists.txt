cmake_minimum_required(VERSION 3.10)
project(AdventOfCode2023Tests)

set(CMAKE_CXX_STANDARD 23)

file(GLOB COMMON_SOURCES "${CMAKE_SOURCE_DIR}/../common/*.cpp")

set(SOURCE_FILES $(COMMON_SOURCES))

