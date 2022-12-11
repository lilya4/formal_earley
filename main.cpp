#include "earley.cpp"
#include <iostream>
#include <vector>

int main() {

  std::vector<char> N;
  std::vector<char> sigma;
  std::vector<std::string> P;
  char S;
  int N_count, sigma_count, P_count;
  std::cin >> N_count;
  N.resize(N_count);
  for (int i = 0; i < N_count; ++i) {
    std::cin >> N[i];
  }
  std::cin >> sigma_count;
  sigma.resize(sigma_count);
  for (int i = 0; i < sigma_count; ++i) {
    std::cin >> sigma[i];
  }
  std::cin >> P_count;
  N.resize(P_count);
  for (int i = 0; i < P_count; ++i) {
    std::cin >> P[i];
  }
  std::cin >> S;
  Earley earley_parser({N, sigma, P, S});
  std::string word;
  std::cin >> word;
  if (earley_parser.ParseWord(word)) {
    std::cout << "Yes";
  } else {
    std::cout << "No";
  }
}