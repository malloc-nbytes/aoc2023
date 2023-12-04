#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
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

struct Card {
  int count;
  int id;
  std::vector<std::string> winning;
  std::vector<std::string> called;
  Card(int id, std::vector<std::string> winning, std::vector<std::string> called)
    : count(1), id(id), winning(std::move(winning)), called(std::move(called)) {}

  int get_winning_count(void) const {
    return std::count_if(called.begin(), called.end(), [&](const std::string& call) {
        return std::find(winning.begin(), winning.end(), call) != winning.end();
    });
  }
};

Card make_card(std::string& line) {
  std::stringstream ss(line);
  std::string tok, id;
  std::vector<std::string> winning, called;
  bool yours = false;

  while (ss >> tok) {
    if (tok == "Card") {
      ss >> id; continue;
    }
    if (tok == "|") {
      yours = true; continue;
    }
    if (yours) called.push_back(tok);
    else if (!yours) winning.push_back(tok);
  }

  return Card { std::stoi(id), winning, called, };
}

void add_copies(std::vector<Card>& cards, int start, int wins) {
  int next = cards[start].id+1;
  for (size_t i = start+1; i < cards.size() && wins; ++i) {
    Card& card = cards[i];
    if (card.id == next) {
      next = card.id+1;
      ++card.count;
      --wins;
    }
  }
}

int iter_cards(std::vector<Card>& cards) {
  for (size_t i = 0; i < cards.size(); ++i) {
    Card& card = cards[i];
    int wins = card.get_winning_count();
    for (int j = 0; j < card.count; ++j) {
      add_copies(cards, i, wins);
    }
  }

  int sum = 0;
  for (auto& card : cards) {
    sum += card.count;
  }
  return sum;
}

int main(void) {
  std::vector<std::string> str = file_to_vec(FILEPATH);

  std::vector<Card> cards;
  for (auto& line : str) {
    size_t pos = line.find(':');
    line.erase(pos, 1);
    cards.push_back(make_card(line));
  }
  std::printf("cards: %d\n", iter_cards(cards));

  return 0;
}
