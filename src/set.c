#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "set.h"
#include "to_dfa.h"
#include "uint128.h"

Pair *newPair(union_state e1, union_state e2)
{
  Pair *p = malloc(sizeof(Pair));
  p->e1 = e1;
  p->e2 = e2;
  // printf("Created pair %d %d\n", p->e1, p->e2);
  return p;
}
static void pairPrint(const Pair *p1, const Pair *p2)
{
  // printf("(%d, %d, %d, %d)\n", p1->e1, p1->e2, p2->e1, p2->e2);
}
bool pairEquals(const Pair *p1, const Pair *p2)
{
  pairPrint(p1, p2);
  return (p1->e1 == p2->e1 && p1->e2 == p2->e2) || (p1->e1 == p2->e2 && p1->e2 == p2->e1);
}
void pairDestroy(Pair *p)
{
  // free(p);
}
Set *createSet()
{
  Set *array = (Set *)malloc(sizeof(Set));
  array->length = 0;
  array->capacity = 0;
  array->data = NULL; //(int *)malloc(sizeof(int) * 32);
  return array;
}
Set *createSetFrom(const Set *s)
{
  Set *set = createSet();
  for (int i = 0; i < s->length; i++)
  {
    setInsertP(set, s->data[i]);
  }
  return set;
}
void setInsert(Set *set, union_state e1, union_state e2)
{
  if (e1 > e2)
  {
    union_state t = e1;
    e1 = e2;
    e2 = t;
  }
  Pair *p = newPair(e1, e2);
  setInsertP(set, p);
}
void setInsertOne(Set *set, union_state e)
{
  setInsert(set, e, e);
}
void setInsertP(Set *set, Pair *p)
{
  if (setHasP(set, p))
    return;
  pairPrint(p, p);
  if (set->length == set->capacity)
  {
    set->capacity = set->capacity * 2 + 1;
    set->data = (Pair **)realloc(set->data, sizeof(Pair *) * set->capacity);
  }
  set->data[set->length] = p;
  set->length++;
}
bool setHas(Set *set, union_state e1, union_state e2)
{
  // check both combinations
  // int ele1 = e1 * 10 + e2;

  // int ele2 = e2 * 10 + e1;
  Pair *p1 = newPair(e1, e2);
  Pair *p2 = newPair(e2, e1);
  bool result = setHasP(set, p1); //|| setHasP(set, p2);
  pairDestroy(p1);
  pairDestroy(p2);
  return result;
}
bool setHasOne(Set *set, union_state e)
{
  return setHas(set, e, e);
}
bool setHasP(Set *set, Pair *p)
{
  for (int i = 0; i < set->length; i++)
  {
    if (pairEquals(p, set->data[i]))
      return true;
  }
  return false;
}
void setRemove(Set *set, union_state e1, union_state e2)
{
  Pair *p1 = newPair(e1, e2);
  Pair *p2 = newPair(e2, e1);
  setRemoveP(set, p1);
  setRemoveP(set, p2);
  pairDestroy(p1);
  pairDestroy(p2);
}
void setRemoveOne(Set *set, union_state e)
{
  setRemove(set, e, e);
}
void setRemoveP(Set *array, Pair *p)
{
  for (int i = 0; i < array->length; i++)
  {
    if (pairEquals(p, array->data[i]))
    {
      array->data[i] = array->data[array->length - 1];
      array->length--;
      return;
    }
  }
}
union_state *setFlatten(Set *s)
{
  union_state *arr = (union_state *)malloc(sizeof(union_state) * s->length);
  for (int i = 0; i < s->length; i++)
  {
    arr[i] = s->data[i]->e1;
  }
  return arr;
}
void setPrintAll(Set *s)
{
  printf("setPrintAll: %d\n", s->length);
  // for (int i = 0; i < s->length; i++)
  // {
  //   printf("(%d %d), ", s->data[i]->e1, s->data[i]->e2);
  // }
  // printf("\n");
}
void setPrintAllAsOne(Set *set)
{
  for (int i = 0; i < set->length; i++)
  {
    printf("%d ", set->data[i]->e1);
  }
  printf("\n");
}
void setDestroy(Set *array)
{
  for (int i = 0; i < array->length; i++)
  {
    pairDestroy(array->data[i]);
  }
  free(array->data);
  free(array);
}