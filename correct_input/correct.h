#ifndef EARLEYPARSER_CORRECT_H
#define EARLEYPARSER_CORRECT_H

#include "../language/language.cpp"
#include <iostream>
#include <stdexcept>
#include "../earley/earley.cpp"

void CheckInput() {
  size_t nonterms_number = 0;
  size_t term_number = 0;
  size_t rules_number = 0;
  std::string terms;
  std::string nonterms;
  std::string rule;
  try {
    std::cin >> nonterms_number >> term_number >> rules_number;
    std::cin >> nonterms;
    std::cin >> terms;
  } catch (...) {
    throw std::invalid_argument("Wrong arguments");
  }
  Grammar grammar(nonterms, terms);
  for (size_t i = 0; i < rules_number; ++i) {
    std::cin >> rule;
    bool is_left = true;
    std::string left, right;
    for (auto letter: rule) {
      if (letter != '-' && letter != '>') {
        if (grammar.Nonterms().find(letter) == grammar.Nonterms().end()) {
          if (grammar.Terms().find(letter) == grammar.Terms().end()) {
            throw std::invalid_argument("Wrong rule");
          } else {
            if (is_left) {
              left.push_back(letter);
            } else {
              right.push_back(letter);
            }
          }
        } else {
          if (is_left) {
            left.push_back(letter);
          } else {
            right.push_back(letter);
          }
        }
      } else {
        is_left = false;
      }
    }
    Rule new_rule = Rule(left[0], right);
    grammar.AddRule(new_rule);
  }
  try {
    std::cin >> grammar.start_;
  } catch (...) {
    throw std::invalid_argument("Wrong start argument");
  }
  if (grammar.Nonterms().find(grammar.start_[0]) == grammar.Nonterms().end()) {
    throw std::invalid_argument("Start is terminal");
  }
  if (!grammar.IsContextFree()) {
    throw std::invalid_argument("Wrong grammar");
  }

  Earley early = Earley(grammar);
  size_t words_count = 0;
  std::cin >> words_count;
  std::string word;
  for (size_t i = 0; i < words_count; ++i) {
    std::cin >> word;
    for (auto letter: word) {
      if (!grammar.IsTerminal(letter)) {
        throw std::invalid_argument("Wrong word");
      }
    }
    if (early.Check(word)) {
      std::cout << "Yes\n";
    } else {
      std::cout << "No\n";
    }
  }
};
#endif //EARLEYPARSER_CORRECT_H
