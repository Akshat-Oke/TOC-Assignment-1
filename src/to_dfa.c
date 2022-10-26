#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "dsu.h"
#include "nfa.h"
#include "queue.h"
#include "set.h"
#include "to_dfa.h"
#include "uint128.h"
int getIndexOf(union_state state, union_state *states, int numberOfStates);

static void printArray(union_state *arr, int size)
{
  for (int i = 0; i < size; i++)
  {
    // printf("%d ", arr[i]);
    print_uint128_no_line(arr[i]);
    printf(" ");
  }
  printf("\n");
}

static void printTransitions(Transition *head, int input)
{
  while (head != NULL)
  {
    // printf("%d\n", head != NULL);
    // printf("(%d, '%d')->%d\n", head->from, input, head->to);
    printf("(");
    print_uint128_no_line(head->from);
    printf(", '%d')->", input);
    print_uint128_no_line(head->to);
    printf("\n");
    head = head->next;
  }
}
static void debugPrintDFA(DFA *dfa)
{
  printf("=====Printing DFA=====\nStates:\n");
  printArray(dfa->states, dfa->numberOfStates);
  printf("Final states:\n");
  printArray(dfa->finalStates, dfa->numberOfFinalStates);
  printf("Starting state: %d\n", dfa->startState);
  printf("Transitions:\n");
  for (int i = 0; i < dfa->numInputs; i++)
  {
    printTransitions(dfa->transitionLists[i], i);
  }
}
void addTransition(DFA *dfa, union_state from, int input, union_state to)
{
  Transition *new = malloc(sizeof(Transition));
  // Transition *head = dfa->transitionLists[input];
  new->from = from;
  new->to = to;
  new->next = dfa->transitionLists[input]; // head;
  dfa->transitionLists[input] = new;
  dfa->numberOfStates++;
}

static bool unionContainsFinal(union_state unionState, int finalState)
{
  return (unionState >> finalState) == 1;
}

static union_state getUnionStateOne(state s)
{
  union_state us = s;
  return 1 << us;
}
// static int *flattenLinkedList(Transition *head)
// {
//   int count = 0;
//   Transition *temp = head;
//   while (temp != NULL)
//   {
//     count++;
//     temp = temp->next;
//   }
//   int *arr = (int *)malloc(sizeof(int) * count);
//   temp = head;
//   for (int i = 0; i < count; i++)
//   {
//     arr[i] = temp->to;
//     temp = temp->next;
//   }
//   return arr;
// }
// static void reverse(int *arr, int size)
// {
//   int temp;
//   for (int i = 0; i < size / 2; i++)
//   {
//     temp = arr[i];
//     arr[i] = arr[size - i - 1];
//     arr[size - i - 1] = temp;
//   }
// }
static void copyListToSet(Transition *head, Set *set)
{
  Transition *temp = head;
  int n = 0;
  while (temp != NULL)
  {
    n++;
    // setInsertOne(set, temp->to);
    setInsertOne(set, temp->from);
    temp = temp->next;
  }
  // reverse(set->data, n /* set->length */);
}
// static int getUnionState(state nextStates[], int num)
// {
//   int state = 0;
//   for (int i = 0; i < num; i++)
//   {
//     state += (1 << nextStates[i]);
//   }
//   return state;
// }

/// @brief Returns the union state after transitions
/// from a union state.
/// @param n Grid size
/// @param currentSet Current union state set
/// @param input Input, '0' or '1'
union_state setTransition(int n, union_state currentSet, char input)
{
  state nextStates[2];
  // get exponents of 2 from currentSet
  union_state dfaUnion = 0;
  if (currentSet == 0)
  {
    fprintf(stderr, "Invalid state set: 0\n");
    exit(1);
  }
  int actualState = 0;
  // int saved = currentSet;
  while (currentSet > 0)
  {
    int rem = currentSet % 2;
    currentSet /= 2;
    if (rem == 1)
    {
      int num = transitions(n, actualState, input, nextStates);
      for (int i = 0; i < num; i++)
      {
        dfaUnion |= ((union_state)1 << (union_state)nextStates[i]);
      }
    }
    actualState++;
  }
  return dfaUnion;
}
static DFA *createDFA(int n)
{
  DFA *dfa = (DFA *)malloc(sizeof(DFA));
  // int maxNumOfStates = (1 << n);
  dfa->numberOfStates = 0;
  dfa->numberOfFinalStates = 0;
  dfa->startState = getUnionStateOne(0);
  dfa->numInputs = 2;
  dfa->transitionLists = (Transition **)malloc(sizeof(Transition *) * 2);
  dfa->transitionLists[0] = NULL;
  dfa->transitionLists[1] = NULL;
  return dfa;
}

