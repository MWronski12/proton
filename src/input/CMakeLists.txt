add_library(inputlib STATIC
    CharReaderBase.cpp
    FileCharReader.cpp
    StringCharReader.cpp
)

target_include_directories(inputlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
