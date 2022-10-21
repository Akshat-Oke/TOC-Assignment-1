#ifndef toc_dsu
#define toc_dsu
// Disjoint set union

void init_dsu(int numberOfNodes);
void make_set(int v);
int find_set(int v);
void union_sets(int a, int b);

#endif