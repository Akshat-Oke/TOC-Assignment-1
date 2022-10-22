#include "string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE *fptr = NULL;
// Initialize the file pointer
// for printing strings
void initStringFile(FILE *f)
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
    // printf won't work correctly
    // and cause interleaved output
    // because of threads
    new[2 * i] = str[i];
    new[2 * i + 1] = ' ';
    i++;
  }
  new[2 * i] = '\0';
  // create a anew string
  // for the digits to be printed
  // we cannot just print each character as
  // an integer because other threads
  // may be printing at the same time and
  // mess up the output
  char str_new[strlen(new) + 1];
  int index = 0;
  // print new as digits, subtract from '0' = 48
  for (int i = 0; i < strlen(new); i++)
  {
    if (new[i] != ' ')
    {
      index += sprintf(&str_new[index], "%d ", new[i] - '0');
    }
  }
  fprintf(fptr, "%s %s\n", prefix, str_new);
}
void append(char *string, char c)
{
  // printf("Appending %c\n", c);
  int len = strlen(string);
  string[len] = c;
  string[len + 1] = '\0';
}

char *new_string(const char *str)
{
  char *new = malloc(sizeof(char) * 1024);
  strcpy(new, str);
  return new;
}