#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#include "file.h"
#include "nfa.h"
#include "min_dfa.h"
#include "set.h"
#include "to_dfa.h"
void getInput(Input *);
void convertAndPrintDFA(int);
int main()
{
  Input input;
  getInput(&input);
  DFA *dfa = convertToDFA(2);
  FILE *fptr = fopen("2020A7PS0284H_t2.txt", "w");
  printDFADirect(dfa, 2, fptr);
  int status = run_thread(input.n, input.string);
  return 0;
}

void getInput(Input *input)
{
  if (getInputFromFile("input.txt", input) == -1)
  {
    fprintf(stderr, "Error reading input file\n");
    exit(1);
  }
}

void test()
{
  int i = 0;
  char p[11] = {0};
  while (i < 2)
  {
    printf("%d\n", i);
    if (i >= 0)
    {
      int pid = fork();
      if (pid == 0)
      {
        p[i * 2] = i + 'a';
      }
      else
        p[i * 2] = i + 'c';
      p[i * 2 + 1] = ' ';
    }
    i++;
    p[i * 2 + 2] = '\0';
    printf("p: %s\n", p);
  }
  printf("After fork somewhere\n");
}