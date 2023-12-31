//
// Created by Archishmaan Peyyety on 7/8/23.
//

#include "evaluate.h"
#ifdef _MSC_VER
#undef _MSC_VER
#endif
#define INCBIN_SILENCE_BITCODE_WARNING
#include "../3rd_party/incbin.h"
INCBIN(nnue, "/Users/archishmaan/Documents/CodeProjects/chess-engine/src/evaluation/netM006.nnue");
const NNUEParams &nnue_params = *reinterpret_cast<const NNUEParams *>(gnnueData);

std::pair<usize, usize> NNUE::index_of(Piece piece, Square square) {
	constexpr usize color_stride = NSQUARES * 6;
	constexpr usize piece_stride = NSQUARES;

	const auto base = type_of(piece);
	const auto color = color_of(piece);

	const auto white_idx =  color * color_stride + base * piece_stride + static_cast<usize>(square);
	const auto black_idx = !color * color_stride + base * piece_stride + static_cast<usize>(flip(square));

	return {white_idx, black_idx};
}

i32 NNUE::screlu_flatten_norm(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
							 const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
							 const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights) {
	i32 sum = 0;

	for (usize i = 0; i < HIDDEN_LAYER1_SIZE; ++i) {
		sum += screlu(us[i]) * weights[i];
		sum += screlu(them[i]) * weights[HIDDEN_LAYER1_SIZE + i];
	}

	return sum / QA;
}

i32 NNUE::screlu_flatten_simd(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
							 const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
							 const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights) {
	auto sum = veci32_zero();

	auto manual_sum = 0;
	for (usize i = 0; i < HIDDEN_LAYER1_SIZE; i += REGISTER_WIDTH) {
		auto simd_input = loadi16_register(&us[i]);
		simd_input = veci16_clamp(simd_input, 0, QA);
		simd_input = veci16_mul(simd_input, simd_input);
		auto simd_weigh = loadi16_register(&weights[i]);
		auto product = veci16_mul_pair_accumi32(simd_input, simd_weigh);
		sum = veci32_add(sum, product);

		i32 us_sum = 0;
		for (usize nidx = i; nidx < i + REGISTER_WIDTH; nidx++) {
			us_sum += screlu(us[nidx]) * weights[nidx];
		}
		assert(us_sum == veci32_horizontal_add(product));

		simd_input = loadi16_register(&them[i]);
		simd_input = veci16_clamp(simd_input, 0, QA);
		simd_input = veci16_mul(simd_input, simd_input);
		simd_weigh = loadi16_register(&weights[i + HIDDEN_LAYER1_SIZE]);
		product = veci16_mul_pair_accumi32(simd_input, simd_weigh);
		sum = veci32_add(sum, product);

		i32 them_sum = 0;
		for (usize nidx = i; nidx < i + REGISTER_WIDTH; nidx++) {
			them_sum += screlu(them[nidx]) * weights[HIDDEN_LAYER1_SIZE + nidx];
		}
		assert(them_sum == veci32_horizontal_add(product));

		manual_sum += us_sum + them_sum;
		assert(manual_sum == veci32_horizontal_add(sum));
	}

	assert(manual_sum == veci32_horizontal_add(sum));
	return veci32_horizontal_add(sum);
}

i32 NNUE::screlu_flatten(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
						const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
						const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights) {
	if constexpr (arch_type == SimdArchType::NONE) {
		return screlu_flatten_norm(us, them, weights);
	}
	assert(screlu_flatten_norm(us, them, weights) == screlu_flatten_simd(us, them, weights));
	return screlu_flatten_simd(us, them, weights);
}
