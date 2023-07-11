#include <gtest/gtest.h>
#include "task_8.h"

TEST(test, res_1) {
  EXPECT_EQ(3, ShortestWordLen("b1.ac+1.a..", 'c', 0));
}
TEST(test, res_2) {
  EXPECT_EQ(3, ShortestWordLen("b*c+aab..+", 'a', 1));
}
TEST(test, res_3) {
  EXPECT_EQ(3, ShortestWordLen("ac1b*+++", 'b', 3));
}
TEST(test, res_4) {
  EXPECT_EQ(1, ShortestWordLen("cc.a*+", 'a', 1));
}
TEST(test, res_5) {
  EXPECT_EQ(3, ShortestWordLen("ac+cc+c.1..", 'b', 0));
}
TEST(test, res_6) {
  EXPECT_EQ(2, ShortestWordLen("aa1**..", 'c', 0));
}
TEST(test, res_7) {
  EXPECT_EQ(4, ShortestWordLen("b1cc**aa.....", 'c', 0));
}
TEST(test, res_8) {
  EXPECT_EQ(3, ShortestWordLen("c*", 'c', 3));
}
TEST(test, res_9) {
  EXPECT_EQ(1, ShortestWordLen("aa.b+", 'a', 0));
}
TEST(test, res_10) {
  EXPECT_EQ(3, ShortestWordLen("acc+a.1..", 'a', 0));
}
TEST(test, res_11) {
  EXPECT_EQ(4, ShortestWordLen("a*", 'a', 4));
}
TEST(test, res_12) {
  EXPECT_EQ(1, ShortestWordLen("bbc++1b+cb..+", 'a', 0));
}
TEST(test, res_13) {
  EXPECT_EQ(4, ShortestWordLen("a1b..ca1ccc++.+..", 'a', 0));
}
TEST(test, res_14) {
  EXPECT_EQ(3, ShortestWordLen("a*b111++.+", 'a', 3));
}
TEST(test, res_15) {
  EXPECT_EQ(3, ShortestWordLen("bb*bbc+1+*+.+", 'b', 3));
}
TEST(test, res_16) {
  EXPECT_EQ(3, ShortestWordLen("ba+*b*ca..+", 'a', 3));
}
TEST(test, res_17) {
  EXPECT_EQ(3, ShortestWordLen("cb*+1a+b++", 'b', 3));
}
TEST(test, res_18) {
  EXPECT_EQ(3, ShortestWordLen("c*c+", 'c', 3));
}
TEST(test, res_19) {
  ASSERT_THROW(ShortestWordLen("c*c*+ca+a1...", 'b', 2), std::invalid_argument);
}