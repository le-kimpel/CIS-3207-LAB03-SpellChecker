#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct logNode{

  struct logNode *next;
  char* element;

}log;


typedef struct l_Queue{
  log *head;
  log *tail;
  int size;
  int counter;
  int capacity;
  pthread_mutex_t mutex;
  pthread_cond_t isEmpty;
  pthread_cond_t isFull;
}lq;


_Bool l_QisEmpty(lq *queue);
log *l_new_node();
void l_enqueue(lq *queue, char *element);
char* l_dequeue(lq *queue);
char* l_get(lq *queue, int index);
lq *l_initialize_queue();
void l_print_q(lq *command_list);


#endif //LOGGER_H
