#include "airports.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getHeight(Node *n) { return n ? n->height : 0; }

int max(int a, int b) { return (a > b) ? a : b; }

int getBalance(Node *n) {
  return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

void updateHeight(Node *n) {
  if (n)
    n->height = 1 + max(getHeight(n->left), getHeight(n->right));
}

Node *rotateRight(Node *a) {
  Node *b = a->left;
  Node *T2 = b->right;
  b->right = a;
  a->left = T2;
  updateHeight(a);
  updateHeight(b);
  return b;
}

Node *rotateLeft(Node *a) {
  Node *b = a->right;
  Node *T2 = b->left;
  b->left = a;
  a->right = T2;
  updateHeight(a);
  updateHeight(b);
  return b;
}

Node *createNode(char *code, char *name) {
  Node *newNode = malloc(sizeof(Node));
  if (!newNode)
    return NULL;
  strncpy(newNode->iataCode, code, 3);
  newNode->iataCode[3] = '\0';
  strncpy(newNode->name, name, 1023);
  newNode->name[1023] = '\0';
  newNode->left = newNode->right = NULL;
  newNode->height = 1;
  return newNode;
}

Node *balanceNode(Node *root) {
  updateHeight(root);
  int balance = getBalance(root);
  if (balance > 1) {
    if (getBalance(root->left) < 0)
      root->left = rotateLeft(root->left);
    return rotateRight(root);
  }
  if (balance < -1) {
    if (getBalance(root->right) > 0)
      root->right = rotateRight(root->right);
    return rotateLeft(root);
  }
  return root;
}

Node *insertNode(Node *root, char *code, char *name) {
  if (root == NULL) {
    return createNode(code, name);
  }

  int cmp = strcmp(code, root->iataCode);
  if (cmp < 0) {
    root->left = insertNode(root->left, code, name);
  } else if (cmp > 0) {
    root->right = insertNode(root->right, code, name);
  } else {
    strncpy(root->name, name, 1023);
    root->name[1023] = '\0';
    return root;
  }

  updateHeight(root);

  int balance = getBalance(root);

  // LL case
  if (balance > 1 && strcmp(code, root->left->iataCode) < 0) {
    return rotateRight(root);
  }

  // RR case
  if (balance < -1 && strcmp(code, root->right->iataCode) > 0) {
    return rotateLeft(root);
  }

  // LR case
  if (balance > 1 && strcmp(code, root->left->iataCode) > 0) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }

  // RL case
  if (balance < -1 && strcmp(code, root->right->iataCode) < 0) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }

  return root;
}

Node *findNode(char *code, Node *root) {
  if (!root)
    return NULL;

  int cmp = strcmp(code, root->iataCode);
  if (cmp < 0)
    return findNode(code, root->left);
  else if (cmp > 0)
    return findNode(code, root->right);
  else
    return root;
}

Node *findMinNode(Node *root) {
  if (!root || !root->left)
    return root;
  return findMinNode(root->left);
}

Node *deleteNode(Node *root, char *code) {
  if (root == NULL) {
    printf("Code %s not found for deletion\n", code);
    return NULL;
  }

  int cmp = strcmp(code, root->iataCode);

  if (cmp < 0) {
    root->left = deleteNode(root->left, code);
  } else if (cmp > 0) {
    root->right = deleteNode(root->right, code);
  } else {
    if (root->left == NULL || root->right == NULL) {
      Node *temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        free(root);
        root = NULL;
      } else {
        // Copy data safely
        strncpy(root->iataCode, temp->iataCode, 3);
        root->iataCode[3] = '\0';
        strncpy(root->name, temp->name, 1023);
        root->name[1023] = '\0';

        // Copy children
        root->left = temp->left;
        root->right = temp->right;
        root->height = temp->height;
        free(temp);
      }
      printf("Node with code %s deleted\n", code);
    } else {
      Node *temp = findMinNode(root->right);
      strncpy(root->iataCode, temp->iataCode, 3);
      root->iataCode[3] = '\0';
      strncpy(root->name, temp->name, 1023);
      root->name[1023] = '\0';
      root->right = deleteNode(root->right, temp->iataCode);
    }
  }

  if (root == NULL) {
    return root;
  }

  return balanceNode(root);
}

void freeTree(Node *root) {
  if (!root)
    return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}
