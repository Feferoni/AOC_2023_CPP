cmake_minimum_required(VERSION 3.12)
project(AdventOfCode2023)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dep_cache)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug) # Debug, Release, RelWithDebInfo, MinSizeRel

add_compile_options(-Wall -Wpedantic -Wextra -Werror)

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



