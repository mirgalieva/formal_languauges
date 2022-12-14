#include "../earley/earley.cpp"
#include "../language/language.cpp"
#include <gtest/gtest.h>
#include <string>

TEST(BracketSequence, Similar) {
  std::string nonterms = "S";
  std::string terms = "()";
  Grammar grammar = Grammar(nonterms, terms);
  grammar.start_ = "S";
  Rule first_rule('S', "(S)S");
  Rule second_rule('S', "");
  grammar.AddRule(first_rule);
  grammar.AddRule(second_rule);
  Earley early(grammar);
  ASSERT_EQ(early.Check(""), true);
  ASSERT_EQ(early.Check("("), false);
  ASSERT_EQ(early.Check(")"), false);
  ASSERT_EQ(early.Check("()"), true);
  ASSERT_EQ(early.Check("()()"), true);
  ASSERT_EQ(early.Check("(())"), true);
  ASSERT_EQ(early.Check("(()"), false);
  ASSERT_EQ(early.Check(")()"), false);
  ASSERT_EQ(early.Check(")()("), false);
}

TEST(BracketSequence, Mixed) {
  std::string nonterms = "S";
  std::string terms = "()[]{}";
  Grammar grammar = Grammar(nonterms, terms);
  grammar.start_ = "S";
  Rule first_rule('S', "(S)S");
  Rule second_rule('S', "");
  Rule third_rule('S', "[S]S");
  Rule fourth_rule('S', "{S}S");
  grammar.AddRule(first_rule);
  grammar.AddRule(second_rule);
  grammar.AddRule(third_rule);
  grammar.AddRule(fourth_rule);
  Earley early(grammar);
  ASSERT_EQ(early.Check(""), true);
  ASSERT_EQ(early.Check("()"), true);
  ASSERT_EQ(early.Check("[]{}"), true);
  ASSERT_EQ(early.Check("[(])"), false);
  ASSERT_EQ(early.Check("[{)]"), false);
  ASSERT_EQ(early.Check("([]){}"), true);
}

TEST(SimpleGrammar, aB) {
  std::string nonterms = "SB";
  std::string terms = "ab";
  Grammar grammar = Grammar(nonterms, terms);
  grammar.start_ = "S";
  Rule first_rule('S', "aB");
  Rule second_rule('B', "b");
  Rule third_rule('B', "ba");
  grammar.AddRule(first_rule);
  grammar.AddRule(second_rule);
  grammar.AddRule(third_rule);
  Earley early(grammar);
  ASSERT_EQ(early.Check("ab"), true);
  ASSERT_EQ(early.Check("aba"), true);
  ASSERT_EQ(early.Check("a"), false);
  ASSERT_EQ(early.Check("ba"), false);
  ASSERT_EQ(early.Check("abab"), false);
  ASSERT_EQ(early.Check("abaa"), false);
  ASSERT_EQ(early.Check("ab "), false);
  ASSERT_EQ(early.Check("aba "),false);
}

TEST(KleeneStar, A) {
  std::string nonterms = "S";
  std::string terms = "a";
  Grammar grammar = Grammar(nonterms, terms);
  grammar.start_ = "S";
  Rule first_rule('S', "aS");
  Rule second_rule('S', "");
  grammar.AddRule(first_rule);
  grammar.AddRule(second_rule);
  Earley early(grammar);
  ASSERT_EQ(early.Check(""), true);
  ASSERT_EQ(early.Check("a"), true);
  ASSERT_EQ(early.Check("aa"), true);
  ASSERT_EQ(early.Check("ab"), false);
}

TEST(Hard, ABC) {
  std::string nonterms = "SBC";
  std::string terms = "abc";
  Grammar grammar = Grammar(nonterms, terms);
  grammar.start_ = "S";
  Rule first_rule('S', "Bb");
  Rule second_rule('B', "a");
  Rule third_rule('S', "Cc");
  Rule fourth_rule('C', "a");
  grammar.AddRule(first_rule);
  grammar.AddRule(second_rule);
  grammar.AddRule(third_rule);
  grammar.AddRule(fourth_rule);
  Earley early(grammar);
  ASSERT_EQ(early.Check("ab"), true);
  ASSERT_EQ(early.Check("ac"), true);
  ASSERT_EQ(early.Check("a"), false);
  ASSERT_EQ(early.Check("abc"), false);
  ASSERT_EQ(early.Check("abb"), false);
  ASSERT_EQ(early.Check("abbbbb"), false);
  ASSERT_EQ(early.Check("ba"), false);
  ASSERT_EQ(early.Check("b"), false);
  ASSERT_EQ(early.Check("a"), false);
  ASSERT_EQ(early.Check("baa"), false);
  ASSERT_EQ(early.Check("aba"), false);
  ASSERT_EQ(early.Check("abab"), false);
 ASSERT_EQ(early.Check("ababababab"), false);
 ASSERT_EQ(early.Check("aaabbbababab"), false);
  ASSERT_EQ(early.Check(""), false);
  ASSERT_EQ(early.Check(" "), false);
  ASSERT_EQ(early.Check("abba"), false);
  ASSERT_EQ(early.Check("babababa"), false);
  ASSERT_EQ(early.Check("bababab"), false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}