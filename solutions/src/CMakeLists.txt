cmake_minimum_required(VERSION 3.12)
project(solutions_src)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOLUTION_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/solutions/src/*.cpp")
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/common/src/*.cpp")

set(SOURCE_FILES
    ${COMMON_SOURCES}
    ${SOLUTION_SOURCES}
)

