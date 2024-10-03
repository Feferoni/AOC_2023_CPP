cmake_minimum_required(VERSION 3.12)
project(AdventOfCode2023)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dep_cache)

# find_package(benchmark QUIET)
# if(NOT benchmark_FOUND)
#     include(FetchContent)
#     FetchContent_Declare(
#         googlebenchmark
#         GIT_REPOSITORY https://github.com/google/benchmark.git
#         GIT_TAG        v1.8.3
#     )
#     set(BENCHMARK_ENABLE_TESTING OFF) # Disable testing of the benchmark itself
#     set(BENCHMARK_ENABLE_EXCEPTIONS OFF) # Optional: Disable exceptions
#     set(BENCHMARK_ENABLE_LTO OFF) # Optional: Disable Link Time Optimization
#     FetchContent_MakeAvailable(googlebenchmark)
# else()
#     message(STATUS "Using local Google Benchmark")
# endif()

# find_package(Catch2 3 REQUIRED)
# if(NOT Catch2_FOUND)
#     include(FetchContent)
#     FetchContent_Declare(
#       catch2
#       SYSTEM
#       GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#       GIT_TAG v3.4.0
#     )
#     FetchContent_MakeAvailable(catch2)
# else()
#     message(STATUS "Using local catch2")
# endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug) # Debug, Release, RelWithDebInfo, MinSizeRel

add_compile_options(-Wall -Wpedantic -Wextra -Werror -fno-omit-frame-pointer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB SOLUTION_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/solutions/src/*.cpp")
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/common/src/*.cpp")

set(SOURCE_FILES
    main/src/main.cpp
    ${COMMON_SOURCES}
    ${SOLUTION_SOURCES}
)

add_subdirectory(tests)
add_subdirectory(solutions)
add_subdirectory(common)
add_subdirectory(benchmark)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(COMPILER_FLAGS -O3)
set(IGNORE_FLAGS)

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_FLAGS} ${IGNORE_FLAGS})

enable_testing()



