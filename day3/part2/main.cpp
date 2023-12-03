#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

#define FILEPATH "./input.txt"

struct Matrix {
  std::vector<std::vector<char>> data;
  size_t rows, cols;

  Matrix(std::string filepath) {
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

    rows = lines.size();
    cols = (rows > 0) ? lines[0].size() : 0;

    data.resize(rows, std::vector<char>(cols));
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        data[i][j] = lines[i][j];
      }
    }
  }

  void print() const {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        std::cout << data[i][j];
      }
      std::cout << '\n';
    }
    std::printf("rows: %zu, cols: %zu\n", rows, cols);
  }
};

int search_symbol(Matrix& mat, size_t row, size_t col) {
  std::unordered_set<char*> visited;

  auto build_num = [&](char **st, char **en) -> int {
    char *it = *st;
    while (it && isdigit(*it)) {
      *st = it;
      --it;
    }
    it = *en;
    while (it && isdigit(*it)) {
      *en = it;
      ++it;
    }

    // Check if we have already visited this number.
    if (visited.find(*st) != visited.end()) return 1;
    visited.insert(*st);

    std::string buff(*st, *en+1);
    return std::stoi(buff);
  };

  int count = 0;
  int res = 1;

  // Check up left.
  if (row > 0 && col > 0 && isdigit(mat.data[row-1][col-1])) {
    char *st = &mat.data[row-1][col-1];
    char *en = &mat.data[row-1][col-1];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check up.
  if (row > 0 && isdigit(mat.data[row-1][col])) {
    char *st = &mat.data[row-1][col];
    char *en = &mat.data[row-1][col];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check up right.
  if (row > 0 && col < mat.cols-1 && isdigit(mat.data[row-1][col+1])) {
    char *st = &mat.data[row-1][col+1];
    char *en = &mat.data[row-1][col+1];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check left.
  if (col > 0 && isdigit(mat.data[row][col-1])) {
    char *st = &mat.data[row][col-1];
    char *en = &mat.data[row][col-1];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check right.
  if (col < mat.cols-1 && isdigit(mat.data[row][col+1])) {
    char *st = &mat.data[row][col+1];
    char *en = &mat.data[row][col+1];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check down left.
  if (row < mat.rows-1 && col > 0 && isdigit(mat.data[row+1][col-1])) {
    char *st = &mat.data[row+1][col-1];
    char *en = &mat.data[row+1][col-1];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check down.
  if (row < mat.rows-1 && isdigit(mat.data[row+1][col])) {
    char *st = &mat.data[row+1][col];
    char *en = &mat.data[row+1][col];
    res *= build_num(&st, &en);
    ++count;
  }

  // Check down right.
  if (row < mat.rows-1 && col < mat.cols-1 && isdigit(mat.data[row+1][col+1])) {
    char *st = &mat.data[row+1][col+1];
    char *en = &mat.data[row+1][col+1];
    res *= build_num(&st, &en);
    ++count;
  }

  std::printf("  count: %d\n", count);
  return count == 2 ? res : 0;
}

int find_num_parts(Matrix& mat) {
  int sum = 0;
  for (size_t i = 0; i < mat.rows; ++i) {
    for (size_t j = 0; j < mat.cols; ++j) {
      char c = mat.data[i][j];
      if (c != '*') continue;
      std::printf("Symbol: %c\n", c);
      sum += search_symbol(mat, i, j);
    }
  }

  return sum;
}

int main(void) {
  Matrix mat(std::string(FILEPATH));
  int s = find_num_parts(mat);
  std::printf("sum: %d\n", s);
  mat.print();
  return 0;
}
