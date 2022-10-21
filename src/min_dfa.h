#ifndef toc_min_dfa
#define toc_min_dfa
#include <stdio.h>
#include "nfa.h"

// typedef struct
// {
//   state from;
//   // Number of transitions
//   int coarity;
//   state *to;
// } Transition;

// typedef struct FA
// {
//   // Number of input symbols,
//   // denoted by nunmbers 0, 1... numInputs-1
//   int numInputs;

//   // Number of transitions
//   // Equivalent to the grid size
//   int numTransitions;
//   // Transition*[numTransitions][numInputs]
//   Transition ***transitions;
//   // Number of final states
//   int finalNum;
//   state *finalStates;
// } FA;

// FA *createNFA(int n);
// void freeFA(FA *nfa);
// void printDFA(FILE *fptr, FA *dfa);
// FA *convertToDFA(FA *nfa);

#endif