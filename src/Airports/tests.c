#include "airports.h"
#include <stdio.h>

void testInsertAndFind(void) {
    printf("Test insert and find\n");
    AVLTree* tree = createAVL();

    tree = avlInsert(tree, "KRR", "Pashkovsky");

    assert(strcmp(find(tree, "KRR"), "Pashkovsky") == 0);
    assert(find(tree, "ZZZ") == NULL);

    freeAVL(tree);
}

void testDelete(void) {
    printf("Test Simple Delete\n");
    AVLTree* tree = createAVL();

    tree = avlInsert(tree, "KRR", "Pashkovsky");
    tree = avlInsert(tree, "QQQ", "Airport2");
    tree = avlInsert(tree, "WWW", "Airport3");

    tree = avlDeleteNode(tree, "KRR");
    assert(find(tree, "KRR") == NULL);
    assert(find(tree, "AAA") != NULL);
    assert(find(tree, "BBB") != NULL);

    freeAVL(tree);
}

void testGeneral(void) {
    printf("Test Interface\n");
    char* testFile = "airports.txt";

    FILE* open = fopen(testFile, "w");
    fprintf(open, "KRR:Pashkovsky\n");
    fprintf(open, "AAA:Airport1\n");
    fclose(open);

    AVLTree* tree = load(testFile);
    assert(tree != NULL);
    assert(strcmp(find(tree, "KRR"), "Pashkovsky") == 0);
    assert(strcmp(find(tree, "AAA"), "Airport1") == 0);

    tree = add(tree, "BBB:Aiport2");
    assert(strcmp(find(tree, "BBB"), "Airport2") == 0);

    char* outputFile = "testOutput.txt";
    saveToFile(tree, outputFile);
    freeAVL(tree);

    AVLTree* tree2 = load(outputFile);
    assert(tree2 != NULL);
    assert(find(tree2, "LED") != NULL);
    assert(find(tree2, "AER") != NULL);

    freeAVL(tree2);
    remove(testFile);
    remove(outputFile);
}

void runTests(void) {
    testInsertAndFind();
    testDelete();
    testGeneral();
    printf("All tests passed!\n");
}
