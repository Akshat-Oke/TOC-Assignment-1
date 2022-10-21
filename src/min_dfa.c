// #include <stdio.h>
// #include <stdlib.h>
// #include "min_dfa.h"
// #include "nfa.h"
// #include "set.h"

// FA *createNFA(int n)
// {
//   FA *nfa = malloc(sizeof(FA));
//   nfa->numTransitions = (n + 1) * (n + 1);
//   nfa->numInputs = 2; //'0' and '1'
//   nfa->transitions = malloc(sizeof(Transition *) * nfa->numTransitions * 2);
//   state nextStates[2];
//   for (int i = 0; i < nfa->numTransitions; i++)
//   {
//     for (int input = 0; input <= 2; input++)
//     {
//       Transition *t = malloc(sizeof(Transition));
//       t->from = i;
//       int num = transitions(n, i, input + '0', nextStates);
//       t->coarity = num;
//       t->to = malloc(sizeof(state) * num);
//       for (int j = 0; j < num; j++)
//       {
//         t->to[j] = nextStates[j];
//       }
//       nfa->transitions[i][input] = t;
//     }
//   }
//   // set final state
//   int finalNum = 1;
//   nfa->finalStates = malloc(sizeof(state));
//   nfa->finalStates[0] = nfa->numTransitions - 1;
// }
// void freeFA(FA *nfa)
// {
//   for (int i = 0; i < nfa->numTransitions; i++)
//   {
//     for (int input = 0; input <= 1; input++)
//     {
//       free(nfa->transitions[i][input]->to);
//     }
//   }
//   free(nfa->finalStates);
//   free(nfa->transitions);
//   free(nfa);
// }
// void printDFA(FILE *fptr, FA *dfa)
// {
//   int n = dfa->numTransitions;
//   int output[n * 2 + 1][n]; // = {0};
//   for (int i = 0; i < dfa->finalNum; i++)
//   {
//     output[0][dfa->finalStates[i]] = 1;
//   }
//   for (int i = 0; i < n; i++)
//   {
//     if (output[0][i] != 1)
//       output[0][i] = 0;
//   }
//   for (int input = 0; input <= 1; input++)
//   {
//     for (int i = 0; i < n; i++)
//     {
//       output[1 + input * n + i][i] = dfa->transitions[input][i]->to[0];
//     }
//   }

//   for (int i = 0; i < n * 2 + 1; i++)
//   {
//     for (int j = 0; j < n; j++)
//     {
//       fprintf(fptr, "%d ", output[i][j]);
//     }
//     fprintf(fptr, "\n");
//   }
// }

// FA *convertToDFA(FA *nfa)
// {
//   return NULL;
// }
// void getDFA(int n)
// {
//   int numStates = (n + 1) * (n + 1);
//   int output[numStates * 2 + 1][numStates];
//   Set *set = createSet();
// }