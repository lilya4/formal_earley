#ifndef EARLEY_H
#define EARLEY_H
#include <string>
#include <vector>

class Grammar {
public:
  struct Rule;
  Grammar(const std::vector<char> &N, const std::vector<char> &sigma, const std::vector <std::string> &rules, char S);
};

class Earley {
public:
  Earley(const Grammar &grammar);
  bool ParseWord(const std::string &word);
};

#endif
