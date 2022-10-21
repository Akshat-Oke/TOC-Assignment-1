#ifndef toc_queue
#define toc_queue

#include <stdbool.h>
#include "nfa.h"

typedef struct Node
{
  int data;
  struct Node *next;
} Node;
typedef struct Queue
{
  int size;
  Node *head, *tail;
} Queue;

Queue *createQueue();
void enqueue(Queue *queue, int e);
int dequeue(Queue *q);
void queueDestroy(Queue *queue);
bool queueNotEmpty(Queue *queue);

#endif