#include "lr.h"

Grammar::Grammar(std::string terminals, std::string non_terminals) {
  for (size_t i = 0; i < terminals.size(); ++i) {
    terminals_.insert(terminals[i]);
  }
  for (size_t i = 0; i < non_terminals.size(); ++i) {
    non_terminals_.insert(non_terminals[i]);
  }
};

bool Rule::operator<(const Rule &rhs) const {
  if (left_path == rhs.left_path) {
    if (rhs.right_path == right_path) {
      if (next == rhs.next) {
        return dot_pos < rhs.dot_pos;
      }
      return next < rhs.next;
    }

    return rhs.right_path < right_path;
  }
  return left_path < rhs.left_path;
}

bool Rule::operator==(const Rule &rhs) const {
  return (left_path == rhs.left_path &&
          right_path == rhs.right_path &&
          next == rhs.next &&
          dot_pos == rhs.dot_pos);
}

void Grammar::AddRule(char left_rule, std::string &right_rule) {
  grammar_[left_rule].push_back(right_rule);
}


std::unordered_map<char, std::vector<std::string>> &Grammar::GetGrammar() {
  return grammar_;
}

std::set<char> &Grammar::GetTerminals() {
  return terminals_;
}

std::set<char> &Grammar::GetNonTerminals() {
  return non_terminals_;
}

void LR::Fit(Grammar &gram) {
  BuildGrammarSets(gram);
  LR_table_.resize(grammar_states_.size());
  go_to_.resize(grammar_states_.size());
  for (size_t i = 0; i < LR_table_.size(); ++i) {
    for (auto &symbol: terminals_)
      LR_table_[i][symbol] = {Action::ERROR};

    for (auto &symbol: non_terminals_)
      go_to_[i][symbol] = -1;
  }
  for (size_t i = 0; i < LR_table_.size(); ++i) {
    for (auto &rule: grammar_states_[i]) {
      if ((rule.dot_pos == rule.right_path.size()) && (rule.left_path != real_start_)) {
        LR_table_[i][rule.next] = {Action::REDUCE, FindRule(rule), rule.left_path};
      } else if ((rule.dot_pos == rule.right_path.size()) && (rule.left_path == real_start_)) {
        LR_table_[i][end_symbol_] = {Action::ACCEPT};
      } else {
        char symbol = rule.right_path[rule.dot_pos];
        if (!IsNonTerminal(symbol)) {
          int index = -1;
          std::set<Rule> move_to = GoTo(grammar_states_[i], symbol);
          for (size_t j = 0; j < grammar_states_.size(); ++j) {
            if (move_to == grammar_states_[j]) {
              index = j;
              break;
            }
          }
          if (index >= 0)
            LR_table_[i][symbol] = {Action::SHIFT, index};
        }
        if (IsNonTerminal(symbol)) {
          int index = -1;
          std::set<Rule> move_to = GoTo(grammar_states_[i], symbol);
          for (size_t j = 0; j < grammar_states_.size(); ++j) {
            if (move_to == grammar_states_[j]) {
              index = j;
              break;
            }
          }
          if (index >= 0)
            go_to_[i][symbol] = index;
        }
      }
    }
  }
}

bool LR::Predict(std::string &&str_find) {
  std::string str = str_find;
  str += end_symbol_;
  int index = 0;
  std::stack<int> stack_algo;
  stack_algo.push(0);
  while (index < str.size()) {
    int cur_state = stack_algo.top();
    if (!(IsTerminal(str[index]))) {
      return false;
    }
    if ((LR_table_[cur_state][str[index]].action == Action::ACCEPT) && (index == str.size() - 1))
      return true;
    if (LR_table_[cur_state][str[index]].action == Action::SHIFT) {
      stack_algo.push(LR_table_[cur_state][str[index]].index);
      ++index;
    } else if (LR_table_[cur_state][str[index]].action == Action::REDUCE) {
      std::string cur_rule_right = grammar_[LR_table_[cur_state][str[index]].start_symbol][LR_table_[cur_state][str[index]].index];
      for (int i = 0; i < cur_rule_right.size(); ++i)
        stack_algo.pop();
      int s = stack_algo.top();
      if (go_to_[s][LR_table_[cur_state][str[index]].start_symbol] < 0)
        return false;
      stack_algo.push(go_to_[s][LR_table_[cur_state][str[index]].start_symbol]);
    } else if (LR_table_[cur_state][str[index]].action == Action::ERROR)
      return false;
  }
  return false;
}

