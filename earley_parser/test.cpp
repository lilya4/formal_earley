#include <gtest/gtest.h>
#include "earley.h"

TEST(Test, Test1) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', '+'}, {"S->T+S", "S->T", "T->F+T", "F->(S)", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("(a+a)"));
  EXPECT_FALSE(earley_parser.ParseWord("b"));
  EXPECT_FALSE(earley_parser.ParseWord("aa"));
  EXPECT_FALSE(earley_parser.ParseWord("(a+b)"));
}

TEST(Test, Test2) {
  Earley earley_parser({{'S'}, {'(', ')'}, {"S->(S)S", "S->"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("(())"));
  EXPECT_FALSE(earley_parser.ParseWord(")"));
  EXPECT_FALSE(earley_parser.ParseWord("(()"));
}

TEST(Test, Test3) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b'}, {"S->aTa", "S->bT", "T->bF", "F->aaa", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("abaaaa"));
  EXPECT_FALSE(earley_parser.ParseWord("abb"));
}

TEST(Test, Test4) {
  Earley earley_parser({{'S', 'T'}, {'a', 'b'}, {"S->a", "S->T", "T->Tb", "T->b"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("bbb"));
  EXPECT_FALSE(earley_parser.ParseWord("ab"));
}

TEST(Test, Test5) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', 'c'}, {"S->aTa", "S->bT", "T->bF", "F->aaa", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("abaa"));
  EXPECT_FALSE(earley_parser.ParseWord("abc"));
}

TEST(Test, Test6) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', 'c'}, {"S->bSbb", "S->T", "T->Sb", "T->F", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("babbb"));
  EXPECT_FALSE(earley_parser.ParseWord("abbc"));
}

TEST(Test, Test6) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', 'c'}, {"S->bSbb", "S->T", "T->Sb", "T->F", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("bbabbbb"));
  EXPECT_FALSE(earley_parser.ParseWord("ccc"));
}

TEST(Test, Test8) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', 'c'}, {"S->TbF", "S->Sa", "T->FT", "T->abc", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("abcba"));
  EXPECT_FALSE(earley_parser.ParseWord("accb"));
}

TEST(Test, Test9) {
  Earley earley_parser({{'S', 'T', 'F'}, {'a', 'b', 'c'}, {"S->TbF", "S->Sa", "T->FT", "T->abc", "F->a"}, 'S'});
  EXPECT_TRUE(earley_parser.ParseWord("aabcbaaaaaa"));
  EXPECT_FALSE(earley_parser.ParseWord("abcbb"));
}