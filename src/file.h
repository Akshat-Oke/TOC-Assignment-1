#ifndef TOC_FILE
#define TOC_FILE

typedef struct
{
  int n;
  int length;
  char *string;
} Input;

int getInputFromFile(const char *filename, Input *input);
#endif