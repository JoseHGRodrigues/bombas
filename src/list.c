#include "list.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
  void *data;
  struct node *next;
} Node;

typedef struct {
  Node *head;
  int size;
} list;

List listInit() {
  list *l = malloc(sizeof(list));
  if (!l)
    return NULL;
  l->head = NULL;
  l->size = 0;
  return (List)l;
}

void listFree(List l) {
  if (!l)
    return;
  list *li = (list *)l;
  Node *currentNode = li->head;
  while (currentNode != NULL) {
    Node *nodeToFree = currentNode;
    currentNode = currentNode->next;
    free(nodeToFree);
  }
  free(li);
}

bool listIsEmpty(List l) {
  if (!l)
    return true;
  list *li = (list *)l;
  return li->size == 0;
}

bool listAddFirst(List l, void *data) {
  if (!l)
    return false;
  list *li = (list *)l;
  Node *newNode = malloc(sizeof(Node));
  if (!newNode)
    return false;
  
  newNode->data = data;
  newNode->next = li->head;
  
  li->head = newNode;
  li->size++;
  return true;
}

void *listGetFirst(List l) {
  if (!l || listIsEmpty(l))
    return NULL;
  list *li = (list *)l;
  return li->head->data;
}

// --- FUNÇÕES QUE FALTAVAM ---

bool listAddLast(List l, void *data) {
    if (!l) return false;
    list *li = (list *)l;
    
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return false;
    newNode->data = data;
    newNode->next = NULL;

    if (li->head == NULL) {
        li->head = newNode;
    } else {
        Node *aux = li->head;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = newNode;
    }
    
    li->size++;
    return true;
}

void *listGetPos(List l, int pos) {
    if (!l) return NULL;
    list *li = (list *)l;

    if (pos < 0 || pos >= li->size) return NULL;

    Node *aux = li->head;
    for (int i = 0; i < pos; i++) {
        aux = aux->next;
    }
    
    return aux->data;
}

void *listGetLast(List l) {
    if (!l || listIsEmpty(l)) return NULL;
    list *li = (list *)l;
    
    Node *aux = li->head;
    while (aux->next != NULL) {
        aux = aux->next;
    }
    return aux->data;
}

bool listAddPos(List l, void *data, int pos) {
    if (!l) return false;
    list *li = (list *)l;

    if (pos < 0 || pos > li->size) return false;

    if (pos == 0) return listAddFirst(l, data);

    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return false;
    newNode->data = data;

    Node *aux = li->head;
    for (int i = 0; i < pos - 1; i++) {
        aux = aux->next;
    }

    newNode->next = aux->next;
    aux->next = newNode;
    li->size++;
    return true;
}
