add_library(lexerlib STATIC
    Lexer.cpp
    utils.cpp
)

target_link_libraries(lexerlib PUBLIC
    inputlib
    errorslib
)

target_include_directories(lexerlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(lexer
    main.cpp
)

target_link_libraries(lexer PUBLIC
    lexerlib
    inputlib
)
