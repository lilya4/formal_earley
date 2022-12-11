#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

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
      P_.emplace_back(rule[0], std::string(rule.begin() + 3, rule.end()));
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
    grammar_.P_.emplace_back('#', std::string(1, grammar_.S_)); //add rule # -> S
    grammar_.S_ = '#';
    D_.clear();
    D_.resize(word.size() + 1);
    D_[0].insert(Situation(grammar_.P_.size() - 1, 0, 0));
    bool changed = false;
    do {
      changed = false;
      changed |= Predict(0);
      changed |= Complete(0);
    } while (changed);
    for (size_t i = 1; i <= word.size(); ++i) {
      Scan(i - 1, word[i - 1]);
      do {
        changed = false;
        changed |= Predict(i);
        changed |= Complete(i);
      } while (changed);
    }
    return D_[word.size()].find(Situation(grammar_.P_.size() - 1, 1, 0)) != D_[word.size()].end();
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

  std::vector<std::unordered_set<Situation, SituationHasher>> D_;
  Grammar grammar_;

  bool Scan(size_t j, char symbol) {
    bool changed = false;
    for (auto &situation: D_[j]) {
      if (grammar_.P_[situation.rule_idx_].to_[situation.point_pos_] == symbol) {
        D_[j + 1].insert(situation);
        changed = true;
      }
    }
    return changed;
  }

  bool Predict(size_t j) {
    std::vector<Situation> new_situations;
    for (auto &situation: D_[j]) {
      for (int rule_idx = 0; rule_idx < grammar_.P_.size(); ++rule_idx) {
        if (grammar_.P_[rule_idx].from_ == grammar_.P_[situation.rule_idx_].to_[situation.point_pos_]) {
          new_situations.emplace_back(rule_idx, 0, j);
        }
      }
    }
    for (auto situation: new_situations) {
      D_[j].insert(situation);
    }
    return !new_situations.empty();
  }

  bool Complete(size_t j) {
    std::vector<Situation> new_situations;
    for (auto complete_situation: D_[j]) {
      if (complete_situation.point_pos_ != grammar_.P_[complete_situation.rule_idx_].to_.size()) continue;
      for (auto situation: D_[complete_situation.i_]) {
        if (grammar_.P_[situation.rule_idx_].to_[situation.point_pos_] == grammar_.P_[situation.rule_idx_].from_) {
          new_situations.emplace_back(situation.rule_idx_, situation.point_pos_ + 1, situation.i_);
        }
      }
    }
    for (auto situation: new_situations) {
      D_[j].insert(situation);
    }
    return !new_situations.empty();
  }
};