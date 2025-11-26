#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
  Data d;
  struct node *next;
}*Node;

typedef struct{
  Node head;
} list;

static Node nodeInit(){
  Node new = (Node)malloc(sizeof(struct node));
  if(!new)
    return NULL;
  new->next = NULL;
  new->d = NULL;
  return new;
}

static Node nodeSearchPos(list *l, Pos p){
  list *li= (list *)l;
  if(!li)
    return NULL;
  
  Node pos = li->head;

  for(int i =0; pos != NULL, i < p; i++){
    pos = pos->next;
  }
  
  return pos;
}

List listInit(){
  list *l = malloc(sizeof(list));
  if(!l)
    return NULL;
  l->head = NULL;
  return (List) l;
}

bool listFree(List l){
  if(!l)
    return true;
  list *li =(list *)l;
  free(l);
}

bool listInsertHead(List l, Data d){
  if(!l)
    return false;
  list *li = (list *)l;
  Node new = nodeInit();
  if(!new)
    return false;
  new->d = d;
  new->next=li->head;
  li->head=new;
  if(li->head == new)
    return true;
  return false;
}

bool listInsertAtPos(List l, Data d, Pos p){
  list *li = (list*)l;
  if(!li)
    return false;
  Node new = nodeInit();
  if(!new)
    return false;
  new->d = d;
  Node pos = nodeSearchPos(li, p);
  if(!pos)
    return false;
  new->next = pos->next;
  pos->next = new;
  return true;
}

bool listInsertAtLast(List l, Data d){
  list *li = (list *)l;
  if(!li)
    return false;
  Node new = nodeInit();
  if(!new)
    return false;
  new->d = d;
  new->next = NULL;
  Node pos = li->head;
  while(pos->next !=NULL){
    pos = pos->next;
  }
  pos->next = new;
  return true;
}

bool listGetHead(List l, Data d){
  list *li = (list *)l;
  if(!li)
    return false;
  Node temp = li->head;
  li->head = temp->next;
  d = temp->d;
  free(temp);
  return true;
}

bool listGetPos(List l, Data d, Pos p){
  list *li = (list*)l;
  if(!li)
    return false;
  Node pos = nodeSearchPos(li,p);
  if(!pos)
    return false;
  Node temp = pos->next;
  pos->next = temp->next;
  d = temp->d;
  free(temp);
  return true;
}

bool listGetLast(List l, Data d){
  list *li = (list*)l;
  if(!li)
    return false;
  Node pos = li->head;
  Node temp = NULL;
  while(pos->next != NULL){
    temp = pos;
    pos = pos->next;
  }
  d = pos->d;
  temp->next =NULL;
  free(pos);
  return true;
}
