#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Edge {
    int to;
    int length;
    struct Edge* next;
} Edge;

typedef struct QueueElement {
    int distance;
    int city;
    int state;
} QueueElement;

typedef struct PriorityQueue {
    QueueElement* data;
    int size;
    int capacity;
} PriorityQueue;

void initQueue(PriorityQueue* pq, int capacity) {
    pq->data = (QueueElement*)malloc(capacity * sizeof(QueueElement));
    pq->size = 0;
    pq->capacity = capacity;
}

void freeQueue(PriorityQueue* pq) {
    free(pq->data);
}

void swap(QueueElement* a, QueueElement* b) {
    QueueElement temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->data[parent].distance > pq->data[index].distance) {
            swap(&pq->data[parent], &pq->data[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapifyDown(PriorityQueue* pq, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
        if (left < pq->size && pq->data[left].distance < pq->data[smallest].distance) {
            smallest = left;
        }
        if (right < pq->size && pq->data[right].distance < pq->data[smallest].distance) {
            smallest = right;
        }
        
        if (smallest != index) {
            swap(&pq->data[index], &pq->data[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void push(PriorityQueue* pq, int distance, int city, int state) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->data = (QueueElement*)realloc(pq->data, pq->capacity * sizeof(QueueElement));
    }
    
    pq->data[pq->size].distance = distance;
    pq->data[pq->size].city = city;
    pq->data[pq->size].state = state;
    heapifyUp(pq, pq->size);
    pq->size++;
}

QueueElement pop(PriorityQueue* pq) {
    QueueElement min = pq->data[0];
    pq->size--;
    pq->data[0] = pq->data[pq->size];
    heapifyDown(pq, 0);
    return min;
}

int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

int main() {
    FILE *file = fopen("airports.txt", "r");
    if (file == NULL) {
        printf("airports.txt not found. starting with empty database.\n");
        return 1;
    }
 
    char line[1024];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading n and m\n");
        fclose(file);
        return 1;
    }
    int n, m;
    sscanf(line, "%d %d", &n, &m);
    
    Edge** graph = (Edge**)calloc(n + 1, sizeof(Edge*));
    
    for (int i = 0; i < m; ++i) {
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Error reading road %d\n", i + 1);
            fclose(file);
            return 1;
        }
        int u, v, len;
        sscanf(line, "%d %d %d", &u, &v, &len);
        
        Edge* edge1 = (Edge*)malloc(sizeof(Edge));
        edge1->to = v;
        edge1->length = len;
        edge1->next = graph[u];
        graph[u] = edge1;
        
        Edge* edge2 = (Edge*)malloc(sizeof(Edge));
        edge2->to = u;
        edge2->length = len;
        edge2->next = graph[v];
        graph[v] = edge2;
    }
    
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading k\n");
        fclose(file);
        return 1;
    }
    int k;
    sscanf(line, "%d", &k);
    
    int* capitals = (int*)malloc(k * sizeof(int));
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading capitals\n");
        fclose(file);
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    for (int i = 0; i < k && token != NULL; i++) {
        capitals[i] = atoi(token);
        token = strtok(NULL, " \t\n");
    }
    
    int capitals_read = (token == NULL) ? k : 0;
    while (capitals_read < k) {
        if (fgets(line, sizeof(line), file) == NULL) break;
        token = strtok(line, " \t\n");
        while (token != NULL && capitals_read < k) {
            capitals[capitals_read++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }
    }
    
    fclose(file);
    
    int* owner = (int*)calloc(n + 1, sizeof(int));
    
    int** states = (int**)malloc((k + 1) * sizeof(int*));
    int* states_size = (int*)calloc(k + 1, sizeof(int));
    int* states_capacity = (int*)calloc(k + 1, sizeof(int));
    
    for (int i = 1; i <= k; i++) {
        states_capacity[i] = 10;
        states[i] = (int*)malloc(states_capacity[i] * sizeof(int));
    }
    
    PriorityQueue pq;
    initQueue(&pq, m + 10);
    int assigned = k;
    for (int state = 1; state <= k; state++) {
        int capital = capitals[state - 1];
        owner[capital] = state;
        
        if (states_size[state] >= states_capacity[state]) {
            states_capacity[state] *= 2;
            states[state] = (int*)realloc(states[state], states_capacity[state] * sizeof(int));
        }
        states[state][states_size[state]++] = capital;
        
        Edge* e = graph[capital];
        while (e != NULL) {
            if (owner[e->to] == 0) {
                push(&pq, e->length, e->to, state);
            }
            e = e->next;
        }
    }
    

    while (assigned < n && !isEmpty(&pq)) {
        QueueElement elem = pop(&pq);
        
        if (owner[elem.city] != 0) continue;
        
        owner[elem.city] = elem.state;
        assigned++;
        
        int state = elem.state;
        if (states_size[state] >= states_capacity[state]) {
            states_capacity[state] *= 2;
            states[state] = (int*)realloc(states[state], states_capacity[state] * sizeof(int));
        }
        states[state][states_size[state]++] = elem.city;
        
        Edge* e = graph[elem.city];
        while (e != NULL) {
            if (owner[e->to] == 0) {
                push(&pq, e->length, e->to, state);
            }
            e = e->next;
        }
    }
    
    // Очистка памяти
    freeQueue(&pq);
    free(owner);
    free(states_size);
    free(states_capacity);
    
    for (int i = 1; i <= k; i++) {
        free(states[i]);
    }
    free(states);
    free(capitals);
    
    for (int i = 1; i <= n; i++) {
        Edge* e = graph[i];
        while (e != NULL) {
            Edge* next = e->next;
            free(e);
            e = next;
        }
    }
    free(graph);
    
    return 0;
}
