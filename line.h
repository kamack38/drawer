#ifndef LINE_H
#define LINE_H
#include <stdio.h>

int fgetline(FILE* file, char** buffer, size_t* size);
char* skip_whitespace(char* buffer);
int has_trailing_words(char* buffer);
#endif  // LINE_H
