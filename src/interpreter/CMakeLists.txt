add_library(interpreter STATIC
    ScopeAnalyzer.cpp
    SemanticAnalyzer.cpp
    interpreter_utils.cpp
)

target_link_libraries(interpreter PUBLIC
    errorslib
    inputlib
    lexerlib
    parserlib
)

target_include_directories(interpreter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
