#pragma once
#include <stdio.h>

typedef struct {
    int distance;
    int city;
    int state;
} QueueElement;

typedef struct {
    QueueElement* data;
    int size;
    int capacity;
} Queue;

typedef struct Edge {
    int to;
    int length;
    struct Edge* next;
} Edge;

void initQueue(Queue* pq, int capacity);
void freeQueue(Queue* pq);
void push(Queue* pq, int distance, int city, int state);
QueueElement pop(Queue* pq);
int isEmpty(Queue* pq);
