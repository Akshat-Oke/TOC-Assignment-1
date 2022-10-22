/*
 * |___________________________________|
 * |   .    .    .     .     .   .   . |
 * |  .   .   Welcome to the  .    .   |
 * |     .   TOC Assignment 1 . .      |
 * |   .    .   Rainforest .   .  .    |
 * | .    .   .    .     .   .         |
 * |. A .lot of things lurk under the  |
 * | . . surface.of this forest.  .    |
 * |.  .Be careful, because things   . |
 * | .  .are not what .you expect. .   |
 * |  .     .   .    .   .      .   .  |
 * |.  . Lest you lose your way,  .   .|
 * | .  follow the helpful signs .  . .|
 * |.  . and you will be fine. . . .  .|
 * |.  . .  .   . .   .  . .    . .  . |
 * | .|| Signs are enclosed in [] ||.  |
 * | .  .  .    .    .   .     .  .  . |
 * |___________________________________|
 *
 */

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
#include "set.h"
#include "to_dfa.h"
void getInput(Input *, int, char *[]);
void convertAndPrintDFA(int);

//[1] Start here!
int main(int argc, char *argv[])
{
  //[2] Input is a struct with `n` and
  // input `string` members
  Input input;
  //[3] Reads the input from cmd args and input file
  getInput(&input, argc, argv);
  //[4] Create two processes for each task:
  // 1. Run the NFA on the input string, and
  // 2. Convert the NFA to DFA and print it
  // The parent process waits for both children to finish
  if (fork())
  {
    // Task 1
    FILE *fptr = fopen("2020A7PS0284_t1.txt", "w");
    //[5] Run the NFA on the input string in a new thread
    run_thread(input.n, input.string, fptr);
    fclose(fptr);
    //[20] This process will terminate when a string is accepted.
    printf("Rejected.\n");
    // run(input.n, input.string);
    wait(NULL);
  }
  else
  {
    DFA *dfa = convertToDFA(input.n);

    FILE *fptr = fopen("2020A7PS0284H_t2.txt", "w");
    printDFADirect(dfa, input.n, fptr);
    fclose(fptr);
  }
  return 0;
}

void getInput(Input *input, int argc, char *argv[])
{
  char *filename = "input.txt";
  if (argc == 3)
  {
    input->n = atoi(argv[1]);
    input->string = argv[2];
  }
  else if (argc < 3)
  {
    if (argc == 2)
      filename = argv[1];
    if (getInputFromFile(filename, input) == -1)
    {
      fprintf(stderr, "Error reading input file\n");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "Too many command line arguments\n");
    printf("Usage: ./bin/main\n");
    printf("Or: ./bin/main [input filename]\n");
    printf("Or: ./bin/main [n] [input_string]\n");
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