cmake_minimum_required(VERSION 3.12)
project(common_src)

set(CMAKE_CXX_STANDARD 23)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/../common/*.cpp")

set(SOURCE_FILES $(COMMON_SOURCES))
