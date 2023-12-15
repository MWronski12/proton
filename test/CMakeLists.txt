include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test
  source/input/StringCharReader_test.cpp
  source/lexer/Lexer_test.cpp
  source/lexer/TokenType_test.cpp
  source/parser/ParseVarDef_test.cpp
  source/parser/ParseConstDef_test.cpp
  source/parser/ParseStructDef_test.cpp
  source/parser/ParseVariantDef_test.cpp
  source/parser/ParseFnDef_test.cpp
)

target_include_directories(test PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test PUBLIC
  GTest::gtest_main
  GTest::gmock_main
  
  errorslib
  inputlib
  lexerlib
  parserlib
)

include(GoogleTest)
gtest_discover_tests(test)
