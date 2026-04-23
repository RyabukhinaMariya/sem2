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

  freeTree(tree.root);
}

void runTests(void) {
  testInsertAndFind();
  testDelete();
  printf("All tests passed!\n");
}

int main() {
  runTests();
  return 0;
}
