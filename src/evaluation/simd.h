#pragma once

enum class SimdArchType {
	ARM_NEON,
	AVX2,
	AVX512,
	NONE
};

#if defined(__AVX512F__)
#include <immintrin.h>
static constexpr auto arch_type = SimdArchType::AVX512;
static constexpr usize REGISTER_WIDTH = 32;

#elif defined(__AVX2__)
#include <immintrin.h>
static constexpr auto arch_type = SimdArchType::AVX2;
static constexpr usize REGISTER_WIDTH = 16;

#elif defined(__ARM_NEON)
#include <arm_neon.h>
static constexpr auto arch_type = SimdArchType::NONE;
static constexpr usize REGISTER_WIDTH = 8;

#else
static constexpr auto arch_type = SimdArchType::NONE;
static constexpr usize REGISTER_WIDTH = 0;
#endif

auto inline loadi16_register([[maybe_unused]] auto value) {
#if defined(__AVX512F__)
	return _mm512_loadu_si512(reinterpret_cast<const __m512i*>(value));
#elif defined(__AVX2__)
	return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(value));
#elif defined(__ARM_NEON)
	return vld1q_s16(value);
#else
	return 0;
#endif
}

auto inline store_veci16([[maybe_unused]] auto value, [[maybe_unused]] auto simd_register) {
#if defined(__AVX512F__)
	_mm512_store_si512((__m512i*)value, simd_register);
#elif defined(__AVX2__)
	_mm256_store_si256((__m256i*)value, simd_register);
#elif defined(__ARM_NEON)
	vst1q_s16(value, simd_register);
#endif
}

auto inline veci32_zero() {
#if defined(__AVX512F__)
	return _mm512_setzero_si512();
#elif defined(__AVX2__)
	return _mm256_setzero_si256();
#elif defined(__ARM_NEON)
	return vdupq_n_s32(0);
#else
	return 0;
#endif
}

auto inline veci16_add([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__AVX512F__)
	return _mm512_add_epi16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_add_epi16(vec1, vec2);
#elif defined(__ARM_NEON)
	return vaddq_s16(vec1, vec2);
#else
	return 0;
#endif
}

auto inline veci32_add([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__AVX512F__)
	return _mm512_add_epi32(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_add_epi32(vec1, vec2);
#elif defined(__ARM_NEON)
	return vaddq_s32(vec1, vec2);
#else
	return 0;
#endif
}

auto inline veci16_sub([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__AVX512F__)
	return _mm512_sub_epi16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_sub_epi16(vec1, vec2);
#elif defined(__ARM_NEON)
	return vsubq_s16(vec1, vec2);
#else
	return 0;
#endif
}

auto inline veci16_mul([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__AVX512F__)
	return _mm512_mullo_epi16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_mullo_epi16(vec1, vec2);
#elif defined(__ARM_NEON)
	return vmulq_s16(vec1, vec2);
#else
	return 0;
#endif
}

auto inline veci16_clamp([[maybe_unused]] auto vec, [[maybe_unused]] auto min, [[maybe_unused]] auto max) {
#if defined(__AVX512F__)
	vec = _mm512_min_epi16(_mm512_set1_epi16(max), vec);
	vec = _mm512_max_epi16(_mm512_set1_epi16(min), vec);
	return vec;
#elif defined(__AVX2__)
	vec = _mm256_min_epi16(_mm256_set1_epi16(max), vec);
	vec = _mm256_max_epi16(_mm256_set1_epi16(min), vec);
	return vec;
#elif defined(__ARM_NEON)
	vec = vminq_s16(vdupq_n_s16(max), vec);
	vec = vmaxq_s16(vdupq_n_s16(min), vec);
	return vec;
#else
	return 0;
#endif
}

// Adds
auto inline veci32_horizontal_add([[maybe_unused]] auto vec) {
#if defined(__AVX512F__)
	auto low   = _mm512_castsi512_si256(vec);
	auto high  = _mm512_extracti32x8_epi32(vec, 1);
	auto sum8 = _mm256_add_epi32(low, high);
	auto sum4 = _mm256_hadd_epi32(sum8, sum8); // 4 numbers
	auto sum2 = _mm256_hadd_epi32(sum4, sum4); // 2 numbers

	auto lower_number = _mm256_castsi256_si128(sum2);
	auto higher_number =  _mm256_extractf128_si256(sum2, 1);
	auto result = _mm_add_epi32(lower_number, higher_number);
	return _mm_extract_epi32(result, 0);
#elif defined(__AVX2__)
	auto sum4 = _mm256_hadd_epi32(vec, vec); // 4 numbers
	auto sum2 = _mm256_hadd_epi32(sum4, sum4); // 2 numbers

	auto lower_number = _mm256_castsi256_si128(sum2);
	auto higher_number =  _mm256_extractf128_si256(sum2, 1);
	auto result = _mm_add_epi32(lower_number, higher_number);
	return _mm_extract_epi32(result, 0);
#elif defined(__ARM_NEON)
	return vaddvq_s32(vec);
#else
	return 0;
#endif
}

auto inline veci16_mul_pair_accumi32([[maybe_unused]] auto vec1,
									  [[maybe_unused]] auto vec2) {
#if defined(__AVX512F__)
	return _mm512_madd_epi16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_madd_epi16(vec1, vec2);
#elif defined(__ARM_NEON)
	auto mul_low = vmull_s16(vget_low_s16(vec1), vget_low_s16(vec2));
	auto mul_high = vmull_s16(vget_high_s16(vec1), vget_high_s16(vec2));
	return veci32_add(mul_low, mul_high);
#else
	return 0;
#endif
}