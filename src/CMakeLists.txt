project(DateConverter)

set(SOURCE_FILES
        uci_interpreter/uci_interpreter.h
        move_search/move_ordering/move_ordering.h
        move_search/search.h
        move_search/search_params.h
        move_search/pvs.h
        move_generation/position.cpp
        move_generation/position.h
        move_generation/tables.cpp
        move_generation/tables.h
        move_generation/types.cpp
        move_generation/types.h
        evaluation/evaluate.h
        move_search/tables/transposition_table.h move_search/tables/transposition_table.cpp move_search/tables/pv_table.h utils/helpers.cpp utils/helpers.h utils/clock.cpp utils/clock.h utils/fen_constants.h uci_interpreter/parse_uci_move.h move_search/tables/history_table.h move_search/move_ordering/ordering_constants.h uci_interpreter/time_manager.h evaluation/pieces/pawn.h evaluation/types.h evaluation/pieces/knight.h evaluation/pieces/bishop.h evaluation/pieces/rook.h evaluation/pieces/queen.h evaluation/pieces/king.h move_search/tables/history_table.cpp move_search/tables/lmr_table.cpp move_search/tables/lmr_table.h engine.h move_search/move_ordering/move_ordering.cpp engine.cpp uci_interpreter/bench_fens.h move_search/reductions.cpp move_search/reductions.h evaluation/bitboards.h move_search/search_constants.h move_search/types.h evaluation/constants/psts.h evaluation/constants/misc.h)
add_library(src STATIC ${SOURCE_FILES})