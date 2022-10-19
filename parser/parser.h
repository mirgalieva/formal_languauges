#include <string>
#include <set>
#include <stack>
#include <iostream>
#include <stdexcept>
class Parser {
  std::string regular_expression_;
  std::stack<std::set<int>> automaton_stack_;
  char chr_;
  int number_of_chr_;
public:
  Parser(const std::string str, char chr, int number);

  void Unification();

  void KleeneStar();

  void Concatenation();

  bool Parsing();

};