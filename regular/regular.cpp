#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

struct Characteristic {
  std::vector<bool> deg_;
  std::vector<int64_t> min_word_;
};
int64_t Min(const int64_t& lhs, const int64_t& rhs) {
  if (lhs == -1 || rhs == -1) {
    return std::max(lhs, rhs);
  }
  return std::min(lhs, rhs);
}
int64_t ShortestWordLen(const std::string& regular_expression, const char& x, const int64_t& k) {
  if (k < 0) {
    throw std::invalid_argument("ERROR");
  }
  std::stack<Characteristic> processed_expressions;
  for (char symbol : regular_expression) {
    if (symbol == '1') {
      processed_expressions.push({std::vector<bool>(k + 1, false), std::vector<int64_t>(k + 1, -1)});
      processed_expressions.top().deg_[0] = true;
      processed_expressions.top().min_word_[0] = 0;
    } else if ((symbol == 'a') || (symbol == 'b') || (symbol == 'c')) {
      processed_expressions.push({std::vector<bool>(k + 1, false), std::vector<int64_t>(k + 1, -1)});
      processed_expressions.top().min_word_[0] = 1;
      if (symbol == x) {
        if (k >= 1) {
          processed_expressions.top().deg_[1] = true;
          processed_expressions.top().min_word_[1] = 1;
        }
      } else {
        processed_expressions.top().min_word_[0] = 1;
      }
    } else if (symbol == '.') {
      if (processed_expressions.size() < 2) {
        throw std::invalid_argument("ERROR");
      }
      auto r2 = processed_expressions.top();
      processed_expressions.pop();
      auto r1 = processed_expressions.top();
      processed_expressions.pop();
      processed_expressions.push({std::vector<bool>(k + 1, false), std::vector<int64_t>(k + 1, -1)});
      auto& r = processed_expressions.top();
      for (int i = 0; i <= k; ++i) {
        for (int j = 0; j <= k - i; ++j) {
          if (r1.deg_[i] && r2.deg_[j]) {
            r.deg_[i + j] = true;
          }
        }
      }
      for (int i = 0; i <= k; ++i) {
        for (int j = 0; j <= i; ++j) {
          if (r1.deg_[j] && r1.min_word_[i] != -1 && r2.min_word_[0] != -1) {
            r.min_word_[i] = Min(r.min_word_[i], j + r2.min_word_[i - j]);
          }
        }
        if (r1.min_word_[i] != -1 && r2.min_word_[0] != -1) {
          r.min_word_[i] = Min(r.min_word_[i], r1.min_word_[i] + r2.min_word_[0]);
        }
      }
    } else if (symbol == '*') {
      if (processed_expressions.empty()) {
        throw std::invalid_argument("ERROR");
      }
      auto r1 = processed_expressions.top();
      processed_expressions.pop();
      processed_expressions.push({std::vector<bool>(k + 1, false), std::vector<int64_t>(k + 1, -1)});
      auto& r = processed_expressions.top();
      r.deg_[0] = true;
      for (int i = 1; i <= k; ++i) {
        for (int j = 1; j <= i; ++j) {
          if (r.deg_[i - j] && r1.deg_[j]) {
            r.deg_[i] = true;
          }
        }
      }
      for (int i = 0; i <= k; ++i) {
        if (r.deg_[i]) {
          r.min_word_[i] = i;
          continue;
        }
        for (int j = 0; j <= i; ++j) {
          if (r.deg_[j]) {
            r.min_word_[i] = Min(r.min_word_[i], j + r1.min_word_[i - j]);
          }
        }
      }
    } else if (symbol == '+') {
      if (processed_expressions.size() < 2) {
        throw std::invalid_argument("ERROR");
      }
      auto r1 = processed_expressions.top();
      processed_expressions.pop();
      auto r2 = processed_expressions.top();
      processed_expressions.pop();
      processed_expressions.push({std::vector<bool>(k + 1, false), std::vector<int64_t>(k + 1, -1)});
      auto& r = processed_expressions.top();
      for (int i = 0; i <= k; ++i) {
        r.deg_[i] = r1.deg_[i] | r2.deg_[i];
        r.min_word_[i] = Min(r1.min_word_[i], r2.min_word_[i]);
      }
    } else {
      throw std::invalid_argument("ERROR");
    }
  }
  if (processed_expressions.size() > 1) {
    throw std::invalid_argument("ERROR");
  }
  if (processed_expressions.top().min_word_[k] == -1) {
    throw std::invalid_argument("INF");
  }
  return processed_expressions.top().min_word_[k];
}

int main() {
  std::string regular_expression;
  char x;
  int64_t k;
  std::cin >> regular_expression >> x >> k;
  try {
    std::cout << ShortestWordLen(regular_expression, x, k);
  } catch (std::invalid_argument const& ex) {
    std::cout << ex.what();
  }
}