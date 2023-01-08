#ifndef EARLEYPARSER_LANGUAGE_H
#define EARLEYPARSER_LANGUAGE_H

#include <cstdio>
#include <string>
#include <set>

class Rule {
public:
  explicit Rule(char left, std::string &right) : left_(left), right_(right) {}
  explicit Rule(char left, std::string &&right): left_(left), right_(std::move(right)){}
  char left_;
  std::string right_;

  bool operator<(const Rule &other) const;

  bool operator==(const Rule &other) const;
};

class Grammar {
public:
  std::string start_;

  explicit Grammar(std::string &nonterms, std::string &terms);

  void AddRule(Rule &rule);

  bool IsTerminal(char letter);

  const std::set<Rule> &Rules();

  bool IsContextFree();

  const std::set<char> &Nonterms();

  const std::set<char> &Terms();

  void InsertRule(const Rule &rule);

  void InsertNonterms(char nonterm);

private:
  std::set<char> nonterms_;
  std::set<char> terms_;
  std::set<Rule> rules_;
};

#endif //EARLEYPARSER_LANGUAGE_H
