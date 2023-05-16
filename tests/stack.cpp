//
// Created by Archishmaan Peyyety on 5/2/23.
//

#include "lib/doctests.h"
#include "../src/utils/stack.h"

TEST_SUITE_BEGIN("stack");

TEST_CASE("push-pop-peek") {
	constexpr u32 STACK_CAP = 100;
	Stack<i32, STACK_CAP> stack{};

	for (usize i = 0; i < STACK_CAP; i++) {
		stack.push(static_cast<i32>(i));
		CHECK_EQ(stack.peek(), i);
		CHECK_EQ(stack.size(), i + 1);
		CHECK_EQ(stack[i], i);
	}

	for (usize i = 0; i < STACK_CAP; i++) {
		auto popped_element = stack.pop();
		CHECK_EQ(STACK_CAP - i - 1, popped_element);
		CHECK_EQ(stack.size(), STACK_CAP - i - 1);
	}
}

TEST_SUITE_END();