int LR::FindRule(const Rule &rule) {
  for (int i = 0; i < grammar_[rule.left_path].size(); ++i) {
    if ((grammar_[rule.left_path][i] == rule.right_path) ||
        (rule.right_path.empty()))
      return i;
  }
  return -1;
}

std::set<Rule> LR::Closure(const std::set<Rule> &set) {
  bool changed = true;
  std::set<Rule> cur_set(set);
  while (changed) {
    changed = false;
    for (auto &situation: cur_set) {
      int pos = situation.dot_pos;
      if (pos != situation.right_path.size()) {
        char start_state = situation.right_path[pos];
        if (IsNonTerminal(start_state)) {
          std::vector<char> next_situation;
          std::string beta;
          char alpha = situation.next;
          if (pos + 1 < situation.right_path.size())
            beta = situation.right_path.substr(pos + 1, situation.right_path.size());
          next_situation = First(beta, alpha);
          for (auto &rule: grammar_[start_state]) {
            Rule new_rule = {start_state, rule};
            new_rule.dot_pos = 0;
            for (auto &next: next_situation) {
              new_rule.next = next;
              if (cur_set.find(new_rule) == cur_set.end()) {
                cur_set.insert(new_rule);
                changed = true;
              }
            }
          }
        }
      }
    }
  }
  return cur_set;
}

std::vector<char> LR::First(std::string beta, char alpha) {
  std::vector<char> first_terminals;
  if (beta.empty()) {
    first_terminals.push_back(alpha);
    return first_terminals;
  }
  if (IsNonTerminal(beta[0])) {
    bool changed = true;
    char state = beta[0];
    while (changed) {
      changed = false;
      for (auto &rule: grammar_[state]) {
        if (IsNonTerminal(rule[0])) {
          state = rule[0];
          break;
        } else {
          first_terminals.push_back(rule[0]);
          changed = true;
        }
      }
    }
  }
  first_terminals.push_back(beta[0]);
  return first_terminals;
}

std::set<Rule> LR::GoTo(std::set<Rule> &set, char symbol) {
  std::set<Rule> cur_set;
  for (auto &situations: set) {
    if (situations.dot_pos != situations.right_path.size()) {
      if (situations.right_path[situations.dot_pos] == symbol)
        cur_set.insert({situations.left_path, situations.right_path, situations.next, situations.dot_pos + 1});
    }
  }
  return Closure(cur_set);
}

void LR::BuildGrammarSets(Grammar &gram) {
  grammar_ = gram.GetGrammar();
  terminals_ = gram.GetTerminals();
  non_terminals_ = gram.GetNonTerminals();
  std::set<Rule> start_rule;
  start_rule.insert(Rule{real_start_, "S", end_symbol_, 0});
  grammar_states_.push_back(Closure(start_rule));
  bool changed = true;
  while (changed) {
    changed = false;
    std::vector<std::set<Rule>> cur_grammar_states;
    for (auto &cur_set: grammar_states_) {
      std::vector<std::set<char>> symbols = {terminals_, non_terminals_};
      for (size_t i = 0; i <= 1; ++i) {
        for (auto &symbol_grammar: symbols[i]) {
          std::set<Rule> next_set = GoTo(cur_set, symbol_grammar);
          if (!next_set.empty()) {
            if (std::find(grammar_states_.begin(), grammar_states_.end(), next_set) ==
                grammar_states_.end()) {
              cur_grammar_states.push_back(next_set);
              grammar_states_.push_back(next_set);
              changed = true;
            }
          }
        }
      }
    }
  }
}

bool LR::IsNonTerminal(char letter) {
  return non_terminals_.find(letter) != non_terminals_.end();
}

bool LR::IsTerminal(char letter) {
  return terminals_.find(letter) != terminals_.end();
}


void LR::SetTerminals(std::string terminals) {
  for (size_t i = 0; i < terminals.size(); ++i) {
    terminals_.insert(terminals[i]);
  }
}

void LR::SetNonTerminals(std::string non_terminals) {
  for (size_t i = 0; i < non_terminals.size(); ++i) {
    non_terminals_.insert(non_terminals[i]);
  }
}