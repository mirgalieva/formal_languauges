#include "parser/parser.cpp"
#include <gtest/gtest.h>

int main() {
  std::string regular_expression;
  char chr;
  size_t number;
  std::cin >> regular_expression >> chr >> number;
  auto solve =Parser(regular_expression, chr, number);
  if (solve.Parsing()) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}