add_library(input STATIC
    CharReaderBase.cpp
    FileCharReader.cpp
    StringCharReader.cpp
)

target_include_directories(input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
