#include "../lr/lr.cpp"
#include <gtest/gtest.h>
#include <string>


TEST(SimpleGrammar, aB) {
  std::string nonterms = "SB";
  std::string terms = "ab";
  Grammar grammar = Grammar(terms, nonterms);
  std::string first_rule = "aB";
  std::string second_rule = "b";
  std::string third_rule = "ba";
  grammar.AddRule('S', first_rule);
  grammar.AddRule('B', second_rule);
  grammar.AddRule('B', third_rule);
  LR algo;
  algo.Fit(grammar);
  ASSERT_EQ(algo.Predict("ab"), true);
  ASSERT_EQ(algo.Predict("aba"), true);
  ASSERT_EQ(algo.Predict("a"), false);
  ASSERT_EQ(algo.Predict("ba"), false);
  ASSERT_EQ(algo.Predict("abab"), false);
  ASSERT_EQ(algo.Predict("abaa"), false);
  ASSERT_EQ(algo.Predict("ab "), false);
  ASSERT_EQ(algo.Predict("aba "), false);
}

TEST(KleeneStar, A) {
  std::string nonterms = "S";
  std::string terms = "a";
  Grammar grammar = Grammar(terms, nonterms);
  std::string first_rule = "aS";
  std::string second_rule = "";
  grammar.AddRule('S', first_rule);
  grammar.AddRule('S', second_rule);
  LR algo;
  algo.Fit(grammar);
  ASSERT_EQ(algo.Predict("aa"), true);
  ASSERT_EQ(algo.Predict("ab"), false);
}

TEST(Hard, ABC) {
  std::string nonterms = "SBC";
  std::string terms = "abc";
  Grammar grammar = Grammar(terms, nonterms);
  std::string first_rule = "Bb";
  std::string second_rule = "a";
  std::string third_rule = "Cc";
  std::string fourth_rule = "a";
  grammar.AddRule('S', first_rule);
  grammar.AddRule('B', second_rule);
  grammar.AddRule('S', third_rule);
  grammar.AddRule('C', fourth_rule);
  LR algo;
  algo.Fit(grammar);
  ASSERT_EQ(algo.Predict("ab"), true);
  ASSERT_EQ(algo.Predict("ac"), true);
  ASSERT_EQ(algo.Predict("a"), false);
  ASSERT_EQ(algo.Predict("abc"), false);
  ASSERT_EQ(algo.Predict("abb"), false);
  ASSERT_EQ(algo.Predict("abbbbb"), false);
  ASSERT_EQ(algo.Predict("ba"), false);
  ASSERT_EQ(algo.Predict("b"), false);
  ASSERT_EQ(algo.Predict("a"), false);
  ASSERT_EQ(algo.Predict("baa"), false);
  ASSERT_EQ(algo.Predict("aba"), false);
  ASSERT_EQ(algo.Predict("abab"), false);
  ASSERT_EQ(algo.Predict("ababababab"), false);
  ASSERT_EQ(algo.Predict("aaabbbababab"), false);
  ASSERT_EQ(algo.Predict(""), false);
  ASSERT_EQ(algo.Predict(" "), false);
  ASSERT_EQ(algo.Predict("abba"), false);
  ASSERT_EQ(algo.Predict("babababa"), false);
  ASSERT_EQ(algo.Predict("bababab"), false);
}

TEST(Hard, SBC) {
  std::string nonterms = "SBC";
  std::string terms = "abc";
  Grammar grammar = Grammar(terms, nonterms);
  std::string first_rule = "B";
  std::string second_rule = "baa";
  std::string third_rule;
  std::string fourth_rule = "baaa";
  grammar.AddRule('S', first_rule);
  grammar.AddRule('B', second_rule);
  grammar.AddRule('S', third_rule);
  grammar.AddRule('B', fourth_rule);
  LR algo;
  algo.Fit(grammar);
  ASSERT_EQ(algo.Predict("baa"), true);
  ASSERT_EQ(algo.Predict("baaa"), true);
  ASSERT_EQ(algo.Predict("ba"), false);
  ASSERT_EQ(algo.Predict("baaaa"), false);
  ASSERT_EQ(algo.Predict("abb"), false);
  ASSERT_EQ(algo.Predict("abbbbb"), false);
  ASSERT_EQ(algo.Predict("ba"), false);
  ASSERT_EQ(algo.Predict("b"), false);
  ASSERT_EQ(algo.Predict("a"), false);
  ASSERT_EQ(algo.Predict("aba"), false);
  ASSERT_EQ(algo.Predict("abab"), false);
  ASSERT_EQ(algo.Predict("abababababab"), false);
  ASSERT_EQ(algo.Predict("abba"), false);
  ASSERT_EQ(algo.Predict("babababa"), false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}