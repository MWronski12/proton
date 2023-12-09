add_library(parserlib STATIC
    Parser.cpp
)

target_link_libraries(parserlib PUBLIC
    errorslib
    inputlib
    lexerlib
)

target_include_directories(parserlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(parser
    main.cpp
)

target_link_libraries(parser PUBLIC
    errorslib
    inputlib
    lexerlib
    parserlib
)
