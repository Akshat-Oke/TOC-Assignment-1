#include <stdlib.h>
#include "dsu.h"

int *parent;
void init_dsu(int numberOfNodes)
{
  parent = malloc(sizeof(int) * numberOfNodes);
  for (int i = 0; i < numberOfNodes; i++)
  {
    parent[i] = i;
  }
}
/// @deprecated Not required: Use
/// init_dsu instead
/// @param v Node
void make_set(int v)
{
  parent[v] = v;
}

int find_set(int v)
{
  if (v == parent[v])
    return v;
  return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b)
{
  a = find_set(a);
  b = find_set(b);
  if (a != b)
    parent[b] = a;
}