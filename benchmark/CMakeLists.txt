cmake_minimum_required(VERSION 3.12)
project(benchmarks)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dep_cache)

find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.8.3
    )
    set(BENCHMARK_ENABLE_TESTING OFF) # Disable testing of the benchmark itself
    set(BENCHMARK_ENABLE_EXCEPTIONS OFF) # Optional: Disable exceptions
    set(BENCHMARK_ENABLE_LTO OFF) # Optional: Disable Link Time Optimization
    FetchContent_MakeAvailable(googlebenchmark)
else()
    message(STATUS "Using local Google Benchmark")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB SOLUTION_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/solutions/src/*.cpp")
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/common/src/*.cpp")

add_subdirectory(${CMAKE_SOURCE_DIR}/common/src common_build)
add_subdirectory(${CMAKE_SOURCE_DIR}/solutions/src solutions_build)

set(SOURCE_FILES
    ${COMMON_SOURCES}
    ${SOLUTION_SOURCES}
    benchmark.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
if(benchmark_FOUND)
    target_link_libraries(${PROJECT_NAME} benchmark)
else()
    target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
endif()

set(COMPILER_FLAGS -O3)
set(IGNORE_FLAGS)

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_FLAGS} ${IGNORE_FLAGS})

