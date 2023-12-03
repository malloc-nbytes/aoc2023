#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define UTILS_FILEPATH "./input.txt"

#ifdef UTILS_IMPL

char *file_to_str(const char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (!fp) {
    fprintf(stderr, "ERR: Failed to open %s. Reason: %s\n", filepath, strerror(errno));
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  size_t file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buffer = (char *)malloc(file_size + 1);
  size_t read_size = fread(buffer, 1, file_size, fp);

  if (read_size != file_size) {
    fprintf(stderr, "ERR: error when reading file. Reason: %s\n",
            strerror(errno));
    fclose(fp);
    free(buffer);
    exit(EXIT_FAILURE);
  }

  buffer[file_size] = '\0';
  fclose(fp);

  return buffer;
}

#endif // UTILS_IMPL
#endif // UTILS_H
