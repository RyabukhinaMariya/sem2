#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Node* createNode(char* code, char* name) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }

    strncpy(newNode->iataCode, code, 3);
    newNode->iataCode[3] = '\0';

    strncpy(newNode->name, name, 1023);
    newNode->name[1023] = '\0';

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height =1;

    return newNode;
}

int getHeight(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int getBalance(Node* node) {
    if (node == NULL) {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);

}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void updateHeight(Node* node) {
    if (node != NULL) {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

Node* rotateLeft(Node* a) { 
    Node* b = a->right;
    Node* T2 = b->left;

    b->left = a;
    a->right = T2;

    updateHeight(a);
    updateHeight(b);

    return b;
}

Node* rotateRight(Node* a) { 
    Node* b = a->left;
    Node* T2 = b->right;

    b->right = a;
    a->left = T2;

    updateHeight(a);
    updateHeight(b);

    return b;
}

Node* insertNode(Node* root, char* code, char* name) {
    if (root == NULL) {
        return createNode(code, name);
    }

    int cmp = strcmp(code, root->iataCode);
    if (cmp < 0) {
        root->left = insertNode(root->left, code, name);
    }
    else if (cmp > 0) {
        root->right = insertNode(root->right, code, name);
    }
    else {
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

Node* findMinNode(Node* root) {
    Node* current = root;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* balanceNode(Node* root) { 
    updateHeight(root);
    
    int balance = getBalance(root);

    // LL case
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rotateRight(root);
    }

    // RR case
    if (balance < -1 && getBalance(root->right) <= 0) {
        return rotateLeft(root);
    }

    // LR case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* deleteNode(Node* root, char* code) {
    if (root == NULL) {
        printf("Code %s not found for deletion\n", code);
        return NULL;
    }

    int cmp = strcmp(code, root->iataCode);
    
    if (cmp < 0) {
        root->left = deleteNode(root->left, code);
    }
    else if (cmp > 0) {
        root->right = deleteNode(root->right, code);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root->left ? root->left : root->right;
            
            if (temp == NULL) {
                free(root);
                root = NULL;
            }
            else {
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
        }
        else {
            Node* temp = findMinNode(root->right);
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

Node* findNode(char* code, Node* root) {
    if (root == NULL) {
        printf("Code is not found\n");
        return NULL;
    }
    
    int cmp = strcmp(code, root->iataCode);
    if (cmp < 0) {
        return findNode(code, root->left);
    }
    else if (cmp > 0) {
        return findNode(code, root->right);
    }
    else {
        return root;
    }
}

void printAirportName(char* code, Node* root) {
    Node* node = findNode(code, root);
    if (node != NULL) {
        printf("%s -> %s\n", code, node->name);
    }
}

void saveToFile(Node* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    
    saveToFile(root->left, file);
    fprintf(file, "%s:%s\n", root->iataCode, root->name);
    saveToFile(root->right, file);
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void processCommand(char* input, AVLTree* tree) {
    // Remove newline character
    input[strcspn(input, "\n")] = '\0';
    
    // Parse command
    char command[20];
    char argument[1024];
    
    if (sscanf(input, "%s %[^\n]", command, argument) < 1) {
        printf("Invalid command\n");
        return;
    }
    
    if (strcmp(command, "find") == 0) {
        if (strlen(argument) > 0) {
            char code[4];
            sscanf(argument, "%3s", code);
            printAirportName(code, tree->root);
        } else {
            printf("Usage: find <code>\n");
        }
    }
    else if (strcmp(command, "add") == 0) {
        if (strlen(argument) > 0) {
            char code[4];
            char name[1024];
            char* colonPos = strchr(argument, ':');
            
            if (colonPos != NULL) {
                int codeLen = colonPos - argument;
                if (codeLen > 3) codeLen = 3;
                strncpy(code, argument, codeLen);
                code[codeLen] = '\0';
                
                strncpy(name, colonPos + 1, 1023);
                name[1023] = '\0';
                
                tree->root = insertNode(tree->root, code, name);
                printf("added/updated airport: %s -> %s\n", code, name);
            } else {
                printf("use: add <code>:<name>\n");
            }
        } else {
            printf("usage: add <code>:<name>\n");
        }
    }
    else if (strcmp(command, "delete") == 0) {
        if (strlen(argument) > 0) {
            char code[4];
            sscanf(argument, "%3s", code);
            tree->root = deleteNode(tree->root, code);
        } else {
            printf("usage: delete <code>\n");
        }
    }
    else if (strcmp(command, "save") == 0) {
        FILE* file = fopen("airports.txt", "w");
        if (file == NULL) {
            printf("can't open file for saving\n");
            return;
        }
        saveToFile(tree->root, file);
        fclose(file);
        printf("saved to airports.txt\n");
    }
    else if (strcmp(command, "quit") == 0) {
        exit(0);
    }
    else {
        printf("unknown command. Available commands: find, add, delete, save, quit\n");
    }
}

int main() {
    FILE *file = fopen("airports.txt", "r");
    if (file == NULL) {
        printf("airports.txt not found. starting with empty database.\n");
    }

    AVLTree tree;
    tree.root = NULL;

    if (file != NULL) {
        char line[1024];
        char code[4];
        char name[1024];
        
        while(fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\n")] = '\0';
            
            char *colPos = strchr(line, ':');
            if (colPos != NULL) {
                int codeLen = colPos - line;
                if (codeLen > 3) {
                    codeLen = 3;
                }
                strncpy(code, line, codeLen);
                code[codeLen] = '\0';
                
                strncpy(name, colPos + 1, 1023);
                name[1023] = '\0';
                
                tree.root = insertNode(tree.root, code, name);
            }
        }
        fclose(file);
        printf("Loaded airports from airports.txt\n");
    }
    
    char input[1024];
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        processCommand(input, &tree);
        printf("\n");
    }

    freeTree(tree.root);
    
    return 0;
}