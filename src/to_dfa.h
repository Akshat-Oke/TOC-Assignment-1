#ifndef toc_to_dfa
#define toc_to_dfa
#include <stdio.h>
#include <stdint.h>
#include "nfa.h"
#include "uint128.h"

typedef struct Transition
{
  struct Transition *next;
  // Will always be 2
  // for '0' and '1'
  // int numInputs;
  // Initial state
  union_state from;
  // Final states for every input
  union_state to;
  /*Example:
  numInputs: 2
  from: 0
  to: 2, 4
  Means d(0, '0') = 2 and
  d(0, '1') = 4
  */
} Transition;

typedef struct
{
  // The number of rows in transitionFunc
  int numberOfStates;
  union_state *states;
  union_state startState;
  int numberOfFinalStates;
  union_state *finalStates;
  // Always 2, for '0', '1'
  int numInputs;
  // All transitions in this DFA
  // are stored as a linked list
  // of Transition objects
  // There are two linked lists, one for each input
  Transition **transitionLists;
} DFA;

/// @brief Converts given NFA to DFA
/// @param n The grid size
DFA *convertToDFA(int n);
void addTransition(DFA *dfa, union_state from, int input, union_state to);
int **convertToArray(DFA *dfa);
void printDFA(DFA *dfa, int n, FILE *fptr);
void printDFADirect(DFA *dfa, int n, FILE *fptr);

#endif