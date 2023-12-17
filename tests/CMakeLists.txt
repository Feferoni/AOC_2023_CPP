cmake_minimum_required(VERSION 3.10)
project(tests)


include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dep_cache)
FetchContent_Declare(
  catch2
  SYSTEM
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(catch2)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOLUTION_SOURCES "${CMAKE_SOURCE_DIR}/solutions/*.cpp")
file(GLOB COMMON_SOURCES "${CMAKE_SOURCE_DIR}/common/*.cpp")

add_subdirectory(${CMAKE_SOURCE_DIR}/common common_build)
add_subdirectory(${CMAKE_SOURCE_DIR}/solutions solutions_build)

include_directories(
    ${common_build}/
    ${solutions_build}/
)

set(SOURCE_FILES
    ${SOLUTION_SOURCES}
    ${COMMON_SOURCES}
    test_main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

include(Catch)
include(CTest)
catch_discover_tests(
    ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)
