#include "gtest/gtest.h"


class DateConverterFixture : public ::testing::Test {

protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

};

TEST_F(DateConverterFixture, FirstDayEver){ // 1/1/1 -> 1/3/1
    EXPECT_EQ(1, 1);
}

TEST_F(DateConverterFixture, PlusOneDiff){ // 3/1/100 -> 3/2/100
	EXPECT_EQ(1, 1);
}

TEST_F(DateConverterFixture, MinusOne_Leap){ // 3/1/300 -> 2/29/300
	EXPECT_EQ(1, 1);
}


TEST_F(DateConverterFixture, PlusFour_Leap){ // 3/1/900 -> 2/25/900

	EXPECT_EQ(1, 1);
}
