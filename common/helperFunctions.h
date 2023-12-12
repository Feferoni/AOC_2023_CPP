#pragma once

#include <concepts>  // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <vector>

#include <cxxabi.h>

struct IsTest {
    static bool isTest;
};

template <typename T>
auto demangle() -> std::string {
    int  status         = -1;
    auto demangled_name = std::unique_ptr<char, void (*)(void*)>{
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status), std::free};
    return (status == 0) ? demangled_name.get() : "unknown";
}

auto getFilePath(const std::source_location& location, const std::string& day) -> std::string;

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string>;

template <class Day>
auto getInput(const std::source_location location) -> std::vector<std::string> {
    const auto filePath = getFilePath(location, demangle<Day>());
    return getInputFromFile(filePath);
}

template <typename T>
concept IterableContainer = requires(T a) {
    { a.begin() } -> std::input_iterator;
    { a.end() } -> std::input_iterator;
} && !std::same_as<T, std::string> && !std::same_as<T, std::string_view>;

template <IterableContainer T>
std::ostream& operator<<(std::ostream& os, const T& container) {
    os << "[";

    auto it = container.begin();
    if (it != container.end()) {
        os << *it;
        ++it;
    }

    for (; it != container.end(); ++it) {
        os << ", " << *it;
    }

    os << "]";
    return os;
}

[[nodiscard]] auto splitStrToStrView(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view>;
