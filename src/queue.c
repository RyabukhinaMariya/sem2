#include "airports.h"
#include <stdio.h>
#include <string.h>

int main() {
    AVLTree tree;
    tree.root = NULL;

    FILE *file = fopen("airports.txt", "r");

    if (file) {
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            char code[4], name[1024];
            char *colPos = strchr(line, ':');
            if (colPos) {
                *colPos = '\0';
                strncpy(code, line, 3);
                code[3] = '\0';
                strncpy(name, colPos + 1, 1023);
                name[strcspn(name, "\n")] = '\0';
                tree.root = insertNode(tree.root, code, name);
            }
        }
        fclose(file);
        printf("Loaded database.\n");
    }

    char input[1024];
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        processCommand(input, &tree);
        printf("\n");
    }

    freeTree(tree.root);
    return 0;
}
