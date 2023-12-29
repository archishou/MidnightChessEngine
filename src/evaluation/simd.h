#pragma once

enum class SimdArchType {
	ARM_NEON,
	AVX2,
	NONE
};

#if defined(__ARM_NEON)
#include <arm_neon.h>
static constexpr auto arch_type = SimdArchType::ARM_NEON;
static constexpr usize REGISTER_WIDTH = 8;

#elif defined(__AVX2__)
#include <immintrin.h>
static constexpr auto arch_type = SimdArchType::AVX2;
static constexpr usize REGISTER_WIDTH = 16;

#else
static constexpr auto arch_type = SimdArchType::NONE;
#endif

auto load_register([[maybe_unused]] auto value) {
#if defined(__ARM_NEON)
	return vld1q_s16(value);
#elif defined(__AVX2__)
	return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(value));
#else
	return 0;
#endif
}

auto store([[maybe_unused]] auto value, [[maybe_unused]] auto simd_register) {
#if defined(__ARM_NEON)
	vst1q_s16(value, simd_register);
#elif defined(__AVX2__)
	_mm256_store_si256((__m256i*)value, simd_register);
#endif
}

auto vec_add([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__ARM_NEON)
	return vaddq_s16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_add_epi16(vec1, vec2);
#else
	return 0;
#endif
}

auto vec_sub([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__ARM_NEON)
	return vsubq_s16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_sub_epi16(vec1, vec2);
#else
	return 0;
#endif
}

auto vec_mul([[maybe_unused]] auto vec1, [[maybe_unused]] auto vec2) {
#if defined(__ARM_NEON)
	return vmulq_s16(vec1, vec2);
#elif defined(__AVX2__)
	return _mm256_mullo_epi16(vec1, vec2);
#else
	return 0;
#endif
}

auto vec_clamp([[maybe_unused]] auto min, [[maybe_unused]] auto max, [[maybe_unused]] auto vec) {
#if defined(__ARM_NEON)
	vec = vminq_s16(vdupq_n_s16(max), vec);
	vec = vmaxq_s16(vdupq_n_s16(min), vec);
	return vec;
#elif defined(__AVX2__)
	vec = _mm256_min_epi16(_mm256_set1_epi16(max), vec);
	vec = _mm256_max_epi16(_mm256_set1_epi16(min), vec);
	return vec;
#else
	return 0;
#endif
}

// Adds
auto vec_horizontal_add([[maybe_unused]] auto vec) {
#if defined(__ARM_NEON)
	auto result32 = vpaddlq_s16(vec);
	return vaddvq_s32(result32);
#elif defined(__AVX2__)
	auto lower128_16bit = _mm256_castsi256_si128(vec);      // 8 numbers
    auto upper128_16bit = _mm256_extractf128_si256(vec, 1); // 8 numbers
    auto lower8_32bit = _mm256_cvtepi16_epi32(lower128_16bit);  // sign extend to 32 bits
    auto upper8_32bit = _mm256_cvtepi16_epi32(upper128_16bit); // sign extend to 32 bits

    auto sum8 = _mm256_add_epi32(lower8_32bit, upper8_32bit); // add a and b // 8 numbers
    auto sum4 = _mm256_hadd_epi32(sum8, sum8); // 4 numbers
    auto sum2 = _mm256_hadd_epi32(sum4, sum4); // 2 numbers

    auto lower_number = _mm256_castsi256_si128(sum2);
    auto higher_number =  _mm256_extractf128_si256(sum2, 1);
    auto result = _mm_add_epi32(lower_number, higher_number);
    return _mm_extract_epi32(result, 0);
#else
	return 0;
#endif
}
