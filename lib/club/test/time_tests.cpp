#include <gtest/gtest.h>
#include <sstream>

#include "time.hpp"

TEST(TimeTests, HoursAndMinutesTest) {
  club::Time time(10, 45);
  EXPECT_EQ(time.hours(), 10);
  EXPECT_EQ(time.minutes(), 45);
}

TEST(TimeTests, CeilTest) {
  club::Time time1(5, 0);
  EXPECT_EQ(time1.ceil(), 5);

  club::Time time2(5, 15);
  EXPECT_EQ(time2.ceil(), 6);

  club::Time time3(23, 59);
  EXPECT_EQ(time3.ceil(), 24);
}

TEST(TimeTests, ComparisonTest) {
  club::Time time1(5, 0);
  club::Time time2(6, 30);
  EXPECT_TRUE(time1 < time2);
  EXPECT_FALSE(time1 < time1);
  EXPECT_FALSE(time1 > time2);
  EXPECT_TRUE(time1 <= time2);
  EXPECT_TRUE(time1 <= time1);
  EXPECT_FALSE(time1 >= time2);
  EXPECT_FALSE(time1 == time2);
  EXPECT_TRUE(time1 != time2);
}

TEST(TimeTests, AdditionTest) {
  club::Time time1(2, 30);
  club::Time time2(1, 45);
  club::Time result = time1 + time2;
  EXPECT_EQ(result.hours(), 4);
  EXPECT_EQ(result.minutes(), 15);

  club::Time day(24, 0);
  club::Time next_day = result + day;
  EXPECT_EQ(next_day.hours(), 4);
  EXPECT_EQ(next_day.minutes(), 15);
  EXPECT_FALSE(result == next_day);
  EXPECT_TRUE(result < next_day);
}

TEST(TimeTests, SubtractionTest) {
  club::Time time1(5, 0);
  club::Time time2(2, 30);
  club::Time result = time1 - time2;
  EXPECT_EQ(result.hours(), 2);
  EXPECT_EQ(result.minutes(), 30);

  club::Time day(24, 0);
  EXPECT_ANY_THROW(result -= day;);
  EXPECT_EQ(result.hours(), 2);
  EXPECT_EQ(result.minutes(), 30);
}

TEST(TimeTests, InputOperatorTest) {
  std::stringstream input("12:34 09:08  invalid");
  club::Time time;
  EXPECT_TRUE(input >> time);
  EXPECT_EQ(time.hours(), 12);
  EXPECT_EQ(time.minutes(), 34);

  EXPECT_TRUE(input >> time);
  EXPECT_EQ(time.hours(), 9);
  EXPECT_EQ(time.minutes(), 8);

  EXPECT_FALSE(input >> time);
}

TEST(TimeTests, OutputOperatorTest) {
  std::stringstream output;
  club::Time time = {15, 45};
  output << time;
  EXPECT_EQ(output.str(), "15:45");

  time = {7, 5};
  output.str("");
  output << time;
  EXPECT_EQ(output.str(), "07:05");
}
