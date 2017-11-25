cmake_minimum_required(VERSION 3.0)

project(C0Compiler)

set(CMAKE_C_STANDARD 11)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(MyParser parsing/parser.y ${PROJECT_SOURCE_DIR}/parsing/parser.c)
FLEX_TARGET(MyScanner parsing/lexer.l ${PROJECT_SOURCE_DIR}/parsing/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


file(GLOB source_SRC "${PROJECT_SOURCE_DIR}/source/*.h")
file(GLOB source_HDR "${PROJECT_SOURCE_DIR}/source/*.c")

set(SOURCE_FILES ${source_SRC} ${source_HDR} parsing/parser.y parsing/lexer.l)

include_directories(Compiler ${PROJECT_SOURCE_DIR}/parsing)
include_directories(Compiler ${PROJECT_SOURCE_DIR}/source)

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
add_test(overflow ../executable/Compiler ../Inputs/overflow.txt)
