#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///function prototypes
char **read_textfile(char *FILENAME);
int check_word(char **dict, char *word);
void print_str(char **str);

#endif //UTILITIES_H