static DFA *justConvertNFAtoDFA(int n)
{
  DFA *dfa = createDFA(n);
  Queue *q = createQueue();
  Set *s = createSet();
  // state nextStates[2] = {0, 2};
  // start with starting state 0
  enqueue(q, getUnionStateOne(0));
  setInsertOne(s, getUnionStateOne(0));
  while (queueNotEmpty(q))
  {
    union_state state = dequeue(q);
    for (int input = 0; input <= 1; input++)
    {
      union_state dfaUnionState = setTransition(n, state, input + '0');
      addTransition(dfa, state, input, dfaUnionState);
      if (!setHasOne(s, dfaUnionState))
      {
        setInsertOne(s, dfaUnionState);
        enqueue(q, dfaUnionState);
      }
    }
  }
  // dfa->states = setFlatten(s);
  dfa->numberOfStates = s->length;
  // printTransitions(dfa->transitionLists[0], 0);
  // printTransitions(dfa->transitionLists[1], 1);
  // mark final states
  Set *finals = createSetFrom(s);
  int finalState = (n + 1) * (n + 1) - 1;
  for (int i = 0; i < s->length; i++)
  {
    if (unionContainsFinal(s->data[i]->e1, finalState))
    {
      dfa->numberOfFinalStates++;
    }
    else
    {
      setRemoveP(finals, s->data[i]);
    }
  }
  dfa->finalStates = setFlatten(finals);
  // printTransitions(dfa->transitionLists[0], 0);
  // printTransitions(dfa->transitionLists[1], 1);
  Set *set = createSet();
  copyListToSet(dfa->transitionLists[0], set);
  copyListToSet(dfa->transitionLists[1], set);
  dfa->states = setFlatten(set);
  queueDestroy(q);
  setDestroy(s);
  setDestroy(finals);
  setDestroy(set);
  return dfa;
}
static int getTransition(const DFA *dfa, int from, int input)
{
  Transition *temp = dfa->transitionLists[input];
  while (temp != NULL)
  {
    if (temp->from == dfa->states[from])
    {
      // printf("Got transition from %d to %d\n", dfa->states[from], temp->to);
      return getIndexOf(temp->to, dfa->states, dfa->numberOfStates); // temp->to;
    }
    temp = temp->next;
  }
  fprintf(stderr, "No transition found from %d on '%d'", from, input);
  return -1;
}
DFA *minimizeDFA(const DFA *dfa)
{
  int numberOfStates = dfa->numberOfStates;
  // reverse(dfa->states, numberOfStates);
  // printf("State %d index is %d\n", dfa->states[0], getIndexOf(dfa->states[0], dfa->states, numberOfStates));
  // printf("Starting state: %d\n", dfa->startState);
  // set of indices of states
  Set *s = createSet();
  // add finals
  for (int i = 0; i < dfa->numberOfFinalStates; i++)
  {
    int finalStateAsIndex = getIndexOf(dfa->finalStates[i], dfa->states, numberOfStates);
    for (int j = 0; j < dfa->numberOfStates; j++)
    {
      if (finalStateAsIndex != j)
      {
        setInsert(s, finalStateAsIndex, j);
      }
    }
  }
  // finals done, go through all now
  // printf("Initial set\n");
  // setPrintAll(s);
  bool changed = true;
  while (changed)
  {
    changed = false;
    for (int i = 0; i < dfa->numberOfStates; i++)
    {
      for (int j = 0; j < dfa->numberOfStates; j++)
      {
        if (i != j)
        {
          if (!setHas(s, i, j))
          {
            // check if they are different
            for (int input = 0; input < dfa->numInputs; input++)
            {
              int from1 = getTransition(dfa, i, input);
              int from2 = getTransition(dfa, j, input);
              if (setHas(s, from1, from2))
              {
                setInsert(s, i, j);
                changed = true;
                break;
              }
            }
          }
        }
      }
    }
  }
  // we now have the set of distinguishable states
  // printf("Distinguishable states:\n");
  // setPrintAll(s);
  // now we need to create a new DFA
  // create dsu
  init_dsu(dfa->numberOfStates);
  int finalNumOfStates = 0;
  for (int i = 0; i < numberOfStates; i++)
  {
    for (int j = 0; j < numberOfStates; j++)
    {
      if (i != j && !setHas(s, i, j))
      {
        union_sets(i, j);
        // printf("Union: %d %d\n", i, j);
        finalNumOfStates++;
      }
      // else if (i != j)
      // printf("Min state pair: %d,%d\n", i, j);
    }
  }
  // now we have the new states
  // int *newStates = malloc(finalNumOfStates * sizeof(int));
  Set *newStates = createSet();
  for (int i = 0; i < numberOfStates; i++)
  {
    setInsertOne(newStates, find_set(i));
  }
  // add final states
  Set *finals = createSet();
  for (int i = 0; i < dfa->numberOfFinalStates; i++)
  {
    // printf("Final state: %d, index: %d\n", dfa->finalStates[i], getIndexOf(dfa->finalStates[i], dfa->states, dfa->numberOfStates));
    setInsertOne(finals, find_set(getIndexOf(dfa->finalStates[i], dfa->states, dfa->numberOfStates)));
  }
  // printf("Minimized dfa:\n");
  // setPrintAll(newStates);
  // setPrintAll(finals);
  // create new DFA
  DFA *newDFA = malloc(sizeof(DFA));
  newDFA->transitionLists = malloc(dfa->numInputs * sizeof(Transition *));
  newDFA->transitionLists[0] = NULL;
  newDFA->transitionLists[1] = NULL;
  // build the transition lists
  //  Transition **transitionLists = (Transition **)malloc(sizeof(Transition **) * dfa->numInputs);
  // printf("Adding to new dfa\n");
  for (int input = 0; input <= 1; input++)
  {
    Transition *temp = dfa->transitionLists[input];
    while (temp != NULL)
    {
      union_state from = (temp->from);
      union_state to = (temp->to);
      int fromIndex = getIndexOf(from, dfa->states, dfa->numberOfStates);
      int toIndex = getIndexOf(to, dfa->states, dfa->numberOfStates);
      fromIndex = find_set(fromIndex);
      toIndex = find_set(toIndex);
      // printf("Added transition from %d to %d on '%d'\n", from, to, input);
      addTransition(newDFA, fromIndex, input, toIndex);
      temp = temp->next;
    }
    // printTransitions(newDFA->transitionLists[input], input);
  }
  newDFA->numberOfStates = newStates->length; // finalNumOfStates;
  newDFA->numberOfFinalStates = finals->length;
  newDFA->finalStates = setFlatten(finals);
  newDFA->numInputs = dfa->numInputs;
  newDFA->startState = find_set(dfa->startState);
  newDFA->states = setFlatten(newStates);
  // reverse(newDFA->states, numberOfStates);

  return newDFA;
}

