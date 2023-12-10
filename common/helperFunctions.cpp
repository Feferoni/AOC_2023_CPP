#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <cxxabi.h>

#include "helperFunctions.h"

namespace {
    constexpr char patternString[] = "static std::string Day\\d+::(.*)\\(\\)";
}

// could fetch the class name and function here, but to lazy to fix
std::string extractDayFromfunctionName(const std::string &functionSignature) {
    std::smatch matches;
    if (std::regex_search(functionSignature, matches, std::regex(patternString)) && !matches.empty()) {
        return matches[1];
    }

    std::cerr << "Could not extract a day from the function: " << functionSignature << " with regex: " << patternString
              << '\n';
    std::abort();
}

auto getFilePath(const std::source_location &location, const std::string &day) -> std::string {
    const auto part        = extractDayFromfunctionName(location.function_name());
    const auto currentPath = std::string(std::filesystem::current_path());

    if (IsTest::isTest) {
        return currentPath + "/../../input/" + day + "_" + part + "_example.txt";
    } else {
        return currentPath + "/../input/" + day + ".txt";
    }
}

auto getInputFromFile(const std::string &filePath) -> std::vector<std::string> {
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
