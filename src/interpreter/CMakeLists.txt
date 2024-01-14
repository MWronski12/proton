add_library(interpreterlib STATIC
    Interpreter.cpp
    SemanticAnalyzer.cpp
    Environment.cpp
    StackFrame.cpp
    Scope.cpp
    interpreter_utils.cpp
    CastHandler.cpp
    OperatorsHandler.cpp
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
