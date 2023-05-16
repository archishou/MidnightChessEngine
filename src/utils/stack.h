//
// Created by Archishmaan Peyyety on 5/1/23.
//
#pragma once

#include <array>
#include <algorithm>
#include "../types.h"

// Credit to Polaris by Ciekce for massively inspiring this.

template<typename T, usize Capacity>
class Stack {
public:
	Stack() = default;
	~Stack() = default;

	inline void push(const T& element) {
		data[length++] = element;
	}

	inline void push(const T&& constructed_element) {
		data[length++] = std::move(constructed_element);
	}

	inline T pop() { return data[length-- -1]; }

	[[nodiscard]] inline T peek() const { return data[length - 1]; }
	[[nodiscard]] inline T& top() { return data[length - 1]; }

	inline void clear() {
		for (usize i = 0; i < length; i++) data[i].~T();
		length = 0;
	}

	[[nodiscard]] inline auto size() const { return length; }

	[[nodiscard]] inline auto empty() const { return length == 0; }

	[[nodiscard]] inline auto operator[](usize i) const { return data[i]; }

	[[nodiscard]] inline auto begin() { return data.begin(); }
	[[nodiscard]] inline auto end() { return data.begin() + static_cast<std::ptrdiff_t>(length); }

	[[nodiscard]] inline auto &operator[](usize i) { return data[i]; }

	[[nodiscard]] inline auto begin() const { return data.begin(); }
	[[nodiscard]] inline auto end() const { return data.begin() + static_cast<std::ptrdiff_t>(length); }

private:
	std::array<T, Capacity> data{};
	usize length = 0;
};