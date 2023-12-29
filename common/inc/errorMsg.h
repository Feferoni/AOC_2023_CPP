#pragma once

#include <source_location>
#include <sstream>  // IWYU pragma: keep
#include <string>

[[maybe_unused]] auto errorMsgImpl(const std::string& errorMessage, const std::source_location& src_loc) -> void;

#define ERROR_MSG_AND_EXIT(message)                               \
    do {                                                          \
        std::ostringstream oss;                                   \
        oss << message;                                           \
        errorMsgImpl(oss.str(), std::source_location::current()); \
        std::abort();                                             \
    } while (0)
