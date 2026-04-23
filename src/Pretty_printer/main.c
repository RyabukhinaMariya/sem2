#include "prettyPrinter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ','
#define MAX_LINE_SIZE 1024

int main() {
  FILE *input = fopen("input.csv", "r");
  FILE *output = fopen("output.txt", "w");

  if (!input || !output) {
    perror("cannot open the files");
    if (input) fclose(input);
    if (output) fclose(output);
    return 1;
  }

  TableConfig *config = createTableConfig(input, DELIMITER);
  if (!config) {
    fclose(input);
    fclose(output);
    return 1;
  }

  rewind(input);
  char line[MAX_LINE_SIZE];

  printSeparatorLine(output, config);
  while (fgets(line, sizeof(line), input)) {
    if (strlen(line) <= 1) continue;

    printRow(output, line, config, DELIMITER);
    printSeparatorLine(output, config);
  }

  freeTableConfig(config);
  fclose(input);
  fclose(output);

  printf("table is succesfully writen to output.txt\n");
  return 0;
}
