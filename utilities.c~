#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

///function prototypes
char **read_textfile(char *FILENAME);
char **sort(char **dict);
int check_word(char **dict, char *word);
void print_str(char **str);

//used for debugging entries
void print_str(char **str){

  int i = 0;
  while(str[i] != NULL){
    printf("%s%s%s\n", "{", str[i], "}");
    i++;
  }
  
}

//read the contents of the text file into a list structure
char **read_textfile(char *FILENAME){

  //open file
  FILE *fp = fopen(FILENAME, "r+");

  //arbitrary buffer size
  int BUFFSIZE = 100;
  
  //used for holding dictionary
  char *word = (char*)malloc(BUFFSIZE*sizeof(char));  
  int lines = 0;

  //count the size needed for dictionary
  while(fgets(word, BUFFSIZE, fp)!=NULL){
    lines++;
  }

  //reset file pointer
  rewind(fp);

  //hold the words in a dictionary of known size
  char **dict = (char**)malloc(lines*sizeof(char*));

  //string index
  int i = 0;
  
  //read file to end and copy each entry into an array of char**
  while (fgets(word, BUFFSIZE, fp)!=NULL){    

    dict[i] = (char*)malloc(BUFFSIZE*sizeof(char));
    strcpy(dict[i], word);
    i++;

  }
  
  //return the newly allocated dictionary
  return dict;

}  

//determines whether a word is in the dictionary
int check_word(char **dict, char *word){
  
  int i = 0;
  while(dict[i]!=NULL){

    if (strcmp(dict[i], word) == 0){
      printf("FOUND: [%s]\n", word);
      return 1;
    }

    i++;
    
  }

  //if not returned, the word does not exist
  puts("MISSPELLED");
  return 0;
}
