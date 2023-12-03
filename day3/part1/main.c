#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>

#define UTILS_IMPL
#include "./utils.h"

#define SCH_AT(s, i, j) (s->data[(i)*(s->cols)+(j)])

#define NOT_PART '.'

struct Schematic {
  char *data;
  size_t rows, cols;
};

void schematic_dump(struct Schematic *sch) {
  for (size_t i = 0; i < sch->rows; ++i) {
    for (size_t j = 0; j < sch->cols; ++j) {
      printf("%c", SCH_AT(sch, i, j));
    }
    putchar('\n');
  }
}

struct Schematic schematic_create(char *data) {
  struct Schematic sch = {
    .data = data,
    .rows = 0,
    .cols = 0,
  };
  for (size_t i = 0; sch.data[i] != '\0'; ++i) {
    if (sch.data[i] == '\n') {
      for (size_t j = i; sch.data[j] != '\0'; ++j) {
        sch.data[j] = sch.data[j+1];
      }
      ++sch.rows;
      --i;
    }
    if (!sch.rows) ++sch.cols;
  }
  return sch;
}

int find_sub(char **start, char **end) {
  char *it = *start;
  while (it && isdigit(*it)) *start = it--;
  it = *end;
  while (it && isdigit(*it)) {
    *end = it;
    ++it;
  }
  it = *start;

  char buff[32];
  memset(buff, '\0', 32);
  size_t len = 0;
  while (it <= *end) {
    // printf("    %c\n", *it);
    buff[len++] = *it;
    *it = '.';
    ++it;
  }

  return atoi(buff);
}

int search_symbol(struct Schematic *sch, size_t row, size_t col) {
  int res = 0;

  // Check left
  if (col > 0 && isdigit(SCH_AT(sch, row, col-1))) {
    char *start = &SCH_AT(sch, row, col-1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check right
  if (col < sch->cols-1 && isdigit(SCH_AT(sch, row, col+1))) {
    char *start = &SCH_AT(sch, row, col+1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check up
  if (row > 0 && isdigit(SCH_AT(sch, row-1, col))) {
    char *start = &SCH_AT(sch, row-1, col);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check down
  if (row < sch->rows-1 && isdigit(SCH_AT(sch, row+1, col))) {
    char *start = &SCH_AT(sch, row+1, col);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check up-left
  if (row > 0 && col > 0 && isdigit(SCH_AT(sch, row-1, col-1))) {
    char *start = &SCH_AT(sch, row-1, col-1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check up-right
  if (row > 0 && col < sch->cols-1 && isdigit(SCH_AT(sch, row-1, col+1))) {
    char *start = &SCH_AT(sch, row-1, col+1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check down-left
  if (row < sch->rows-1 && col > 0 && isdigit(SCH_AT(sch, row+1, col-1))) {
    char *start = &SCH_AT(sch, row+1, col-1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  // Check down-right
  if (row < sch->rows-1 && col < sch->cols-1 && isdigit(SCH_AT(sch, row+1, col+1))) {
    char *start = &SCH_AT(sch, row+1, col+1);
    char *end = start;
    int r = find_sub(&start, &end);
    printf("  found %d\n", r);
    printf("  adding %d\n", r);
    res += r;
  }

  return res;
}

int sum_part_nums(struct Schematic *sch) {
  int sum = 0;
  for (size_t i = 0; i < sch->rows; ++i) {
    for (size_t j = 0; j < sch->cols; ++j) {
      char c = SCH_AT(sch, i, j);
      if (c == NOT_PART || isdigit(c) || c == '\n') continue;
      // Found symbol
      printf("%c\n", c);
      sum += search_symbol(sch, i, j);
    }
  }
  return sum;
}

int main(void) {
  struct Schematic sch = schematic_create(file_to_str(UTILS_FILEPATH));
  int s = sum_part_nums(&sch);
  printf("%d\n", s);
  // schematic_dump(&sch);
  return 0;
}
