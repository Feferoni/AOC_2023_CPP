cmake_minimum_required(VERSION 3.12)
project(tests)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dep_cache)
FetchContent_Declare(
  catch2
  SYSTEM
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1
)

FetchContent_MakeAvailable(catch2)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOLUTION_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/solutions/src/*.cpp")
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/common/src/*.cpp")

add_subdirectory(${CMAKE_SOURCE_DIR}/common/src common_build)
add_subdirectory(${CMAKE_SOURCE_DIR}/solutions/src solutions_build)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

set(SOURCE_FILES
    ${SOLUTION_SOURCES}
    ${COMMON_SOURCES}
    test_main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE -O0 -DTEST)
target_link_options(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
include(Catch)
include(CTest)
catch_discover_tests(
    ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(${PROJECT_NAME}_sanitized ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME}_sanitized PRIVATE -O0 -DTEST -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(${PROJECT_NAME}_sanitized PRIVATE -fsanitize=address,undefined)
target_link_libraries(${PROJECT_NAME}_sanitized PRIVATE Catch2::Catch2WithMain)
include(Catch)
include(CTest)
catch_discover_tests(
    ${PROJECT_NAME}_sanitized
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

