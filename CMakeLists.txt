cmake_minimum_required(VERSION 3.5)

project(liblgpp)

set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-std=c++17 -stdlib=libc++ -Wall -Werror -O2 -g)

file(GLOB_RECURSE src src/lgpp/*.cpp)

add_executable(test ${src} src/test.cpp)
target_include_directories(test PUBLIC src/)

add_executable(bench ${src} src/bench.cpp)
target_include_directories(bench PUBLIC src/)

add_library(lgpp STATIC ${src})
target_include_directories(lgpp PUBLIC src/)

file(GLOB inc src/lgpp/*.hpp)
install(FILES ${inc} DESTINATION include/lgpp)

file(GLOB ops_inc src/lgpp/ops/*.hpp)
install(FILES ${ops_inc} DESTINATION include/lgpp/ops)

install(FILES build/liblgpp.a DESTINATION lib)
