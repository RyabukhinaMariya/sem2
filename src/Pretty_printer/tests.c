#include "prettyPrinter.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFF 4096

int createTestCsv(char filename[], char content[])
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("[ERROR] Can't create a test csv file\n");
        assert(fp != NULL);
        return -1;
    }

    fputs(content, fp);
    fclose(fp);
    return 0;
}

int checkFileContent(char filename[], char expected[])
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("[ERROR] Can't open a test file\n");
        assert(fp != NULL);
        return -1;
    }

    char buff[MAX_BUFF];
    size_t len = fread(buff, 1, sizeof(buff) - 1, fp);
    buff[len] = '\0';

    fclose(fp);
    int result = strcmp(buff, expected);
    if (result != 0) {
        printf("[FAIL] File content mismatch.\n");
        printf("[EXPECTED]\n%s\n", expected);
        printf("[GET]\n%s\n", buff);
        assert(result == 0);
    }

    return 0;
}

void testIsNumber(void)
{
    printf("[RUNNING] Test isNumber:\n");

    assert(isNumber("123") == 1);
    assert(isNumber("42.1") == 1);
    assert(isNumber("-12") == 1);
    assert(isNumber("0.0") == 1);

    assert(isNumber("") == 0);
    assert(isNumber("testing") == 0);
    assert(isNumber("4343adsds121") == 0);
    assert(isNumber("aaaaa323232") == 0);
    assert(isNumber(NULL) == 0);
    assert(isNumber("12.34.56") == 0);


    printf("[SUCCESS] Test isNumber passed!\n");
}

void testFullTablePrinting(void)
{
    printf("[RUNNING] Test full table printing:\n");

    char input[] = "Product,Price,Quantity\n"
                   "Apple,1.50,100\n"
                   "Banana,0.75,250\n"
                   "Orange,2.00,75\n";

    char expected[] = "+---------+-------+----------+\n"
                      "| Product | Price | Quantity |\n"
                      "+---------+-------+----------+\n"
                      "| Apple   |  1.50 |      100 |\n"
                      "+---------+-------+----------+\n"
                      "| Banana  |  0.75 |      250 |\n"
                      "+---------+-------+----------+\n"
                      "| Orange  |  2.00 |       75 |\n"
                      "+---------+-------+----------+\n";

    createTestCsv("test_full.csv", input);

    FILE *inputFile = fopen("test_full.csv", "r");
    FILE *outputFile = fopen("test_output.txt", "w");
    assert(inputFile != NULL);
    assert(outputFile != NULL);

    TableConfig *config = createTableConfig(inputFile, ',');
    assert(config != NULL);

    rewind(inputFile);
    char line[MAX_BUFF];

    printSeparatorLine(outputFile, config);
    if (fgets(line, sizeof(line), inputFile)) {
        if (strlen(line) > 1) {
            printRow(outputFile, line, config, ',');
            printSeparatorLine(outputFile, config);
        }
    }
    while (fgets(line, sizeof(line), inputFile)) {
        if (strlen(line) <= 1)
            continue;
        printRow(outputFile, line, config, ',');
        printSeparatorLine(outputFile, config);
    }

    freeTableConfig(config);
    fclose(inputFile);
    fclose(outputFile);

    checkFileContent("test_output.txt", expected);
    printf("[SUCCESS] Test full table printing passed!\n");
}

void testEmptyFields(void)
{
    printf("[RUNNING] Test empty fields handling:\n");

    char input[] = "ID,Name,Value\n"
                   "1,,100\n"
                   "2,Test,\n"
                   ",,\n";

    char expected[] = "+----+------+-------+\n"
                      "| ID | Name | Value |\n"
                      "+----+------+-------+\n"
                      "|  1 |      |   100 |\n"
                      "+----+------+-------+\n"
                      "|  2 | Test |       |\n"
                      "+----+------+-------+\n"
                      "|    |      |       |\n"
                      "+----+------+-------+\n";

    createTestCsv("test_empty.csv", input);

    FILE *inputFile = fopen("test_empty.csv", "r");
    FILE *outputFile = fopen("test_output.txt", "w");
    assert(inputFile != NULL);
    assert(outputFile != NULL);

    TableConfig *config = createTableConfig(inputFile, ',');
    assert(config != NULL);

    rewind(inputFile);
    char line[MAX_BUFF];

    printSeparatorLine(outputFile, config);
    while (fgets(line, sizeof(line), inputFile)) {
        if (strlen(line) <= 1)
            continue;
        printRow(outputFile, line, config, ',');
        printSeparatorLine(outputFile, config);
    }

    freeTableConfig(config);
    fclose(inputFile);
    fclose(outputFile);

    checkFileContent("test_output.txt", expected);
    printf("[SUCCESS] Test empty fields passed!\n");
}

void runTests(void)
{
    printf("[RUNNING] Starting Tests!\n\n");

    testIsNumber();
    testCreateTableConfig();
    testPrintSeparatorLine();
    testPrintRowAlignment();
    testFullTablePrinting();
    testEmptyFields();

    printf("\n[SUCCESS] ALL Tests passed!\n");
}

int main() {
    runTests();
    return 0;
}
