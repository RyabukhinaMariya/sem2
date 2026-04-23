#include "airports.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void testInsertAndFind(void) {
  printf("Test insert and find\n");
  AVLTree tree;
  tree.root = NULL;

  tree.root = insertNode(tree.root, "KRR", "Pashkovsky");

  assert(strcmp(findNode("KRR", tree.root)->name, "Pashkovsky") == 0);
  assert(findNode("ZZZ", tree.root) == NULL);

  freeTree(tree.root);
}

void testDelete(void) {
  printf("Test Simple Delete\n");
  AVLTree tree;
  tree.root = NULL;

  tree.root = insertNode(tree.root, "KRR", "Pashkovsky");
  tree.root = insertNode(tree.root, "QQQ", "Airport2");
  tree.root = insertNode(tree.root, "WWW", "Airport3");

  tree.root = deleteNode(tree.root, "KRR");
  assert(findNode("KRR", tree.root) == NULL);
  assert(findNode("AAA", tree.root) == NULL);
  assert(findNode("BBB", tree.root) == NULL);

  freeTree(tree.root);
}

void testGeneral(void) {
  printf("Test Interface\n");
  char *testFile = "airports.txt";

  FILE *open = fopen(testFile, "w");
  fprintf(open, "KRR:Pashkovsky\n");
  fprintf(open, "AAA:Airport1\n");
  fclose(open);

  AVLTree tree;
  tree.root = NULL;

  // Load from file
  FILE *file = fopen(testFile, "r");
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
  }

  assert(tree.root != NULL);
  assert(strcmp(findNode("KRR", tree.root)->name, "Pashkovsky") == 0);
  assert(strcmp(findNode("AAA", tree.root)->name, "Airport1") == 0);

  tree.root = insertNode(tree.root, "BBB", "Airport2");
  assert(strcmp(findNode("BBB", tree.root)->name, "Airport2") == 0);

  char *outputFile = "testOutput.txt";
  FILE *out = fopen(outputFile, "w");
  if (out) {
    saveToFile(tree.root, out);
    fclose(out);
  }
  freeTree(tree.root);

  // Load from output file
  AVLTree tree2;
  tree2.root = NULL;
  FILE *file2 = fopen(outputFile, "r");
  if (file2) {
    char line[1024];
    while (fgets(line, sizeof(line), file2)) {
      char code[4], name[1024];
      char *colPos = strchr(line, ':');
      if (colPos) {
        *colPos = '\0';
        strncpy(code, line, 3);
        code[3] = '\0';
        strncpy(name, colPos + 1, 1023);
        name[strcspn(name, "\n")] = '\0';
        tree2.root = insertNode(tree2.root, code, name);
      }
    }
    fclose(file2);
  }

  assert(tree2.root != NULL);
  assert(findNode("LED", tree2.root) == NULL);
  assert(findNode("AER", tree2.root) == NULL);

  freeTree(tree2.root);
  remove(testFile);
  remove(outputFile);
}

void runTests(void) {
  testInsertAndFind();
  testDelete();
  testGeneral();
  printf("All tests passed!\n");
}

int main() {
  runTests();
  return 0;
}
