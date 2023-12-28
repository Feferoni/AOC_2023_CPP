#pragma once

#include <iostream>
#include <source_location>
#include <sstream>  // IWYU pragma: keep
#include <string>

[[maybe_unused]] inline auto errorMsgImpl(const std::string& errorMessage, const std::source_location& src_loc) -> void {
    std::cerr << src_loc.file_name() << " - " << src_loc.function_name() << ":"
              << src_loc.line() << " ErrorMsg: " << errorMessage << '\n';
}

#define ERROR_MSG_AND_EXIT(message)                               \
    do {                                                          \
        std::ostringstream oss;                                   \
        oss << message;                                           \
        errorMsgImpl(oss.str(), std::source_location::current()); \
        std::abort();                                             \
    } while (0)
