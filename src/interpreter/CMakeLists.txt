add_library(interpreter STATIC
    ScopeChecker.cpp
)

target_link_libraries(interpreter
    errorslib
    inputlib
    lexerlib
    parserlib
)

target_include_directories(interpreter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
