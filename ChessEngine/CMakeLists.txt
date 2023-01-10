cmake_minimum_required(VERSION 3.14)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(ChessEngine_run ${SOURCE_FILES})

include_directories(ChessEngine_lib)
add_subdirectory(ChessEngine_lib)

target_link_libraries(ChessEngine_run ChessEngine_lib)

add_subdirectory(ChessEngine_tests)
