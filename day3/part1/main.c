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
  printf("got: ");
  while (it <= *end) putchar(*it++);
  putchar('\n');

  return -1;
}

void search_symbol(struct Schematic *sch, size_t row, size_t col) {
  // Check left
  if (col > 0 && isdigit(SCH_AT(sch, row, col-1))) {
    char *start = &SCH_AT(sch, row, col-1);
    char *end = start;
    find_sub(&start, &end);
  }

  // Check right
  if (col < sch->cols-1 && isdigit(SCH_AT(sch, row, col+1))) {
    char *start = &SCH_AT(sch, row, col+1);
    char *end = start;
    find_sub(&start, &end);
  }

  // Check up
  if (row > 0 && isdigit(SCH_AT(sch, row-1, col))) {
    char *start = &SCH_AT(sch, row-1, col);
    char *end = start;
    find_sub(&start, &end);
  }

  // Check down
  if (row < sch->rows-1 && isdigit(SCH_AT(sch, row+1, col))) {
    char *start = &SCH_AT(sch, row+1, col);
    char *end = start;
    find_sub(&start, &end);
  }

  // check up-left
  if (row > 0 && col > 0 && isdigit(SCH_AT(sch, row-1, col-1))) {
    char *start = &SCH_AT(sch, row-1, col-1);
    char *end = start;
    find_sub(&start, &end);
  }

  // check up-right
  if (row > 0 && col < sch->cols-1 && isdigit(SCH_AT(sch, row-1, col+1))) {
    char *start = &SCH_AT(sch, row-1, col+1);
    char *end = start;
    find_sub(&start, &end);
  }

  // check down-left
  if (row < sch->rows-1 && col > 0 && isdigit(SCH_AT(sch, row+1, col-1))) {
    char *start = &SCH_AT(sch, row+1, col-1);
    char *end = start;
    find_sub(&start, &end);
  }

  // check down-right
  if (row < sch->rows-1 && col < sch->cols-1 && isdigit(SCH_AT(sch, row+1, col+1))) {
    char *start = &SCH_AT(sch, row+1, col+1);
    char *end = start;
    find_sub(&start, &end);
  }
}

int sum_part_nums(struct Schematic *sch) {
  char buff[32];
  size_t n = sizeof(buff)/sizeof(buff[0]);
  size_t len = 0;
  memset(buff, '\0', n);
  for (size_t i = 0; i < sch->rows; ++i) {
    for (size_t j = 0; j < sch->cols; ++j) {
      char c = SCH_AT(sch, i, j);
      if (c == NOT_PART || isdigit(c)) continue;
      printf("c: %c\n", c);

      // Found symbol
      search_symbol(sch, i, j);
    }
    memset(buff, '\0', n);
    len = 0;
  }
}

int main(void) {
  struct Schematic sch = schematic_create(file_to_str(UTILS_FILEPATH));
  int s = sum_part_nums(&sch);
  return 0;
}
