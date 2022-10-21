#ifndef toc_array
#define toc_array
#include <stdbool.h>

typedef struct
{
  int e1;
  int e2;
} Pair;

typedef struct Set
{
  int length;
  int capacity;
  Pair **data;
} Set;

Pair *newPair(int, int);
bool pairEquals(const Pair *, const Pair *);
void pairDestroy(Pair *);

Set *createSet();
Set *createSetFrom(const Set *s);
void setInsertP(Set *set, Pair *p);
void setInsertOne(Set *set, int e);
void setInsert(Set *set, int e1, int e2);
bool setHasP(Set *set, Pair *p);
bool setHasOne(Set *set, int e);
bool setHas(Set *set, int e1, int e2);
void setRemoveP(Set *set, Pair *p);
void setRemoveOne(Set *set, int e);
void setRemove(Set *set, int e1, int e2);
int *setFlatten(Set *set);
void setPrintAll(Set *set);
void setPrintAllAsOne(Set *set);
void setDestroy(Set *set);

#endif