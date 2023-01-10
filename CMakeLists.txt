cmake_minimum_required(VERSION 3.24)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/evaluation)
include_directories(src/move_generation)
include_directories(src/move_search)
add_subdirectory(ChessEngine_tests)

add_executable(ChessEngine
        src/evaluation/evaluate.cpp
        src/evaluation/evaluate.h
        src/move_generation/position.cpp
        src/move_generation/position.h
        src/move_generation/tables.cpp
        src/move_generation/tables.h
        src/move_generation/types.cpp
        src/move_generation/types.h
        src/move_search/search.h
        src/main.cpp src/move_search/alphabeta.h src/move_search/search_params.h src/move_search/id.h src/move_search/move_ordering.h src/uci_interpreter/uci_interpreter.h)
