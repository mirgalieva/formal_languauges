#include "../parser/parser.h"
#include <gtest/gtest.h>
#include <string>

TEST(CorrectInput, CorrectWord) {
  Parser first_parser = Parser("ab.ba.+*1a+ba.+.", 'a', 2);
  ASSERT_EQ(first_parser.Parsing(), true);
  Parser second_parser = Parser("aa+b.", 'a', 3);
  ASSERT_EQ(second_parser.Parsing(), false);
}

TEST(CorrectInput, LetterDoesNotExist) {
  Parser parser = Parser("aa+b.", 'c', 3);
  ASSERT_EQ(parser.Parsing(), false);
}

TEST(CorrectInput, ZeroLetters) {
  Parser first_parser = Parser("aa+b.", 'c', 0);
  ASSERT_EQ(first_parser.Parsing(), true);
  Parser second_parser = Parser("aa+b.", 'b', 0);
  ASSERT_EQ(second_parser.Parsing(), false);
}

TEST(IncorrectInput, TooManyArguments) {
  Parser parser = Parser("aaaaaaaaaa+b.", 'b', 0);
  ASSERT_ANY_THROW(parser.Parsing());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
