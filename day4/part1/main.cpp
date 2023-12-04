#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#define FILEPATH "./input.txt"

std::vector<std::string> file_to_vec(std::string filepath) {
  std::ifstream fp(filepath);
  if (!fp.is_open()) {
    std::fprintf(stderr, "Could not open file.");
    std::exit(EXIT_FAILURE);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(fp, line)) {
    lines.push_back(line);
  }
  fp.close();

  return lines;
}

int determine_score(std::string& card) {
  std::stringstream ss(card);
  std::string tok;
  std::unordered_set<std::string> win_nums;
  int score = 0;
  bool yours = false;

  while (ss >> tok) {
    if (tok == "Card") {
      // Consume card id.
      ss >> tok;
      continue;
    }
    
    if (tok == "|") {
      yours = true;
      continue;
    }

    if (yours && win_nums.count(tok)) {
      score = score == 0 ? 1 : score * 2;
    } 
    else if (!yours) {
      win_nums.insert(tok);
    }
  }

  return score;
}

int main(void) {
  std::vector<std::string> str = file_to_vec(FILEPATH);
  int score = 0;
  
  for (auto& line : str) {
    // Removing the ':'.
    size_t pos = line.find(':');
    line.erase(pos, 1);

    score += determine_score(line);
  }

  std::printf("score: %d\n", score);
  return 0;
}
