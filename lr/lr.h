#ifndef LR_LR_H
#define LR_LR_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <stack>

enum class Action {
  SHIFT, REDUCE, ACCEPT, ERROR,
};

struct TableStatus {
  Action action;
  int index;
  char start_symbol = '.';
};

struct Rule {
  char left_path;
  std::string right_path;
  char next;
  int dot_pos = 0;

  bool operator<(const Rule &rhs) const;

  bool operator==(const Rule &rhs) const;
};

class Grammar {
public:
  Grammar(std::string terminals, std::string non_terminals);

  void AddRule(char left_rule, std::string &right_rule);

  std::unordered_map<char, std::vector<std::string>> &GetGrammar();

  std::set<char> &GetTerminals();

  std::set<char> &GetNonTerminals();

private:
  std::unordered_map<char, std::vector<std::string>> grammar_;
  std::set<char> terminals_ = {'$'};
  std::set<char> non_terminals_;
};

class LR {
public:
  void Fit(Grammar &gram);

  bool IsTerminal(char letter);

  bool IsNonTerminal(char letter);

  bool Predict(std::string &&str_find);

  void SetTerminals(std::string);

  void SetNonTerminals(std::string);

private:
  const char end_symbol_ = '$';
  char real_start_ = '#';
  std::set<char> terminals_;
  std::set<char> non_terminals_;
  std::vector<std::unordered_map<char, TableStatus>> LR_table_;
  std::vector<std::unordered_map<char, int>> go_to_;
  std::vector<std::set<Rule>> grammar_states_;
  std::unordered_map<char, std::vector<std::string>> grammar_;

  int FindRule(const Rule &rule);

  std::set<Rule> Closure(const std::set<Rule> &set);

  std::vector<char> First(std::string beta, char alpha);

  std::set<Rule> GoTo(std::set<Rule> &set, char symbol);

  void BuildGrammarSets(Grammar &gram);
};

#endif //LR_LR_H
