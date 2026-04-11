#pragma once
#include <stdio.h>

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

typedef struct Queue {
    QueueElement* data;
    int size;
    int capacity;
} Queue;

void initQueue(Queue* pq, int capacity);
void freeQueue(Queue* pq);
void swap(QueueElement* a, QueueElement* b);
void heapifyUp(Queue* pq, int index);
void heapifyDown(Queue* pq, int index);
void push(Queue* pq, int distance, int city, int state);
QueueElement pop(Queue* pq);
int isEmpty(Queue* pq);
