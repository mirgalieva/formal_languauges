#include "early.h"
#include "../language/language.h"

const Rule &Condition::GetRule() const {
  return rule_;
}

const int Condition::GetPointPosition() const {
  return point_position_;
}

const int Condition::GetIdx() const {
  return i_;
}

bool Condition::operator<(const Condition &another) const {
  if (rule_ == another.rule_) {
    if (point_position_ == another.point_position_) {
      return i_ < another.i_;
    }
    return point_position_ < another.point_position_;
  }
  return another.rule_ < rule_;
}

bool Earley::Check(const std::string &word) {
  std::vector<std::set<Condition>> D(word.size() + 1);
  Rule new_rule = Rule(real_start_, grammar_.start_);
  Condition condition = Condition(new_rule, 0, 0);
  grammar_.InsertRule(new_rule);
  grammar_.InsertNonterms(real_start_);
  D[0].insert(condition);
  size_t current_size = 0;
  for (size_t i = 0; i <= word.size(); ++i) {
    Scan(D, i, word[i - 1]);
    do {
      current_size = D[i].size();
      Predict(D, i);
      Complete(D, i);
    } while (D[i].size() != current_size);
  }
  return D[word.size()].find(Condition(Rule(real_start_, grammar_.start_), 0, 1)) != D[word.size()].end();
}

void Earley::Predict(std::vector<std::set<Condition>> &D, int j) {
  std::set<Condition> copy = D[j];
  for (auto &condition: copy) {
    for (auto &rule: grammar_.Rules()) {
      if (condition.GetPointPosition() != condition.GetRule().right_.size()) {
        if (rule.left_ == condition.GetRule().right_[condition.GetPointPosition()]) {
          D[j].insert(Condition(rule, j, 0));
        }
      }
    }
  }
}

void Earley::Scan(std::vector<std::set<Condition>> &D, int j, char letter) {
  if (j == 0) {
    return;
  }
  for (auto condition: D[j - 1]) {
    if (condition.GetPointPosition() != condition.GetRule().right_.size()) {
      if (condition.GetRule().right_[condition.GetPointPosition()] == letter) {
        D[j].insert(Condition(condition.GetRule(), condition.GetIdx(), condition.GetPointPosition() + 1));
      }
    }
  }
}

void Earley::Complete(std::vector<std::set<Condition>> &D, int j) {
  std::set<Condition> copy = D[j];
  for (auto situation_a: copy) {
    if (situation_a.GetPointPosition() != situation_a.GetRule().right_.size()) continue;
    for (auto situation_b: D[situation_a.GetIdx()]) {
      if (situation_b.GetPointPosition() != situation_b.GetRule().right_.size()) {
        if (situation_b.GetRule().right_[situation_b.GetPointPosition()] == situation_a.GetRule().left_) {
          D[j].insert(Condition(situation_b.GetRule(), situation_b.GetIdx(), situation_b.GetPointPosition() + 1));
        }
      }
    }
  }
}
