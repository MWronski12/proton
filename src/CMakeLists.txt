add_subdirectory(
    lexer
)

add_executable(proton
    main.cpp
)

target_link_libraries(proton PUBLIC
    lexer
)
