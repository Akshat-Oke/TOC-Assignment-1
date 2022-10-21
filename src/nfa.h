#ifndef toc_nfa
#define toc_nfa

// The state of an FA
// 0, 1, 2... n-1
// Represents q0, q1, and so on.
typedef short int state;
// Symbol in the language alphabet,
// actually a char but represented
// by a short int to save the trouble of casting
typedef short int alphabet;

int run(int n, char *source);
int run_thread(int n, char *source);
int transitions(int n, state current, char input, state nextStates[]);
// void convertToDfaAndPrint(int n);
#endif