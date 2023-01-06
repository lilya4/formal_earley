#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <utility>
#include <vector>
#include <unordered_set>

class Grammar {
public:
  struct Rule {
    char from_;
    std::string to_;
    Rule(const char &from, std::string to) : from_(from), to_(std::move(to)) {}
  };

  Grammar(const std::vector<char> &N, const std::vector<char> &sigma, const std::vector<std::string> &rules, char S)
          : N_(N), sigma_(sigma), S_(S) {
    for (auto &rule: rules) {
      P_.emplace_back(rule[0], std::string(rule.begin() + 3, rule.end()) + "$");
    }
  }

private:
  std::vector<char> N_;
  std::vector<char> sigma_;
  std::vector<Rule> P_;
  char S_;

  friend class Earley;
};

class Earley {
public:
  Earley(const Grammar &grammar) : grammar_(grammar) {
  }
  bool ParseWord(const std::string &word) {
    grammar_.P_.emplace_back('#', std::string(1, grammar_.S_) + "$"); //add rule #->S
    D_.clear();
    D_.resize(word.size() + 1);
    D_[0][grammar_.S_].emplace(grammar_.P_.size() - 1, 0, 0);
    grammar_.S_ = '#';
    bool changed = false;
    Closure(0);
    for (size_t i = 1; i <= word.size(); ++i) {
      Scan(i - 1, word[i - 1]);
      Closure(i - 1);
    }
    for (auto &pair: D_[word.size()]) {
      for (auto &situation: pair.second) {
        if (situation == Situation(grammar_.P_.size() - 1, 1, 0)) {
          return true;
        }
      }
    }
    return false;
  }

private:
  struct Situation {
    size_t rule_idx_;
    size_t point_pos_;
    size_t i_;
    Situation() {};
    Situation(size_t rule_idx, size_t point_pos, size_t i) : rule_idx_(rule_idx), point_pos_(point_pos), i_(i) {};

    bool operator==(const Situation &other) const {
      return rule_idx_ == other.rule_idx_ && point_pos_ == other.rule_idx_ && i_ == other.i_;
    }
  };

  struct SituationHasher {
    size_t operator()(const Situation &situation) const {
      return std::hash<uint64_t>()(situation.rule_idx_);
    }
  };

  std::vector<std::map<char, std::unordered_set<Situation, SituationHasher>>> D_;
  Grammar grammar_;
  void Closure(size_t j) {
    std::queue<Situation> situations_to_close;
    for (auto &pair: D_[j]) {
      for (auto &situation: pair.second) {
        situations_to_close.push(situation);
      }
    }
    std::vector<Situation> new_situations;
    while(!situations_to_close.empty()) {
      auto situation = situations_to_close.front();
      situations_to_close.pop();
      new_situations = Predict(j, situation);
      for (auto& sit: new_situations) {
        situations_to_close.push(sit);
      }
      new_situations = Complete(j, situation);
      for (auto& sit: new_situations) {
        situations_to_close.push(sit);
      }
    }
  }

  void Scan(size_t j, char symbol) {
    for (auto &situation: D_[j][symbol]) {
      D_[j + 1][grammar_.P_[situation.rule_idx_].to_[situation.point_pos_ + 1]].emplace(
              situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
    }
  }

  std::vector<Situation> Predict(size_t j, Situation situation) {
    std::vector<Situation> new_situations;
    for (int rule_idx = 0; rule_idx < grammar_.P_.size(); ++rule_idx) {
      if (grammar_.P_[rule_idx].from_ == grammar_.P_[situation.rule_idx_].to_[situation.point_pos_]) {
        new_situations.emplace_back(rule_idx, 0, j);
      }
    }
    for (auto sit: new_situations) {
      D_[j + 1][grammar_.P_[sit.rule_idx_].to_[sit.point_pos_ + 1]].emplace(
              sit.rule_idx_, sit.point_pos_ + 1, sit.i_);
    }
    return new_situations;
  }

  std::vector<Situation> Complete(size_t j, Situation complete_situation) {
    std::vector<Situation> new_situations;
    if (complete_situation.point_pos_ != grammar_.P_[complete_situation.rule_idx_].to_.size()) {
      return new_situations;
    }
    for (auto &situation: D_[complete_situation.i_][grammar_.P_[complete_situation.rule_idx_].from_]) {
      new_situations.emplace_back(situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
    }
    for (auto sit: new_situations) {
      D_[j + 1][grammar_.P_[sit.rule_idx_].to_[sit.point_pos_ + 1]].emplace(
              sit.rule_idx_, sit.point_pos_ + 1, sit.i_);
    }
    return new_situations;
  }
};
