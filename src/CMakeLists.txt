add_subdirectory(errors)
add_subdirectory(input)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(interpreter)

add_executable(proton
    main.cpp
)

target_link_libraries(proton PUBLIC
    lexerlib
)
