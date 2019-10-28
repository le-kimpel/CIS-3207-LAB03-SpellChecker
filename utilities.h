#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

///function prototypes
char **read_textfile(char *FILENAME);
char **sort(char **dict);
int check_word(char **dict, char *word);
void print_str(char **str);

#endif //UTILITIES_H
