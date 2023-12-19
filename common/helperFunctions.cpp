#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <cxxabi.h>

#include "helperFunctions.h"

namespace {
constexpr char
    functionNamePatternString[] = "static std::string Day\\d+::(.*)\\(\\)";

// could fetch the class name and function here, but to lazy to fix
auto extractDayFromfunctionName(const std::string& functionSignature) -> std::string {
    std::smatch matches;
    if (std::regex_search(functionSignature, matches, std::regex(functionNamePatternString)) &&
        !matches.empty()) {
        return matches[1];
    }

    std::cerr << "Could not extract a day from the function: " << functionSignature << " with regex: " << functionNamePatternString << '\n';
    std::abort();
}
}  // namespace

auto getFilePath(const std::source_location& location, const std::string& day) -> std::string {
    const auto part = extractDayFromfunctionName(location.function_name());
    const auto currentPath = std::string(std::filesystem::current_path());

    if (IsTest::isTest) {
        return currentPath + "/../input/" + day + "_" + part + "_example.txt";
    } else {
        return currentPath + "/../input/" + day + ".txt";
    }
}

auto getInputFromFile(const std::string& filePath) -> std::vector<std::string> {
    std::vector<std::string> lines;
    std::ifstream            file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filePath << '\n';
        std::abort();
    }

    return lines;
}

[[nodiscard]] auto splitString(const std::string& str, const std::string& delimiter) -> std::vector<std::string> {
    std::vector<std::string> result;
    size_t                        start = 0;
    size_t                        end   = str.find(delimiter);

    while (end != std::string::npos) {
        auto token = std::string(str.data() + start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
        end   = str.find(delimiter, start);
    }

    result.push_back(std::string(str.data() + start, str.length() - start));
    return result;
}


[[nodiscard]] auto splitStrToStrViews(const std::string_view& str, const std::string_view& delimiter) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    size_t                        start = 0;
    size_t                        end   = str.find(delimiter);

    while (end != std::string::npos) {
        std::string_view token = std::string_view(str.data() + start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
        end   = str.find(delimiter, start);
    }

    result.push_back(std::string_view(str.data() + start, str.length() - start));

    return result;
}

[[nodiscard]] auto getNumbersFromString(const std::string& str) -> std::vector<std::uint32_t> {
    const auto splitNumbers = splitString(str, " ");
    std::vector<std::uint32_t> numbers;
    std::ranges::for_each(splitNumbers, [&numbers] (const std::string& str) {
                try {
                    numbers.push_back(std::stoul(str));
                } catch (const std::invalid_argument& ex) {
                    std::cerr << ex.what() << " - " << str << '\n';
                    std::abort();
                } catch (const std::out_of_range& ex) {
                    std::cerr << ex.what() << " - " << str << '\n';
                    std::abort();
                }
            });
    return numbers;
}
