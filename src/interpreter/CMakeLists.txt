add_library(interpreterlib STATIC
    SemanticAnalyzer.cpp
    Interpreter.cpp
    Environment.cpp
    StackFrame.cpp
    Scope.cpp
    interpreter_utils.cpp
)

target_link_libraries(interpreterlib PUBLIC
    errorslib
    inputlib
    lexerlib
    parserlib
)

target_include_directories(interpreterlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
