# Create the library from source files
add_library(lib
    # calc.cpp
    # include/calc.h
    ast.cpp
    parser.cpp
    include/ast.h
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
