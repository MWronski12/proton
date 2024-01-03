add_library(parserlib STATIC
    Parser.cpp
    parser_utils.cpp
)

target_link_libraries(parserlib PUBLIC
    errorslib
    inputlib
    lexerlib
)

target_include_directories(parserlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
