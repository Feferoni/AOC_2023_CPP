#include <iostream>

#include "../inc/errorMsg.h"

[[maybe_unused]] auto errorMsgImpl(const std::string& errorMessage, const std::source_location& src_loc) -> void {
    std::cerr << src_loc.file_name() << " - " << src_loc.function_name() << ":"
              << src_loc.line() << " ErrorMsg: " << errorMessage << '\n';
}
