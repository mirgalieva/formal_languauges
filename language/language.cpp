#include "language.h"

bool Rule::operator<(const Rule &other) const {
  if (left_ == other.left_) {
    return right_ < other.right_;
  }
  return left_ < other.left_;
}

bool Rule::operator==(const Rule &other) const {

  return left_ == other.left_ && right_ == other.right_;

}

Grammar::Grammar(std::string &nonterms, std::string &terms) {
  for (auto ch: nonterms) {
    nonterms_.insert(ch);
  }
  for (auto ch: terms) {
    terms_.insert(ch);
  }
}

void Grammar::AddRule(Rule &rule) {
  rules_.insert(rule);
}

bool Grammar::IsTerminal(char letter) {
  return terms_.find(letter) != terms_.end();
}

const std::set<Rule> &Grammar::Rules() {
  return rules_;
}

bool Grammar::IsContextFree() {
  for (auto rule: rules_) {
    if (nonterms_.find(rule.left_) == nonterms_.end()) {
      return false;
    }
  }
  return true;
}

const std::set<char> &Grammar::Nonterms() {
  return nonterms_;
}

const std::set<char> &Grammar::Terms() {
  return terms_;
}

void Grammar::InsertRule(const Rule &rule) {
  rules_.insert(rule);
}

void Grammar::InsertNonterms(char nonterm) {
  nonterms_.insert(nonterm);
}
