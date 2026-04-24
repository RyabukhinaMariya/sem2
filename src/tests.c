#include "states.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testQueueOperations(void)
{
    printf("Running testQueueOperations... ");
    
    Queue pq;
    initQueue(&pq, 10);
    
    assert(pq.data != NULL);
    assert(pq.size == 0);
    assert(pq.capacity == 10);
    assert(isEmpty(&pq));

    push(&pq, 10, 1, 1);
    push(&pq, 5, 2, 2);
    push(&pq, 15, 3, 1);
    
    assert(pq.size == 3);
    assert(!isEmpty(&pq));

    QueueElement elem = pop(&pq);
    assert(elem.distance == 5);
    assert(elem.city == 2);
    assert(elem.state == 2);
    assert(pq.size == 2);
    
    elem = pop(&pq);
    assert(elem.distance == 10);
    assert(elem.city == 1);
    assert(elem.state == 1);
    assert(pq.size == 1);
    
    elem = pop(&pq);
    assert(elem.distance == 15);
    assert(elem.city == 3);
    assert(elem.state == 1);
    assert(pq.size == 0);
    assert(isEmpty(&pq));
    
    freeQueue(&pq);
    printf("PASSED!\n");
}

void testAddEdge(void)
{
    printf("Running testAddEdge... ");
    
    int n = 5;
    Edge **graph = (Edge **)calloc(n + 1, sizeof(Edge *));
    assert(graph != NULL);

    addEdge(graph, 1, 2, 10);
    addEdge(graph, 1, 3, 20);
    addEdge(graph, 2, 4, 15);

    assert(graph[1] != NULL);
    assert(graph[1]->to == 2);
    assert(graph[1]->length == 10);
    
    assert(graph[1]->next != NULL);
    assert(graph[1]->next->to == 3);
    assert(graph[1]->next->length == 20);
    assert(graph[1]->next->next == NULL);
    
    assert(graph[2] != NULL);
    assert(graph[2]->to == 4);
    assert(graph[2]->length == 15);
    assert(graph[2]->next == NULL);
    
    assert(graph[4] != NULL);
    assert(graph[4]->to == 2);
    assert(graph[4]->length == 15);
    
    for (int i = 1; i <= n; i++) {
        Edge *current = graph[i];
        while (current) {
            Edge *next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph);
    printf("PASSED!\n");
}

void testExpansionWithTwoCapitals(void)
{
    printf("Running testExpansionWithTwoCapitals... ");
    
    int n = 4, m = 3;
    Edge **graph = (Edge **)calloc(n + 1, sizeof(Edge *));
    assert(graph != NULL);
    
    addEdge(graph, 1, 2, 10);
    addEdge(graph, 2, 3, 5);
    addEdge(graph, 3, 4, 8);
    
    int k = 2;
    int *capitals = (int *)malloc(k * sizeof(int));
    capitals[0] = 1;
    capitals[1] = 4;
    
    int *owner = (int *)calloc(n + 1, sizeof(int));
    Queue pq;
    initQueue(&pq, m + 10);
    
    int assigned = 0;
    for (int i = 0; i < k; i++) {
        int state = i + 1;
        int cap = capitals[i];
        if (owner[cap] == 0) {
            owner[cap] = state;
            assigned++;
            for (Edge *e = graph[cap]; e; e = e->next) {
                if (owner[e->to] == 0)
                    push(&pq, e->length, e->to, state);
            }
        }
    }
    
    while (assigned < n && !isEmpty(&pq)) {
        QueueElement elem = pop(&pq);
        if (owner[elem.city] != 0)
            continue;
        
        owner[elem.city] = elem.state;
        assigned++;
        
        for (Edge *e = graph[elem.city]; e; e = e->next) {
            if (owner[e->to] == 0) {
                push(&pq, elem.distance + e->length, e->to, elem.state);
            }
        }
    }
    
    assert(owner[1] == 1);
    assert(owner[4] == 2);
    assert(owner[2] == 1);
    assert(owner[3] == 2);

    freeQueue(&pq);
    free(owner);
    free(capitals);
    for (int i = 1; i <= n; i++) {
        Edge *current = graph[i];
        while (current) {
            Edge *next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph);
    
    printf("PASSED!\n");
}

void runTests(void)
{
    printf("--- Starting Unit Tests ---\n");
    
    testQueueOperations();
    testAddEdge();
    testExpansionWithTwoCapitals();
    
    printf("--- All tests completed successfully! ---\n");
}