DFA *convertToDFA(int n)
{
  if (n > 10)
  {
    fprintf(stderr, "[Task 2 bounds error]\nCannot convert NFA(n=%d) to DFA: (n+1)^2 (i.e. %d) cannot be represented by 128-bit unsigned integer.\n", n, (n + 1) * (n + 1));
    return NULL;
  }
  DFA *source_dfa = justConvertNFAtoDFA(n);
  // Reverse the states
  // because the start state is actually the last in
  // the `states` array originally.
  union_state *reversedStates = malloc(source_dfa->numberOfStates * sizeof(union_state));
  for (int i = source_dfa->numberOfStates - 1; i >= 0; i--)
  {
    reversedStates[source_dfa->numberOfStates - i - 1] = source_dfa->states[i];
  }
  source_dfa->states = reversedStates;
  // printf("DFA from NFA is:\n");
  // debugPrintDFA(source_dfa);
  DFA *dfa = minimizeDFA(source_dfa);
  // printf("Minimized DFA is:\n");
  // debugPrintDFA(dfa);
  return dfa;
}

int getIndexOf(union_state state, union_state *states, int numberOfStates)
{
  for (int i = 0; i < numberOfStates; i++)
  {
    if (states[i] == state)
    {
      return i;
    }
  }
  return -1;
}

