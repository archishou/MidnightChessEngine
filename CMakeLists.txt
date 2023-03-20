cmake_minimum_required(VERSION 3.14)
project(ChessEngine)
add_compile_options(-O3)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(ChessEngine_run ${SOURCE_FILES})

include_directories(src)
add_subdirectory(src)

target_link_libraries(ChessEngine_run src)

add_subdirectory(tests)
