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


_Bool l_QisEmpty(q *queue);
log *l_new_node();
void l_enqueue(q *queue, char *element);
char* l_dequeue(q *queue);
char* l_get(q *queue, int index);
q *l_initialize_queue();
void l_print_q(q *command_list);


#endif //LOGGER_H
