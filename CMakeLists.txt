cmake_minimum_required(VERSION 3.0)

project(C0Compiler)

set(CMAKE_C_STANDARD 11)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(MyScanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

set(SOURCE_FILES assembler.c tree.c)

include_directories(Compiler ${CMAKE_CURRENT_BINARY_DIR})
include_directories(Compiler ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable)

add_executable(Compiler ${SOURCE_FILES} ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})
