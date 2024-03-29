cmake_minimum_required(VERSION 3.12)
project(benchmarks)


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

