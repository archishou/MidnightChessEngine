cmake_minimum_required(VERSION 3.14)
project(Midnight)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "-fconstexpr-steps=900000000")
else()
    set(CMAKE_CXX_FLAGS "-fconstexpr-ops-limit=900000000")
endif()

set(MIDNIGHT_LIB src/engine.h src/engine.cpp src/utils/helpers.cpp src/utils/clock.h src/utils/clock.cpp src/move_search/types.cpp src/move_search/search.cpp src/move_search/reductions.cpp src/move_search/pvs.cpp src/move_search/tables/transposition_table.cpp src/move_search/tables/history_table.h src/move_search/tables/history_table.cpp src/evaluation/types.h src/board/position.cpp src/board/types/bitboard.cpp src/evaluation/evaluate.h src/evaluation/constants/misc.h src/evaluation/pieces/bishop.h src/evaluation/pieces/king.h src/evaluation/pieces/knight.cpp src/evaluation/pieces/pawn.cpp src/evaluation/pieces/queen.cpp src/evaluation/pieces/rook.cpp src/evaluation/pieces/king.cpp)

add_executable(Midnight src/main.cpp ${MIDNIGHT_LIB})
add_executable(MidnightTune texel-tuner/tune_main.cpp ${MIDNIGHT_LIB})
add_executable(MidnightTests tests/attacks.cpp tests/board-rep.cpp tests/hash.cpp tests/perft.cpp tests/stack.cpp ${MIDNIGHT_LIB})
