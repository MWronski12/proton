add_library(lexerlib STATIC
    Lexer.cpp
    lexer_utils.cpp
)

target_link_libraries(lexerlib PUBLIC
    inputlib
    errorslib
)

target_include_directories(lexerlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
