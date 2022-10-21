#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "string.h"

#include "nfa.h"

/// @brief Length of input string
int length, n;
bool early_terminate = true;
/// Output file pointer
FILE *fptr;

/// Current snapshot of the NFA
typedef struct NFA
{
  state current;
  // input character index
  int i;
  // path traversed till now
  char *path;
} NFA;
char *source;
/// @brief Finds the next state(s) for the NFA
/// @param n The grid size
/// @param current Current state
/// @param input Input symbol '0' or '1'
/// @param nextStates Array where next states will be stored
/// @return Number of transitions from current state on input (<= 2)
int transitions(int n, state current, char input, state nextStates[])
{
  nextStates[0] = -1;
  nextStates[1] = -1;
  int num = 0;
  // handle vertical transitions
  if (input == '0')
  {
    if ((current + 1) % (n + 1) != 0)
    {
      nextStates[0] = current + 1;
      num++;
    }
    if (current % (n + 1) != 0)
    {
      nextStates[1] = current - 1;
      num++;
    }
  }
  else if (input == '1')
  {
    // handle horizontal transitions
    if (!(current >= n * (n + 1)))
    {
      nextStates[0] = current + (n + 1);
      num++;
    }
    if (current > n)
    {
      nextStates[1] = current - (n + 1);
      num++;
    }
  }
  else
  {
    fprintf(stderr, "Invalid input: %c\n", input);
    exit(1);
  }
  if (num == 1)
  {
    if (nextStates[0] == -1)
    {
      nextStates[0] = nextStates[1];
      nextStates[1] = -1;
    }
  }
  // printf("Transitions: %d:%c->%d, %d(%d)\n", current, input, nextStates[0], nextStates[1], num);
  return num;
}

/// Runs in a thread to transition to the next states.
/// Will spawn new threads if there are multiple transitions.
/// @param arg Current NFA snapshot
void *runMachine(void *arg)
{
  NFA *nfa = (NFA *)arg;
  state current = nfa->current;
  int i = nfa->i;
  append_and_print(nfa->path, current + '0', "Exploring Path:");
  // Check if input string is over
  if (nfa->i >= length)
  {
    if (current == (n + 1) * (n + 1) - 1)
    {
      printf("Accepted.\n");
      print_with_spaces(nfa->path, "Accepted! Followed path:");
      if (early_terminate)
        exit(0); // terminate all threads (program)
      return NULL;
    }
    // Failed as not in final state
    print_with_spaces(nfa->path, "Failed at path:");
    return NULL;
  }
  // Continue with input
  char input = source[nfa->i];
  state nextStates[2];
  int num = transitions(n, current, input, nextStates);
  if (num == 1)
  {
    // if only 1 transition is possible, continue
    // the same thread
    nfa->current = nextStates[0];
    nfa->i++;
    runMachine(nfa);
  }
  else if (num == 2)
  {
    // create new threads for both transitions.
    // this thread will terminate.
    // NFA *nfa1 = (NFA *)malloc(sizeof(NFA));
    NFA *nfa1 = nfa;
    nfa1->current = nextStates[0];
    nfa1->i = i + 1;
    // nfa1->path = nfa->path;
    NFA *nfa2 = (NFA *)malloc(sizeof(NFA));
    nfa2->current = nextStates[1];
    nfa2->i = i + 1;
    // We cannot use nfa1->path = nfa->path
    // because it will be reused by the other thread
    nfa2->path = new_string(nfa->path);
    // Create the threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, runMachine, (void *)nfa1);
    pthread_create(&thread2, NULL, runMachine, (void *)nfa2);
    // Wait for both
    int r1 = pthread_join(thread1, NULL);
    int r2 = pthread_join(thread2, NULL);
    if (r1 || r2)
    {
      printf("Error:unable to join, %d %d", r1, r2);
      exit(-1);
    }
    // release memory
    // free(nfa2->path);
    // free(nfa2);
  }
  return NULL;
}
int run_thread(int gridSize, char *sourceStr)
{
  fptr = fopen("2020A7PS0284_t1.txt", "w");
  initFile(fptr);
  source = new_string(sourceStr);
  n = gridSize;
  length = strlen(source);
  // First thread will have starting state
  pthread_t tid;
  NFA *nfa = (NFA *)malloc(sizeof(NFA));
  nfa->current = 0;
  nfa->i = 0;
  nfa->path = new_string(""); //(char *)malloc(sizeof(char) * (length + 1));
  int result = pthread_create(&tid, NULL, runMachine, (void *)(nfa));
  if (result)
  {
    printf("Error creating thread %d", result);
    exit(1);
  }
  result = pthread_join(tid, NULL);
  if (result)
  {
    printf("Error joining thread %d", result);
    exit(1);
  }
  free(nfa->path);
  free(nfa);
  return 0;
}