#!/bin/bash

CODE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
find "$CODE_DIR" -type d -name "build" -prune -o -type f \( -name "*.cpp" -or -name "*.h" -or -name "*.hpp" \) -exec clang-format -i {} +
echo "Code formatting complete."
