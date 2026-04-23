#include "airports.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printAirportName(char* code, Node* root) {
    Node* node = findNode(code, root);
    if (node) printf("%s -> %s\n", code, node->name);
}

void saveToFile(Node* root, FILE* file) {
    if (!root) return;
    saveToFile(root->left, file);
    fprintf(file, "%s:%s\n", root->iataCode, root->name);
    saveToFile(root->right, file);
}

void processCommand(char* input, AVLTree* tree) {
    input[strcspn(input, "\n")] = '\0';
    char command[20], argument[1024];

    if (sscanf(input, "%s %[^\n]", command, argument) < 1) return;

    if (strcmp(command, "find") == 0) {
        char code[4];
        sscanf(argument, "%3s", code);
        printAirportName(code, tree->root);
    } 
    else if (strcmp(command, "add") == 0) {
        char code[4], name[1024];
        char* colon = strchr(argument, ':');
        if (colon) {
            int len = colon - argument;
            strncpy(code, argument, len > 3 ? 3 : len);
            code[len > 3 ? 3 : len] = '\0';
            strncpy(name, colon + 1, 1023);
            tree->root = insertNode(tree->root, code, name);
        }
    }
    else if (strcmp(command, "save") == 0) {
        FILE* f = fopen("airports.txt", "w");
        if (f) { saveToFile(tree->root, f); fclose(f); }
    }
    else if (strcmp(command, "quit") == 0) exit(0);
}
