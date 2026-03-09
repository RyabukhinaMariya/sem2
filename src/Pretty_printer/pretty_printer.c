#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// проверка на число
int isNumber(const char *str, int len)
{
    if (len == 0)
        return 0;

    int dotCount = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1)
                return 0;
        } else if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// поиск максимальных длин ячеек
void findMaxLengths(FILE *file, int *max_len, int num_cols)
{
    int current_len = 0;
    int col = 0;
    int ch;

    rewind(file);

    while ((ch = fgetc(file)) != EOF) {
        if (ch == ';' || ch == '\n') {
            if (current_len > max_len[col]) {
                max_len[col] = current_len;
            }
            current_len = 0;
            col = (ch == '\n') ? 0 : (col + 1) % num_cols;
        } else {
            current_len++;
        }
    }
}

void printCell(FILE *out, char *data, int lenCell, int align)
{
    fprintf(out, "|");
    if (align) {
        fprintf(out, " %*s ", lenCell, data);
    } else {
        fprintf(out, " %-*s ", lenCell, data);
    }
}

void printRow(FILE *out, char *line, int numCols, int *colWidths)
{
    int col = 0;
    int pos = 0;

    while (col < numCols && line[pos] != '\0') {
        char buffer[256] = {0};
        int bufPos = 0;

        while (line[pos] != ';' && line[pos] != '\n' && line[pos] != '\0') {
            buffer[bufPos++] = line[pos++];
        }
        buffer[bufPos] = '\0';

        int isNum = isNumber(buffer, bufPos);

        printCell(out, buffer, colWidths[col], isNum);

        if (line[pos] != '\0') {
            pos++;
        }
        col++;
    }

    fprintf(out, "|\n");
}

void printLine(FILE *out, int *colWidths, int numCols)
{
    fprintf(out, "+");

    for (int i = 0; i < numCols; ++i) {
        for (int j = 0; j < colWidths[i] + 2; ++j) {
            fprintf(out, "-");
        }
        fprintf(out, "+");
    }

    fprintf(out, "\n");
}

int main()
{
    FILE *file = fopen("input.csv", "r");
    if (file == NULL) {
        printf("input file not found\n");
        return 1;
    }

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("output file not found\n");
        fclose(file);
        return 1;
    }

    // подсчет столбцов в первой строке
    int ch;
    int count_col = 1;
    int count_row = 0;

    while ((ch = fgetc(file)) != '\n' && ch != EOF) {
        if (ch == ';') {
            count_col++;
        }
    }
    count_row++;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count_row++;
        }
    }

    int *col_widths = malloc(count_col * sizeof(int));
    for (int i = 0; i < count_col; ++i) {
        col_widths[i] = 0;
    }

    findMaxLengths(file, col_widths, count_col);

    rewind(file);

    char line[1024];

    printLine(out, col_widths, count_col);

    int row_num = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        printRow(out, line, count_col, col_widths);

        if (row_num < count_row - 1) {
            printLine(out, col_widths, count_col);
        }

        row_num++;
    }

    printLine(out, col_widths, count_col);

    free(col_widths);
    fclose(file);
    fclose(out);

    printf("Table written to output.txt\n");
    return 0;
}