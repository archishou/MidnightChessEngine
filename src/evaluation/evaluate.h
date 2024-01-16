#pragma once

#include <array>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <span>
#include <vector>
#include "../board/types/square.h"
#include "../board/types/piece.h"
#include "simd.h"

constexpr usize INPUT_LAYER_SIZE = NSQUARES * 12;
constexpr usize HIDDEN_LAYER1_SIZE = 768;

constexpr i32 SCALE = 400;

constexpr i32 QA = 181;
constexpr i32 QB = 64;

constexpr i32 QAB = QA * QB;
constexpr bool ADD_TO_SQUARE = true;
constexpr bool RM_FROM_SQUARE = false;

// 64 byte alignment for avx512 SIMD instruction
struct alignas(64) NNUEParams {
	std::array<i16, INPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE> feature_weights;
	std::array<i16, HIDDEN_LAYER1_SIZE> feature_bias;
	std::array<i16, HIDDEN_LAYER1_SIZE * 2> output_weights;
	i16 output_bias;
};

extern const NNUEParams &nnue_params;

struct alignas(64) LazyHiddenLayer {
	std::array<i16, HIDDEN_LAYER1_SIZE> white;
	std::array<i16, HIDDEN_LAYER1_SIZE> black;

	void init_biases(std::span<const i16, HIDDEN_LAYER1_SIZE> bias) {
		std::memcpy(white.data(), bias.data(), bias.size_bytes());
		std::memcpy(black.data(), bias.data(), bias.size_bytes());
	}
};

constexpr i32 screlu(i16 x) {
	auto clamped = std::clamp(static_cast<i32>(x), 0, QA);
	return clamped * clamped;
}

struct NNUE {
	std::vector<LazyHiddenLayer> m_accumulator_stack{};

	explicit NNUE() {
		m_accumulator_stack.reserve(512);
	}

	~NNUE() = default;

	std::pair<usize, usize> index_of(Piece piece, Square square);
	i32 screlu_flatten(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
					  const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
					  const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights);

	i32 screlu_flatten_simd(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
						   const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
						   const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights);

	i32 screlu_flatten_norm(const std::array<i16, HIDDEN_LAYER1_SIZE> &us,
						   const std::array<i16, HIDDEN_LAYER1_SIZE> &them,
						   const std::array<i16, HIDDEN_LAYER1_SIZE * 2> &weights);

	void reset() {
		m_accumulator_stack.clear();
		m_accumulator_stack.push_back({});
		m_accumulator_stack.back().init_biases(nnue_params.feature_bias);
	}

	void push_copy() {
		auto& m_curr = m_accumulator_stack.back();
		assert(m_accumulator_stack.size() < m_accumulator_stack.capacity());
		m_accumulator_stack.push_back(m_curr);
	}

	void pop() {
		m_accumulator_stack.pop_back();
		assert(!m_accumulator_stack.empty());
	}

	template <bool add_to_square>
	void update_feature(Piece piece, Square square) {
		const auto [white_idx, black_idx] = index_of(piece, square);

		auto& m_curr = m_accumulator_stack.back();
		if constexpr (add_to_square) {
			add_feature(m_curr.white, nnue_params.feature_weights, white_idx * HIDDEN_LAYER1_SIZE);
			add_feature(m_curr.black, nnue_params.feature_weights, black_idx * HIDDEN_LAYER1_SIZE);
		} else {
			remove_feature(m_curr.white, nnue_params.feature_weights, white_idx * HIDDEN_LAYER1_SIZE);
			remove_feature(m_curr.black, nnue_params.feature_weights, black_idx * HIDDEN_LAYER1_SIZE);
		}
	}

	inline void add_feature(std::array<i16, HIDDEN_LAYER1_SIZE> &input,
							const std::array<i16, INPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE> &weights,
							usize offset) {
		if constexpr (arch_type == SimdArchType::NONE) {
			for (usize i = 0; i < input.size(); ++i) {
				input[i] += weights[offset + i];
			}
		} else {
			for (usize i = 0; i < input.size(); i += REGISTER_WIDTH) {
				auto simd_reg_input = loadi16_register(&input[i]);
				auto simd_reg_weigh = loadi16_register(&weights[offset + i]);
				store_veci16(&input[i], veci16_add(simd_reg_input, simd_reg_weigh));
			}
		}
	}

	inline void remove_feature(std::array<i16, HIDDEN_LAYER1_SIZE> &input,
							   const std::array<i16, INPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE> &weights,
							   usize offset) {
		if constexpr (arch_type == SimdArchType::NONE) {
			for (usize i = 0; i < input.size(); ++i) {
				input[i] -= weights[offset + i];
			}
		} else {
			for (usize i = 0; i < input.size(); i += REGISTER_WIDTH) {
				auto simd_reg_input = loadi16_register(&input[i]);
				auto simd_reg_weigh = loadi16_register(&weights[offset + i]);
				store_veci16(&input[i], veci16_sub(simd_reg_input, simd_reg_weigh));
			}
		}
	}

	template<Color color>
	i32 evaluate() {
		auto output = 0;
		const auto& m_curr = m_accumulator_stack.back();
		if constexpr (color == WHITE) {
			output = screlu_flatten(m_curr.white, m_curr.black, nnue_params.output_weights);
		} else {
			output = screlu_flatten(m_curr.black, m_curr.white, nnue_params.output_weights);
		}
		return (output + nnue_params.output_bias) * SCALE / QAB;
	}
};
