cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    # -Ofast # Enables compiler optimization
    # -fsanitize=leak
    # -fsanitize=address
    # -fsanitize=undefined
)

project(Proton)

add_subdirectory(src)
add_subdirectory(test)
