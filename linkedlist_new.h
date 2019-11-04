#ifndef LINKEDLISTNEW_H
#define LINKEDLISTNEW_H
#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct jobNode{

  struct jobNode *next;
  int element;

}connection;


typedef struct Queue{
  connection *head;
  connection *tail;
  int size;
}q;


_Bool QisEmpty(q *queue);
connection *new_node();
void enqueue(q *queue, int element);
int dequeue(q *queue);
int get(q *queue, int index);
q *initialize_queue();
void print_q(q *command_list);


#endif //LINKEDLISTNEW_H
