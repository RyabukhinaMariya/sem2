#include "states.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addEdge(Edge **graph, int u, int v, int len) {
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->to = v;
    edge->length = len;
    edge->next = graph[u];
    graph[u] = edge;
}

int main() {
    FILE *file = fopen("airports.txt", "r");
    if (!file)
        return 1;

    int n, m;
    fscanf(file, "%d %d", &n, &m);

    Edge **graph = (Edge **)calloc(n + 1, sizeof(Edge *));
    for (int i = 0; i < m; ++i) {
        int u, v, len;
        fscanf(file, "%d %d %d", &u, &v, &len);
        addEdge(graph, u, v, len);
        addEdge(graph, v, u, len);
    }

    int k;
    fscanf(file, "%d", &k);
    int *capitals = (int *)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
        fscanf(file, "%d", &capitals[i]);
    fclose(file);

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

    freeQueue(&pq);
    free(owner);
    free(capitals);

    return 0;
}
