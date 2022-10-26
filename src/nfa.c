#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

typedef short int state;
int accepted = 0,
    i = 0,
    length, n;
state current = 0;

/*
  Returns the number of next states
  from current state on current input.
  Stores the next states in provided array.
*/
static int transitions(int n, state current, char input, state nextStates[])
{
  nextStates[0] = -1;
  nextStates[1] = -1;
  printf("Transitions: %c\n", input);
  int num = 0;
  // handle vertical transitions
  if (input == '1')
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
  else if (input == '0')
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
  if (num == 1)
  {
    if (nextStates[0] == -1)
    {
      nextStates[0] = nextStates[1];
      nextStates[1] = -1;
    }
  }
  return num;
}
int run_fork(int gridSize, char *source)
{
  state nextState[2] = {-1, -1};
  // 2, 0, '1'
  // printf("%d\n", transitions(2, 0, '1', nextState));
  // printf("%d %d\n", nextState[0], nextState[1]);
  // // 2, 3, '0'
  // printf("%d\n", transitions(2, 3, '0', nextState));
  // printf("%d %d\n", nextState[0], nextState[1]);
  // // 2, 4, '1'
  // printf("%d\n", transitions(2, 4, '1', nextState));
  // printf("%d %d\n", nextState[0], nextState[1]);
  // // 2, 8, '0'
  // printf("%d\n", transitions(2, 8, '0', nextState));
  // printf("%d %d\n", nextState[0], nextState[1]);
  // return 0;
  ///////////////////////
  FILE *fptr = fopen("output.txt", "w");

  char path[gridSize * 3];
  n = gridSize;
  accepted = 0;
  i = 0;
  length = strlen(source);
  int shmid;
  if ((shmid = shmget(2041, 32, IPC_CREAT | 0666)) == -1)
  {
    perror("shmget");
    exit(1);
  }
  int *shm;
  if ((shm = shmat(shmid, NULL, 0)) == (int *)-1)
  {
    perror("shmat");
    exit(1);
  }
  *shm = 0;
  printf("%d\n", length);
  while (i < length)
  {
    fprintf(fptr, "i = %d in process %d\n", i, getpid());
    char curChar = source[i];
    // check if accepted somewhere
    if ((shm = shmat(shmid, NULL, 0)) == (int *)-1)
    {
      perror("shmat");
      exit(1);
    }
    if (*shm)
    {
      exit(0);
    }
    path[i * 2] = current + '0';
    path[i * 2 + 1] = ' ';
    state nextStates[2] = {-1, -1};
    int numOfTransitions = transitions(n, current, curChar, nextStates);

    if (numOfTransitions == 1)
    {
      current = nextStates[0];
    }
    else
    {
      int pid = fork();
      // fprintf(fptr, "Forked: %d\n", i);
      if (pid == 0)
      {
        // in child
        current = nextStates[0];
      }
      else
      {
        // in parent
        current = nextStates[1];
        // check if accepted somewhere
        if ((shm = shmat(shmid, NULL, 0)) == (int *)-1)
        {
          perror("shmat");
          exit(1);
        }
      }
    }
    path[i * 2 + 2] = '\0';
    fprintf(fptr, "%d] Path: %s\n", getpid(), path);
    i++;
  }
  if (current == (n + 1) * (n + 1) - 1)
  {
    *shm = 1;
    fprintf(fptr, "Accepted %s%d\n", path, current);
    return 1;
  }
  else
  {
    fprintf(fptr, "Rejected %s%d\n", path, current);
    return 0;
  }
}
