#pragma once
#include <stdio.h>

typedef struct Node {
    char iataCode[4];
    char name[1024];
    struct Node *left;
    struct Node *right;
    int height;
} Node;

typedef struct {
    Node *root;
} AVLTree;

// tree functions
int getHeight(Node* n);
int max(int a, int b);
int getBalance(Node* n);
void updateHeight(Node* n);
Node* rotateRight(Node* a);
Node* rotateLeft(Node* a);
Node* createNode(char* code, char* name);
Node* balanceNode(Node* root);
Node* insertNode(Node* root, char* code, char* name);
Node* deleteNode(Node* root, char* code);
Node* findNode(char* code, Node* root);
void freeTree(Node* root);

// input/output functions
void printAirportName(char* code, Node* root);
void saveToFile(Node* root, FILE* file);
void processCommand(char* input, AVLTree* tree);