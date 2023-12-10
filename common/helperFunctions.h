#pragma once

#include <source_location>
#include <vector>
#include <string>
#include <memory>
#include <cxxabi.h>

struct IsTest {
    static bool isTest;
};

template <typename T>
auto demangle() -> std::string {
    int status = -1;
    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
            std::free};
    return (status == 0) ? res.get() : "unknown";
}

auto getFilePath(const std::source_location& location, const std::string &day) -> std::string;

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string>;

template <class Day>
auto getInput(const std::source_location location) -> std::vector<std::string> {
  const auto filePath = getFilePath(location, demangle<Day>());
  return getInputFromFile(filePath);
}

