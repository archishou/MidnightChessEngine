project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(ChessEngine_tests_run MoveGenerationTests/MoveGenerationPerft.cpp)

target_link_libraries(ChessEngine_tests_run ChessEngine_lib)

target_link_libraries(ChessEngine_tests_run gtest gtest_main)