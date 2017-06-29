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

enable_testing()
add_test(1 ../executable/Compiler ../Inputs/1.txt)
add_test(2 ../executable/Compiler ../Inputs/2.txt)
add_test(3 ../executable/Compiler ../Inputs/3.txt)
add_test(4 ../executable/Compiler ../Inputs/4.txt)
add_test(5 ../executable/Compiler ../Inputs/5.txt)
add_test(6 ../executable/Compiler ../Inputs/6.txt)
add_test(7 ../executable/Compiler ../Inputs/7.txt)
add_test(8 ../executable/Compiler ../Inputs/8.txt)
add_test(9 ../executable/Compiler ../Inputs/9.txt)
add_test(10 ../executable/Compiler ../Inputs/10.txt)
add_test(11 ../executable/Compiler ../Inputs/11.txt)
add_test(12 ../executable/Compiler ../Inputs/12.txt)
add_test(13 ../executable/Compiler ../Inputs/13.txt)