#ifndef toc_queue
#define toc_queue

#include <stdbool.h>
#include "nfa.h"
#include "uint128.h"

typedef struct Node
{
  union_state data;
  struct Node *next;
} Node;
typedef struct Queue
{
  int size;
  Node *head, *tail;
} Queue;

Queue *createQueue();
void enqueue(Queue *queue, union_state e);
union_state dequeue(Queue *q);
void queueDestroy(Queue *queue);
bool queueNotEmpty(Queue *queue);

#endif