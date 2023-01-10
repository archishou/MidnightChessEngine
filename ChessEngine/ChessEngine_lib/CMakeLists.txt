project(DateConverter)

set(SOURCE_FILES
        uci_interpreter/uci_interpreter.h
        move_search/move_ordering.h
        move_search/search.h
        move_search/search_params.h
        move_search/id.h
        move_search/alphabeta.h
        move_generation/position.cpp
        move_generation/position.h
        move_generation/tables.cpp
        move_generation/tables.h
        move_generation/types.cpp
        move_generation/types.h
        evaluation/evaluate.cpp
        evaluation/evaluate.h
        )
add_library(ChessEngine_lib STATIC ${SOURCE_FILES})