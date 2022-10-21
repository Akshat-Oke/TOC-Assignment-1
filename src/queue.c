#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "nfa.h"
#include "queue.h"

Queue *createQueue()
{
  Queue *q = malloc(sizeof(Queue));
  q->size = 0;
  q->head = NULL;
  q->tail = NULL;
  return q;
}
void enqueue(Queue *q, int e)
{
  Node *n = malloc(sizeof(Node));
  n->data = e;
  n->next = NULL;
  if (q->head == NULL)
  {
    q->head = n;
    q->tail = n;
  }
  else
  {
    q->tail->next = n;
    q->tail = n;
  }
  q->size++;
}
int dequeue(Queue *q)
{
  if (q->head == NULL)
  {
    fprintf(stderr, "Dequeueing an empty queue\n");
    exit(1);
  }
  Node *n = q->head;
  q->head = n->next;
  q->size--;
  int e = n->data;
  free(n);
  if (q->head == NULL)
    q->tail = NULL;
  return e;
}
void queueDestroy(Queue *q)
{
  // Node *next = q->head; //->next;
  // while (next != NULL)
  // {
  //   Node *nn = next->next;
  //   free(next);
  //   next = nn;
  // }
  // free(q);
}
bool queueNotEmpty(Queue *q)
{
  return q->head != NULL;
  // return q->size > 0;
}