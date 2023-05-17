project(Google_tests)

add_executable(ChessEngine_tests_run attacks.cpp board-rep.cpp hash.cpp perft.cpp stack.cpp)

target_link_libraries(ChessEngine_tests_run src)