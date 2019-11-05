#include "simpleServer.h"
#include "utilities.h"
#include "linkedlist_new.h"
#include "logger.h"
#include <pthread.h>

//An extremely simple server that connects to a given port.
//Once the server is connected to the port, it will listen on that port
//for a user connection.
//A user will connect through telnet, the user also needs to know the port number.
//If the user connects successfully, a socket descriptor will be created.
//The socket descriptor works exactly like a file descriptor, allowing us to read/write on it.
//The server will then use the socket descriptor to communicate with the user, sending and 
//receiving messages.


//function prototypes
pthread_t *create_threadpool();
void *take_socket(void *threadID);
void client_handler(int clientSocket);
void *log_entry(void *arg);

//global values for default dictionary
char **DEFAULT_DICT;

//global values for custom dictionary included in cmd line
char **CUSTOM_DICT;

//default port number
int DEFUALT_PORT = 1111;

//default worker num
const int WORKER_NUM = 2;

//global variables for servicing client using recv
int bytesReturned;
char recvBuffer[BUF_LEN];

//global socket queue
q *job_queue;

//global log queue
lq *log_queue;

//logger thread
pthread_t log_thread;

//main thread
int main(int argc, char** argv)
{ 
  puts("START");
   
  puts("Initializing queues...");

  //generate the connection queue
  job_queue = initialize_queue();

  //generate the logger queue
  log_queue = l_initialize_queue();

  puts("Generating threadpool...");

  //generate threadpool
  create_threadpool();

  puts("Threadpool generated...");
  
 //default dictionary preset
  DEFAULT_DICT = read_textfile("words-2.txt");

  //custom dictionary is read if there is an argument for it
  if (argv[2] != NULL){
    CUSTOM_DICT = read_textfile(argv[2]);
    DEFAULT_DICT = CUSTOM_DICT;
    printf("Using custom dictionary: [%s]\n", argv[2]);
  }

  //if there is no port number specified
  if(argc == 1){
    printf("No port number entered.\n");
    return -1;
  }
  
  //sockaddr_in holds information about the user connection. 
  //We don't need it, but it needs to be passed into accept().
  struct sockaddr_in client;
  int clientLen = sizeof(client);
  int connectionPort = atoi(argv[1]);
  int connectionSocket, clientSocket;
  recvBuffer[0] = '\0';
  
  connectionPort = atoi(argv[1]);
  
  //We can't use ports below 1024 and ports above 65535 don't exist.
  if(connectionPort < 1024 || connectionPort > 65535){
    printf("Port number is either too low(below 1024), or too high(above 65535).\n");
    return -1;
  }
  
  //Does all the hard work for us.
  connectionSocket = open_listenfd(connectionPort);
  printf("%d\n", connectionSocket);
  if(connectionSocket == -1){
    printf("Could not connect to %s, maybe try another port number?\n", argv[1]);
    return -1;
  }

  while(1){

  printf("I am awaiting your input at port [%d] and socket [%d]\n", connectionPort, connectionSocket);
  //accept() waits until a user connects to the server, writing information about that server
  //into the sockaddr_in client.
  //If the connection is successful, we obtain A SECOND socket descriptor. 
  //There are two socket descriptors being used now:
  //One by the server to listen for incoming connections.
  //The second that was just created that will be used to communicate with 
  //the connected user.

  if((clientSocket = accept(connectionSocket, (struct sockaddr*)&client, &clientLen)) == -1){
    printf("Error connecting to client.\n");
    return -1;
  }

  //enqueue the incoming file descriptor into the job queue
  enqueue(job_queue, clientSocket);
}
  return 0;
}

void client_handler(int clientSocket){

  //create the file pointer for the log file
  FILE *fp = fopen("log.txt", "w");
  
  printf("Connection success!\n");
  char* clientMessage = "Hello! I hope you can see this.\n";
  char* msgRequest = "Send me some text and I'll respond with something interesting!\nSend the escape key to close the connection.\n";
  char* msgResponse = "I actually don't have anything interesting to say...but I know you sent ";
  char* msgPrompt = ">>>";
  char* msgError = "I didn't get your message. ):\n";
  char* msgClose = "Goodbye!\n";
  
  //initialize the logger thread
  pthread_create(&log_thread, NULL, &log_entry, fp);
  
  //send()...sends a message.
  //We specify the socket we want to send, the message and it's length, the 
  //last parameter are flags.
  send(clientSocket, clientMessage, strlen(clientMessage), 0);
  send(clientSocket, msgRequest, strlen(msgRequest), 0);
  
  //Begin sending and receiving messages.
  while(1){
    
    send(clientSocket, msgPrompt, strlen(msgPrompt), 0);
    //recv() will store the message from the user in the buffer, returning
    //how many bytes we received.
    bytesReturned = recv(clientSocket, recvBuffer, BUF_LEN, 0);
    
    //Check if we got a message, send a message back or quit if the
    //user specified it.
    if(bytesReturned == -1){
      send(clientSocket, msgError, strlen(msgError), 0);
    }else if (bytesReturned == 0){
      close(clientSocket);
      printf("Disconnected client\n");
      fclose(fp);
      break;
    //"done" is the escape key.
    }else{
      send(clientSocket, msgResponse, strlen(msgResponse), 0);
      send(clientSocket, recvBuffer, bytesReturned, 0);
    }

    //clears buffer, checks for terminating condition
    recvBuffer[bytesReturned] = '\0';
    if(strcmp(recvBuffer, "done\n") == 0){
      send(clientSocket, msgClose, strlen(msgClose), 0);
      close(clientSocket);
      fclose(fp);
      break;
    }
   
    //check if dictionary word is legitimate and log it
    (check_word(DEFAULT_DICT, recvBuffer));

    l_enqueue(log_queue, recvBuffer);
    
  }
  
}

//method that logs each string entry to log.txt
void *log_entry(void *arg){

  while(1){
    char* entry = l_dequeue(log_queue);
    fprintf(arg,"%s\n", entry);
  }
}
  
//method that creates a pool of threads to 1 function
pthread_t *create_threadpool(){

  //allocates memory for an array of WORKER_NUM threads
  pthread_t *THREADPOOL = (pthread_t*)malloc(WORKER_NUM*sizeof(pthread_t));
  int thread_ID[WORKER_NUM];
  
  //fill the array for tracking purposes
  for (int i = 0; i < WORKER_NUM; i++){
    thread_ID[i] = i;
    pthread_create(&THREADPOOL[i], NULL, &take_socket, &i);
  }
  
  return THREADPOOL;
}

//function for threads to retrieve socket connections from queue
void *take_socket(void *threadID){

  //thread ID 
  int tid = (*((int*)threadID));

  while(1){
    puts("Attempting to take socket...");
    
    //if our queue is nonempty, retrieve the connection from it
    int retrieved = dequeue(job_queue);
    
    printf("socket [%d] was retrieved from queue by thread [%d]\n", retrieved,
	   tid);
    //handle the retireved connection for the client
    client_handler(retrieved); 
  }
  
}
 
