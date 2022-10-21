#ifndef toc_string
#define toc_string
#include <stdio.h>
// create string
// char *new_string(int size);
void initFile(FILE *f);
char *new_string(const char *string);
void append(char *string, char ch);
void append_and_print(char *string, char ch, const char *prefix);
void print_with_spaces(char *string, const char *);
long unsigned int strlen(const char *string);
#endif