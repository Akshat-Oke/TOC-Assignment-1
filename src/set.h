#ifndef toc_array
#define toc_array
#include <stdbool.h>
#include <stdint.h>
#include "uint128.h"

typedef struct
{
  union_state e1;
  union_state e2;
} Pair;

typedef struct Set
{
  int length;
  int capacity;
  Pair **data;
} Set;

Pair *newPair(union_state, union_state);
bool pairEquals(const Pair *, const Pair *);
void pairDestroy(Pair *);

Set *createSet();
Set *createSetFrom(const Set *s);
void setInsertP(Set *set, Pair *p);
void setInsertOne(Set *set, union_state e);
void setInsert(Set *set, union_state e1, union_state e2);
bool setHasP(Set *set, Pair *p);
bool setHasOne(Set *set, union_state e);
bool setHas(Set *set, union_state e1, union_state e2);
void setRemoveP(Set *set, Pair *p);
void setRemoveOne(Set *set, union_state e);
void setRemove(Set *set, union_state e1, union_state e2);
union_state *setFlatten(Set *set);
void setPrintAll(Set *set);
void setPrintAllAsOne(Set *set);
void setDestroy(Set *set);

#endif