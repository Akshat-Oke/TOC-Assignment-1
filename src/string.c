#include "string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE *fptr = NULL;
void initFile(FILE *f)
{
  fptr = f;
}
long unsigned int strlen(const char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    i++;
  }
  return i;
}
void append_and_print(char *str, char c, const char *prefix)
{
  append(str, c);
  // if (fptr != NULL)
  // {
  //   fprintf(fptr, "Path: ");
  //   fflush(fptr);
  // }
  // else
  //   printf("Path: ");
  print_with_spaces(str, prefix);
}
void print_with_spaces(char *str, const char *prefix)
{
  char *new = malloc(strlen(prefix) + strlen(str) * 2 + 1);
  strcpy(new, prefix);
  int i = 0;
  while (str[i] != '\0')
  {
    // fprintf(fptr, "%c ", str[i]);
    new[2 * i] = str[i];
    new[2 * i + 1] = ' ';
    i++;
  }
  new[2 * i] = '\0';
  fprintf(fptr, "%s %s\n", prefix, new);
}
void append(char *string, char c)
{
  // printf("Appending %c\n", c);
  int len = strlen(string);
  string[len] = c;
  string[len + 1] = '\0';
}
// static char *strcpy(char *s1, const char *s2)
// {
//   int i = 0;
//   while (s2[i] != '\0')
//   {
//     s1[i] = s2[i];
//     i++;
//   }
//   s1[i] = '\0';
//   return s1;
// }
char *new_string(const char *str)
{
  char *new = malloc(sizeof(char) * 1024);
  strcpy(new, str);
  return new;
}