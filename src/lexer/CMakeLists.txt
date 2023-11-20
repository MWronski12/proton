add_library(lexer STATIC
    Lexer.cpp
)

target_link_libraries(lexer PUBLIC
    input
)

target_include_directories(lexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
