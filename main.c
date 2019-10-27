#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "linkedlist_new.h"


q *read_textfile(char *FILENAME);

//create socket?

int main(){

  q * read = initialize_queue();
  read = read_textfile("words-2.txt");
  
  
  
  
  return 0;
}

//read the contents of the text file into a list structure
q *read_textfile(char *FILENAME){

  //open file
  FILE *fp = fopen(FILENAME, "r+");

  //arbitrary buffer size
  int BUFFSIZE = 100;
  
  //used for holding dictionary
  char *word = (char*)malloc(BUFFSIZE*sizeof(char));  
  q *dict = initialize_queue();

  //read file to end
  while (fgets(word, BUFFSIZE, fp)!=NULL){    

    char *temp = (char*)malloc(BUFFSIZE*sizeof(char));
    strcpy(temp, word);
    enqueue(dict, temp);

  }
  
  //return the newly allocated dictionary
  return dict;

}  
			     


