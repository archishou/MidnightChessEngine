#include "gtest/gtest.h"


TEST(AbsoluteDateTestSuite, ExampleDate){ // 12/2/2020 -> 737761
    EXPECT_EQ(1, 1);
}


TEST(AbsoluteDateTestSuite, IncorrectDate){ // 12/0/2020 -> 0
	EXPECT_EQ(1, 1);
}