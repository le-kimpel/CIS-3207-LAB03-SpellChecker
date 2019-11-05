#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_CAPACITY 15

/*Logger thread that holds strings*/

typedef struct logNode{

  struct logNode *next;
  char *element;
  
}log;

//defines the queue structure and associated condition variables
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

//function to print the list for debugging
void l_print_q(lq *command_list){

  int i = 0;
  while(l_get(command_list, i) != NULL){
    char* entry = l_get(command_list, i);
    printf("%s%s%s", "[", entry, "]");
    i++;
  }
  
  puts("");
}

char *l_get(lq *queue, int index){

  log *current = queue->head;
  
  int flag = 0;
  
  while(current!=NULL){
    
    if (flag == index){
      
      return (current->element);
    }
    
    flag++;
    current = current->next;
    
  }

  return current->element;
}
lq *l_initialize_queue(){

  //allocate memory for the queue structure
  lq *queue = (lq*)malloc(sizeof(lq));

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

log *l_new_node(char *element){

  //create and allocate new node for queue pointer
  log *temp = (log*)malloc(sizeof(log));
  temp->element = element;
  temp->next = NULL;
  return temp;
  
}

void l_enqueue(lq *queue, char* element){

  //creates a new node for the queue
  log *temp = l_new_node(element);

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
char *l_dequeue(lq *queue){

  puts("Locking dequeue...");
  
  if (pthread_mutex_lock(&queue->mutex) != 0){
    puts("Error locking queue");
  }

  puts("Queue locked");
  
  while(queue->counter == 0){
    if (pthread_cond_wait(&queue->isFull, &queue->mutex)){
      puts("ERROR: unable to wait on a full slot");
  }

  }

  puts("waiting...");
 
    //queue can remove an item from the back

    log *temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;

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

  _Bool l_QisEmpty(lq *queue){
    if (queue->size == 0){
      return 1;
    }else{
      return 0;
    }
}
