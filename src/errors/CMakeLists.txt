add_library(errorslib STATIC
    ErrorHandler.cpp
)

target_link_libraries(errorslib PUBLIC 
    inputlib
)

target_include_directories(errorslib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
