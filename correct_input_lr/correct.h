#ifndef LR_CORRECT_H
#define LR_CORRECT_H

#include "../lr/lr.h"
#include <iostream>
#include <stdexcept>

void CheckInput() {
  size_t rules_number = 0;
  std::string terms;
  std::string nonterms;
  std::cin >> terms;
  std::cin >> nonterms;
  std::cin >> rules_number;
  Grammar grammar = Grammar(terms, nonterms);
  LR algo = LR();
  algo.SetTerminals(terms);
  algo.SetNonTerminals(nonterms);
  for (size_t i = 0; i < rules_number; ++i) {
    std::string rule;
    std::cin >> rule;
    int index = 0;
    if (!algo.IsNonTerminal(rule[index])) {
      throw std::invalid_argument("Wrong arguments");
    }
    ++index;
    index += 2;
    std::string tmp = rule.substr(index, rule.size());
    grammar.AddRule(rule[0], tmp);
    for (int j = index; j < rule.size(); ++j) {
      if (!algo.IsTerminal(rule[j])) {
        if (!algo.IsNonTerminal(rule[j])) {
          throw std::invalid_argument("Wrong arguments");
        }
      }
    }
  }
  algo.Fit(grammar);
  size_t words_count;
  std::cin >> words_count;
  for (size_t i = 0; i < words_count; ++i) {
    std::string checkword;
    std::cin >> checkword;
    if (algo.Predict(std::move(checkword))) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
};
#endif //LR_CORRECT_H
