#include "prettyPrinter.h"
#include <stdlib.h>
#include <string.h>

int isNumber(const char *str) {
  if (!str || *str == '\0')
    return 0;

  int dotCount = 0;
  int i = 0;

  if (str[0] == '-') {
    if (strlen(str) == 1)
      return 0;
    i = 1;
  }

  for (; str[i] != '\0'; i++) {
    if (str[i] == '.') {
      dotCount++;
      if (dotCount > 1)
        return 0;
    } else if (!isdigit(str[i])) {
      return 0;
    }
  }
  return 1;
}

TableConfig *createTableConfig(FILE *file, char delimiter) {
  TableConfig *config = malloc(sizeof(TableConfig));
  if (!config)
    return NULL;

  int ch, currentLen = 0, col = 0, maxCols = 1;

  // find count of columns
  rewind(file);
  while ((ch = fgetc(file)) != '\n' && ch != EOF) {
    if (ch == delimiter)
      maxCols++;
  }

  config->numCols = maxCols;
  config->colWidths = calloc(maxCols, sizeof(int));

  rewind(file);
  col = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == delimiter || ch == '\n') {
      if (currentLen > config->colWidths[col]) {
        config->colWidths[col] = currentLen;
      }
      currentLen = 0;
      col = (ch == '\n') ? 0 : (col + 1) % maxCols;
    } else if (ch != '\r') {
      currentLen++;
    }
  }
  return config;
}

void freeTableConfig(TableConfig *config) {
  if (config) {
    free(config->colWidths);
    free(config);
  }
}

void printSeparatorLine(FILE *output, TableConfig *config) {
  fprintf(output, "+");
  for (int i = 0; i < config->numCols; i++) {
    for (int j = 0; j < config->colWidths[i] + 2; j++)
      fprintf(output, "-");
    fprintf(output, "+");
  }
  fprintf(output, "\n");
}

void printRow(FILE *output, char *line, TableConfig *config, char delimiter) {
  char *token;
  char *rest = line;
  int col = 0;

  fprintf(output, "|");

  while (col < config->numCols) {
    char buffer[256] = {0};
    int i = 0;

    while (*rest != delimiter && *rest != '\n' && *rest != '\r' &&
           *rest != '\0') {
      buffer[i++] = *rest++;
    }
    buffer[i] = '\0';

    int isNum = isNumber(buffer);
    // (%*s) - for right orientation,  (%-*s) - for left
    fprintf(output, " %*s ", isNum ? config->colWidths[col] : -config->colWidths[col],
            buffer);
    fprintf(output, "|");

    if (*rest == delimiter)
      rest++;
    col++;
  }
  fprintf(output, "\n");
}
