#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist_new.h"
#define WORKER_NUM 10

//function prototypes
pthread_t * create_threadpool();
void * stuff(void * stuff);
q* job_queue;

int main(){

  //initialize dummy queue
  job_queue = initialize_queue();

  int i = 0;

  //enqueue stuff in the queue
  while(i<10){
    enqueue(job_queue, i);
    i++
      }

  //initiate threads
  create_threadpool();
  
  return 0;
}

//experimental method that creates a pool of threads to 1 function
pthread_t * create_threadpool(){

  pthread_t *THREADPOOL = (pthread_t*)malloc(WORKER_NUM*sizeof(pthread_t));
  int thread_ID[WORKER_NUM];

  for (int i = 0; i < WORKER_NUM; i++){
    thread_ID[i] = i;
    pthread_create(&THREADPOOL[i], NULL, &stuff, &thread_ID[i]);
  }
    
  for (int j = 0; j < WORKER_NUM; j++){
    pthread_join(THREADPOOL[j], NULL);
  }

  return THREADPOOL;
}

//sample function: prints 'hello' WORKER_NUM times
void * stuff(void * stuff){

  int retrieved = dequeue(job_queue);

  printf("item [%d] was retrieved by thread [%d]\n", retrieved, stuff);


}
