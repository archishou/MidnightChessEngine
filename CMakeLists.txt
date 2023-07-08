cmake_minimum_required(VERSION 3.14)
project(Midnight)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "-fconstexpr-steps=900000000")
else()
    set(CMAKE_CXX_FLAGS "-fconstexpr-ops-limit=900000000")
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORTED)
if(LTO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO OFF)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
endif()


set(MIDNIGHT_LIB src/engine.cpp src/utils/helpers.cpp src/utils/clock.cpp src/move_search/types.cpp src/move_search/reductions.cpp src/move_search/pvs.cpp src/move_search/tables/transposition_table.cpp src/move_search/tables/history_table.cpp src/board/position.cpp src/board/types/bitboard.cpp src/evaluation/pieces/knight.cpp src/evaluation/pieces/pawn.cpp src/evaluation/pieces/queen.cpp src/evaluation/pieces/rook.cpp src/evaluation/pieces/king.cpp src/move_search/tables/lmr_table.cpp src/move_search/move_ordering/move_ordering.cpp src/evaluation/pieces/bishop.cpp src/uci_interpreter/time_manager.cpp src/uci_interpreter/uci_move_parse.cpp src/uci_interpreter/uci_interpreter.cpp src/evaluation/evaluate.cpp)

add_executable(Midnight src/main.cpp ${MIDNIGHT_LIB})
add_executable(MidnightTune ${MIDNIGHT_LIB} texel-tuner/tune_main.cpp texel-tuner/tuner.cpp texel-tuner/threadpool.cpp texel-tuner/midnight.cpp)
add_executable(MidnightTests ${MIDNIGHT_LIB} tests/attacks.cpp tests/board-rep.cpp tests/hash.cpp tests/perft.cpp tests/stack.cpp)
