#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

int getInputFromFile(const char *filename, Input *input)
{
  FILE *fptr;
  if ((fptr = fopen(filename, "r")) == NULL)
  {
    return -1;
  }
  // read n
  fscanf(fptr, "%d\n", &input->n);
  // read input string
  input->string = (char *)malloc(1024);
  fscanf(fptr, "%s", input->string);
  input->length = strlen(input->string);
  fclose(fptr);
  return 1;
}