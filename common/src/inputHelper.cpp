#include <filesystem>
#include <fstream>
#include <regex>

#include <cxxabi.h>

#include "../inc/inputHelper.h"

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
    ERROR_MSG_AND_EXIT("Could not extract a day from the function: " << functionSignature << " with regex: " << functionNamePatternString);
}
}  // namespace
namespace helper::input {
auto getFilePath(const std::source_location& location, const std::string& day) -> std::string {
    [[maybe_unused]] const auto part = extractDayFromfunctionName(location.function_name());
    const auto currentPath = std::string(std::filesystem::current_path());

#ifdef TEST
    return currentPath + "/../input/" + day + "_" + part + "_example.txt";
#else
    return currentPath + "/../input/" + day + ".txt";
#endif
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
        ERROR_MSG_AND_EXIT("Unable to open file: " << filePath);
    }

    return lines;
}

}  // namespace helper::input