int **convertToArray(DFA *dfa)
{
  printf("States: %d\n", dfa->numberOfStates);
  int numberOfStates = dfa->numberOfStates;
  // int arr[numberOfStates * 2 + 1][numberOfStates];
  // int(*arr)[numberOfStates * 2 + 1] = (int **)malloc(arraySize);
  size_t arraySize = sizeof(int) * ((numberOfStates * 2 + 1) * numberOfStates);
  int **arr = malloc(sizeof(int) * (numberOfStates * 2 + 1));
  for (int i = 0; i < numberOfStates * 2 + 1; i++)
  {
    arr[i] = malloc(sizeof(int) * numberOfStates);
  }
  // printf("Size of array: %d\n", arraySize / sizeof(arr[0]));
  union_state *states = dfa->states;
  // memset(arr, 0, sizeof(int) * (numberOfStates * 2 + 1) * numberOfStates);
  memset(arr, 0, arraySize);
  // return arr;
  for (int i = 0; i < numberOfStates; i++)
  {
    printf("%d\n", dfa->states[i]);
  }
  for (int i = 0; i < dfa->numberOfFinalStates; i++)
  {
    int index = getIndexOf(dfa->finalStates[i], states, numberOfStates);
    printf("Index of %d = %d\n", dfa->finalStates[i], index);
    if (index > -1)
    {
      printf("Setting [0][%d] = 1\n", index);
      arr[0][index] = 1;
    }
  }
  // return arr;
  for (int input = 0; input <= 1; input++)
  {
    Transition *head = dfa->transitionLists[input];
    while (head != NULL)
    {
      union_state from = head->from;
      union_state to = head->to;
      int fromIndex = getIndexOf(from, states, numberOfStates);
      int toIndex = getIndexOf(to, states, numberOfStates);
      // printf("Writing transition (%d, '%d')->%d\n", from, input, to);
      printf("Writing transition [%d, %d]=1\n", fromIndex + input * (numberOfStates) + 1, toIndex);
      arr[fromIndex + input * (numberOfStates) + 1][toIndex] = 1;
      head = head->next;
    }
  }
  return arr;
}

static int isFinalState(DFA *dfa, union_state state)
{
  for (int i = 0; i < dfa->numberOfFinalStates; i++)
  {
    if (dfa->finalStates[i] == state)
    {
      return 1;
    }
  }
  return 0;
}
void printDFADirect(DFA *dfa, int n, FILE *fptr)
{
  // return;
  int numberOfStates = dfa->numberOfStates;
  // print finals first
  // reverse(dfa->states, numberOfStates);
  for (int i = 0; i < numberOfStates; i++)
  {
    union_state state = dfa->states[i];
    fprintf(fptr, "%d ", isFinalState(dfa, state));
  }
  fprintf(fptr, "\n");
  // now input '0' and '1'
  for (int input = 0; input <= 1; input++)
  {
    for (int i = 0; i < numberOfStates; i++)
    {
      union_state state = dfa->states[i];
      Transition *head = dfa->transitionLists[input];
      union_state final = 0;
      while (head != NULL)
      {
        if (head->from == state)
        {
          final = head->to;
          break;
        }
        head = head->next;
      }
      // printf("Found (");
      // print_uint128_no_line(state);
      // printf(", '%d')->", input);
      // print_uint128(final);

      int finalIndex = getIndexOf(final, dfa->states, numberOfStates);
      // printf("FoundIndex (%d, '%d')->%d\n", getIndexOf(state, dfa->states, numberOfStates), input, finalIndex);
      for (int i = 0; i < numberOfStates; i++)
      {
        if (i == finalIndex)
        {
          fprintf(fptr, "%d ", 1);
        }
        else
        {
          fprintf(fptr, "%d ", 0);
        }
      }
      fprintf(fptr, "\n");
    }
  }
}

void printDFA(DFA *dfa, int n, FILE *fptr)
{
  printDFADirect(dfa, n, fptr);
  return;
  int numberOfStates = dfa->numberOfStates;
  // int arr[numberOfStates * 2 + 1][numberOfStates];

  // int(*arr)[numberOfStates] = convertToArray(dfa);
  int **arr = convertToArray(dfa);
  // return;

  for (int column = 0; column < numberOfStates; column++)
  {
    fprintf(fptr, "%d ", arr[2][column]);
  }
  fprintf(fptr, "\n");
  int rows = 2 * numberOfStates + 1;
  for (int i = 0; i < rows; i++)
  {
    for (int column = 0; column < numberOfStates; column++)
    {
      fprintf(fptr, "%d ", arr[i][column]);
    }
    fprintf(fptr, "\n");
  }
}