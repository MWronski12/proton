add_subdirectory(errors)
add_subdirectory(input)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantic_analyzer)

add_executable(proton
    main.cpp
)

target_link_libraries(proton PUBLIC
    lexerlib
)
