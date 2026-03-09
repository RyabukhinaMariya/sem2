#include <stdio.h>
#include <stdlib.h>

int main() {

    // Тест 1
    FILE *f = fopen("input.csv", "w");
    fprintf(f, "a;b\n");
    fprintf(f, "c;d\n");
    fclose(f);

    system("pretty_printer.exe");

    f = fopen("output.txt", "r");
    if (f) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            putchar(c);
        }
        fclose(f);
    }
    printf("\n");

    // Тест 2
    f = fopen("input.csv", "w");
    fprintf(f, "hello;world\n");
    fclose(f);

    system("pretty_printer.exe");

    f = fopen("output.txt", "r");
    if (f) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            putchar(c);
        }
        fclose(f);
    }
    printf("\n");

    // Тест 3
    f = fopen("input.csv", "w");
    fprintf(f, "1;2;3\n");
    fprintf(f, "4;5;6\n");
    fprintf(f, "7;8;9\n");
    fclose(f);

    system("pretty_printer.exe");

    f = fopen("output.txt", "r");
    if (f) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            putchar(c);
        }
        fclose(f);
    }
    printf("\n");

    return 0;
}