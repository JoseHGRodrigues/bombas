#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
  void *data;
  struct node *next;
} Node;

typedef struct {
  Node *front;
  Node *rear;
  int size;
} queue;

Queue queueInit() {
  queue *q = malloc(sizeof(queue));
  if (!q)
    return NULL;
  q->front = NULL;
  q->rear = NULL;
  q->size = 0;
  return (Queue)q;
}

void queueFree(Queue s) {
  if (!s)
    return;
  queue *q = (queue *)s;
  Node *currentNode = q->front;
  while (currentNode != NULL) {
    Node *nodeToFree = currentNode;
    currentNode = currentNode->next;
    free(nodeToFree);
  }
  free(q);
}

bool queueIsEmpty(Queue s) {
  if (!s)
    return true;
  queue *q = (queue *)s;
  return q->size == 0;
}

int queueEnqueue(Queue s, void *data) {
  if (!s)
    return 0;
  queue *q = (queue *)s;
  Node *newNode = malloc(sizeof(Node));
  if (!newNode)
    return 0;
  newNode->data = data;
  newNode->next = NULL;
  if (queueIsEmpty(s))
    q->front = newNode;
  else
    q->rear->next = newNode;
  q->rear = newNode;
  q->size++;
  return 1;
}

void *queueDequeue(Queue s) {
  if (!s || queueIsEmpty(s))
    return NULL;
  queue *q = (queue *)s;
  Node *nodeToDequeue = q->front;
  void *data = nodeToDequeue->data;
  q->front = nodeToDequeue->next;
  if (!q->front)
    q->rear = NULL;
  free(nodeToDequeue);
  q->size--;
  return data;
}

int queueGetSize(Queue s) {
  if (!s)
    return -1;
  queue *q = (queue *)s;
  return q->size;
}
