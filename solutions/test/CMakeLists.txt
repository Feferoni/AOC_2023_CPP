cmake_minimum_required(VERSION 3.12)
project(solutions_test)

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

file(GLOB SRC_FILES CONFIGURE_DEPENDS "../src/*.cpp")
file(GLOB TEST_SRC_FILES CONFIGURE_DEPENDS "./*.cpp")

file(GLOB SRC_FILES_COMMON CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/common/src/*.cpp")

include_directories(
    ../src
    ../inc
)

set(SOURCE_FILES
    ${SRC_FILES}
    ${TEST_SRC_FILES}
    ${SRC_FILES_COMMON}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_compile_options(${PROJECT_NAME} PRIVATE -O0 -DTEST)
target_link_options(${PROJECT_NAME} PRIVATE)
include(CTest)
catch_discover_tests(
    ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(${PROJECT_NAME}_sanitized ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_sanitized PRIVATE Catch2::Catch2WithMain)
target_compile_options(${PROJECT_NAME}_sanitized PRIVATE -O0 -DTEST -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(${PROJECT_NAME}_sanitized PRIVATE -fsanitize=address,undefined)
include(CTest)
catch_discover_tests(
    ${PROJECT_NAME}_sanitized
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
