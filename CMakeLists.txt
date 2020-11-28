cmake_minimum_required(VERSION 3.5)

project(liblgpp)

set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-std=c++17 -stdlib=libc++ -Wall -Werror -O2 -g)

file(GLOB_RECURSE src src/lgpp/*.cpp)

add_executable(tests ${src} src/tests.cpp)
target_include_directories(tests PUBLIC src/)

file(GLOB inc src/lgpp/*.hpp)
install(FILES ${inc} DESTINATION include/lgpp)

file(GLOB ops_inc src/lgpp/ops/*.hpp)
install(FILES ${ops_inc} DESTINATION include/lgpp/ops)

install(FILES build/liblgpp.a DESTINATION lib)
