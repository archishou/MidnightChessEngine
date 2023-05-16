//
// Created by Archishmaan Peyyety on 5/16/23.
//

#include "lib/doctests.h"
#include "string"
#include "uci_interpreter/bench_fens.h"
#include "board/position.h"
#include "evaluation/evaluate.h"

TEST_SUITE_BEGIN("bench-eval");

constexpr i32 scores[] = {
		83,
		-24,
		196,
		-745,
		24,
		225,
		15,
		56,
		-145,
		-470,
		69,
		-63,
		95,
		-127,
		-60,
		126,
		102,
		114,
		66,
		9,
		-331,
		75,
		85,
		-161,
		-142,
		131,
		48,
		40,
		53,
		-157,
		117,
		-188,
		-79,
		-133,
		169,
		-212,
		-265,
		356,
		-129,
		28,
		74,
		-688,
		119,
		133,
		-67,
		81,
		6,
		-85,
		197,
		174,
};

TEST_CASE("bench-evals") {
	Position p;
	usize idx = 0;
	for (std::string fen : BENCH_FENS) {
		p.set_fen(fen);
		i32 score{};
		if (p.turn() == WHITE) score = evaluate<WHITE>(p);
		else score = evaluate<BLACK>(p);
		CHECK_EQ(scores[idx], score);
		idx += 1;
	}
}

TEST_SUITE_END();