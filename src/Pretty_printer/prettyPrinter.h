#ifndef PRETTY_PRINTER_H
#define PRETTY_PRINTER_H

#include <stdio.h>

typedef struct {
  int numCols;
  int *colWidths;
} TableConfig;

TableConfig *createTableConfig(FILE *file, char delimiter);
void freeTableConfig(TableConfig *config);
void printSeparatorLine(FILE *output, TableConfig *config);
void printRow(FILE *output, char *line, TableConfig *config, char delimiter);

#endif
