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
    std::queue<size_t> d_to_change;
    for (size_t i = 0; i <= word.size(); ++i) {
      d_to_change.emplace(i);
    }
    do {
      if (!Predict(0) && !Complete(0)) {
        d_to_change.pop();
      }
    } while (d_to_change.front() == 0);
    while (!d_to_change.empty()) {
      auto id = d_to_change.front();
      Scan(id - 1, word[id - 1]);
      do {
        if (!Predict(id) && !Complete(id)) {
          d_to_change.pop();
        }
      } while (d_to_change.front() == id);
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

  bool Scan(size_t j, char symbol) {
    bool changed = false;
    for (auto &situation: D_[j][symbol]) {
      D_[j + 1][grammar_.P_[situation.rule_idx_].to_[situation.point_pos_ + 1]].emplace(
              situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
      changed = true;
    }
    return changed;
  }

  bool Predict(size_t j) {
    std::vector<Situation> new_situations;
    for (int rule_idx = 0; rule_idx < grammar_.P_.size(); ++rule_idx) {
      for (auto &situation: D_[j][grammar_.P_[rule_idx].from_]) {
        new_situations.emplace_back(rule_idx, 0, j);
      }
    }
    for (auto situation: new_situations) {
      D_[j + 1][grammar_.P_[situation.rule_idx_].to_[situation.point_pos_ + 1]].emplace(
              situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
    }
    return !new_situations.empty();
  }

  bool Complete(size_t j) {
    std::vector<Situation> new_situations;
    for (auto &complete_situation: D_[j]['$']) {
      for (auto situation: D_[complete_situation.i_][grammar_.P_[complete_situation.rule_idx_].from_]) {
        new_situations.emplace_back(situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
      }
    }
    for (auto situation: new_situations) {
      D_[j + 1][grammar_.P_[situation.rule_idx_].to_[situation.point_pos_ + 1]].emplace(
              situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
    }
    return !new_situations.empty();
  }
};