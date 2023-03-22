project(Google_tests)

add_executable(ChessEngine_tests_run move-gen-tests/MoveGenerationPerft.cpp move-gen-tests/MoveGenerationOptionsTests.cpp move-gen-tests/MoveGenerationZobristHastTests.cpp lib/doctests.h lib/doctests.h main.cpp main.cpp)

target_link_libraries(ChessEngine_tests_run src)