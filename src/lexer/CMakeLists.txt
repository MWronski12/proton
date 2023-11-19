add_library(lexer STATIC
    CharReader.cpp
    Lexer.cpp
)

target_include_directories(lexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
