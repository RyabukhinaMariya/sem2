#pragma once
#include <stdio.h>

typedef struct Node {
    char iataCode[4];
    char name[1024];
    struct Node* left;
    struct Node* right;
    int height;
} Node;

typedef struct AVLTree {
    Node* root;
} AVLTree;

Node* createNode(char* code, char* name);
int getHeight(Node* node);
int getBalance(Node* node);
int max(int a, int b);
void updateHeight(Node* node);
Node* rotateLeft(Node* a);
Node* rotateRight(Node* a);
Node* insertNode(Node* root, char* code, char* name);
Node* findMinNode(Node* root);
Node* balanceNode(Node* root);
Node* deleteNode(Node* root, char* code);
Node* findNode(char* code, Node* root);
void printAirportName(char* code, Node* root);
void saveToFile(Node* root, FILE* file);
void freeTree(Node* root);
void processCommand(char* input, AVLTree* tree);
