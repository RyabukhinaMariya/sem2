#pragma once
#include <stdio.h>

typedef struct {
    int numCols;
    int *colWidths;
} TableConfig;

int is_number(const char *str);

TableConfig* createTableConfig(FILE *file, char delimiter);

void freeTableConfig(TableConfig *config);

void printSeparatorLine(FILE *output, TableConfig *config);

void printRow(FILE *output, char *line, TableConfig *config, char delimiter);