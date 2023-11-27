#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

find src test \( -name '*.cpp' -o -name '*.hpp' -o -name '*.c' -o -name '*.h' \) \
    -exec clang-format -i {} \;
