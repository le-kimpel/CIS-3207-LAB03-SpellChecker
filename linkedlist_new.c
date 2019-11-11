#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_CAPACITY 2

/* Overhauled version of the queue from the first lab.
   Holds socket connections this time and dequeues from
   the back. It also contains locks and condition variables
   within the queue struct for better and more atomic 
   organization.
*/

typedef struct jobNode{

  struct jobNode *next;
  int element;
  
}connection;

//defines the queue structure and associated condition variables
typedef struct Queue{
  connection *head;
  connection *tail;
  int size;
  int counter;
  int capacity;
  pthread_mutex_t mutex;
  pthread_cond_t isEmpty;
  pthread_cond_t isFull;
}q;

_Bool QisEmpty(q *queue);
connection *new_node();
void enqueue(q *queue, int element);
int dequeue(q *queue);
int get(q *queue, int index);
q *initialize_queue();
void print_q(q *command_list);

//function to print the list for debugging
//...oh shit, I might need a lock around this...
//or i can just delete it :P
void print_q(q *command_list){
  
  for (int i = 0; i < command_list->size; i++){
    int entry = get(command_list, i);
    printf("%s%d%s", "[", entry, "]");
  }
  
  puts("");
}


//yeah, uh oh 
int get(q *queue, int index){

  connection *current = queue->head;
  
  int flag = 0;
  
  while(current!=NULL){
    
    if (flag == index){
      
      return (current->element);
    }
    
    flag++;
    current = current->next;
    
  }
 
}
q *initialize_queue(){

  //allocate memory for the queue structure
  q *queue = (q*)malloc(sizeof(q));

  //define the max capacity and the flag that tracks it
  //(so as to control indeces operated on by our finite amount of threads)
  queue->capacity = MAX_CAPACITY;
  queue->counter = 0;

  //initialize the mutex and condition variables within the queue
  pthread_mutex_init(&queue->mutex, NULL);
  pthread_cond_init(&queue->isEmpty, NULL);
  pthread_cond_init(&queue->isFull, NULL);
  
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  
  puts("Queue initialized");
  return queue;
}

connection *new_node(int element){
  
  //create and allocate new node for queue pointer
  connection *temp = (connection*)malloc(sizeof(connection));
  temp->element = element;
  temp->next = NULL;
  return temp;
  
}

void enqueue(q *queue, int element){
  
  //creates a new node for the queue
  connection *temp = new_node(element);
  
  puts("locking queue...");
  
  //lock the queue to avoid race conditions from other worker threads
  if (pthread_mutex_lock(&queue->mutex)!=0){
    puts("could not lock the queue");
  }
  
  //hitting a producer-consumer problem condition:
  //prints error if we try to wait on an empty queue
  while(queue->counter == queue->capacity){
    if (pthread_cond_wait(&queue->isEmpty, &queue->mutex) != 0){
      puts("queue is empty, what am I supposed to do with that?");
    }
  }
  
  //if we hit max capacity on queue
  if (queue->size == MAX_CAPACITY){
    puts("MAX CAPACITY error!");
  }
  
  //if we are starting at size 0 or queue is empty
  if (queue->tail == NULL || queue->size == 0){
    queue->head = temp;
    queue->tail = temp;
  }else{
    
    queue->tail->next = temp;
    queue->tail = temp;
    
  }
  
  //increase overall size of queue
  queue->size++;
  
  //increment counter flag to track for max capacity changes
  queue->counter++;
  
  //if we can't signal a full slot, error
  if (pthread_cond_signal(&queue->isFull) != 0){
    puts("Could not signal for a full slot!");
  }
  
  //if we could not unlock for whatever reason, error
  if (pthread_mutex_unlock(&queue->mutex) != 0){
    puts("Could not unlock mutex");
  }
  
}                                            

//dequeues and locks the structure
int dequeue(q *queue){
  
  puts("Locking dequeue...");
  
  if (pthread_mutex_lock(&queue->mutex) != 0){
    puts("Error locking queue");
  }
  
  puts("Queue locked");
  
  printf("q counter = [%d]\n", queue->counter);
  printf("q size = [%d]\n", queue->size);
  
  while(queue->counter == 0){
    if (pthread_cond_wait(&queue->isFull, &queue->mutex)){
      puts("ERROR: unable to wait on a full slot");
    }
  }
  
  printf("q counter = [%d]\n", queue->counter);
  printf("q size = [%d]\n", queue->size);
  
  puts("waiting...");
  
  //queue can remove an item from the back
  
  connection *temp = queue->head;
  queue->head = queue->head->next;
  queue->size--;
  queue->counter--;
  
  puts("Signaling...");
  
  //attempt to signal producers
  if (pthread_cond_signal(&queue->isEmpty)!=0){
      puts("Cannot signal an empty slot!");
    }

    puts("Attempting to unlock queue...");
    
    //unlock the thread
    if (pthread_mutex_unlock(&queue->mutex)!=0){
      puts("Cannot unlock mutex!");
    }

    puts("Queue unlocked");
   
    return temp->element;
  }

  _Bool QisEmpty(q *queue){
    if (queue->size == 0){
      return 1;
    }else{
      return 0;
    }
}
