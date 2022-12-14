#ifndef EARLEYPARSER_EARLY_H
#define EARLEYPARSER_EARLY_H

#include "../language/language.h"
#include <stdexcept>
#include <vector>
#include <any>
#include <set>

class Condition {
public:
  explicit Condition(const Rule &rule, int i, int point_position) : rule_(rule), i_(i),
                                                                    point_position_(point_position) {
  }

  const Rule &GetRule() const;

  const int GetPointPosition() const;

  const int GetIdx() const;

  bool operator<(const Condition &another) const;

private:
  Rule rule_;
  int i_;
  int point_position_;
};




class Earley {
public:
  explicit Earley(Grammar grammar) : grammar_(grammar) {}

  bool Check(const std::string &word);

  void Predict(std::vector<std::set<Condition>> &D, int j);

  void Scan(std::vector<std::set<Condition>> &D, int j, char letter) ;

  void Complete(std::vector<std::set<Condition>> &D, int j);
private:
  Grammar grammar_;
  char real_start_ = '#';
};

#endif //EARLEYPARSER_EARLY_H
