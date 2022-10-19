#include "parser.h"

  void Parser::Unification(){
    std::set<int> first = automaton_stack_.top();
    automaton_stack_.pop();
    std::set<int> second = automaton_stack_.top();
    automaton_stack_.pop();
    for (auto element: second){
      first.insert(element);
    }
    automaton_stack_.push(first);
  }
  void Parser::KleeneStar(){
    std::set<int> first = automaton_stack_.top();
    automaton_stack_.pop();
    std::set<int> new_set(first);
    std::set<int> old_set;
    while (old_set.size()!=new_set.size() && old_set != new_set){
      old_set = std::set<int>(new_set);
      for (int first_element:old_set){
        for (int second_element:old_set){
          if (number_of_chr_!=0){
            new_set.insert((first_element+second_element)%number_of_chr_);
          }
        }
      }
    }
    automaton_stack_.push(new_set);
  }
  void Parser::Concatenation(){
    std::set<int> first = automaton_stack_.top();
    automaton_stack_.pop();
    std::set<int> second = automaton_stack_.top();
    automaton_stack_.pop();
    std::set<int> temp_set;
    for (auto first_element: first){
      for (auto second_element:second){
        if (number_of_chr_!=0){
          temp_set.insert((first_element+second_element)%number_of_chr_);
        }
      }
    }
    automaton_stack_.push(temp_set);
  }
  bool Parser::Parsing(){
    if (chr_!='a' && chr_!='b' && chr_!='c' && chr_!='1'){
      throw std::invalid_argument( "wrong letter" );
    } else {
      automaton_stack_ = std::stack<std::set<int>>();
      for (auto symbol:regular_expression_){
        switch (symbol){
          case '1':
            automaton_stack_.push({0});
            break;
          case 'a':
          case 'b':
          case 'c':
            automaton_stack_.push({symbol==chr_});
            break;
          case '*':
            if (automaton_stack_.size()>=1){
              KleeneStar();
            } else {
              throw std::invalid_argument( "wrong regular expression" );
            }
            break;
          case '.':
            if (automaton_stack_.size()>=2){
              Concatenation();
            } else {
              throw std::invalid_argument( "wrong regular expression" );
            }
            break;
          case '+':
            if (automaton_stack_.size()>=2){
              Unification();
            } else {
              throw std::invalid_argument( "wrong regular expression" );
            }
            break;
        }
      }
      if (automaton_stack_.size()!=1){
        throw std::invalid_argument( "wrong regular expression" );
      }
    }
    if (number_of_chr_==0){
      if (regular_expression_.find(chr_)==std::string::npos){
        return true;
      } else {
        return false;
      }
    } else if (regular_expression_.find(chr_)==std::string::npos) {
      if (number_of_chr_ == 0) {
        return true;
      } else {
        return false;
      }
    }
    return automaton_stack_.top().find(0)!=automaton_stack_.top().end();
  }
Parser::Parser(const std::string str, char chr, int number):regular_expression_(str), chr_(chr), number_of_chr_(number){
  }