project(Google_tests)

add_executable(ChessEngine_tests_run move-gen-tests/perft.cpp move-gen-tests/move-gen-options.cpp move-gen-tests/zobrist.cpp lib/doctests.h lib/doctests.h main.cpp main.cpp)

target_link_libraries(ChessEngine_tests_run src)