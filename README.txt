
-----------------------------------------------------
	CIS 3207 LAB 03: NETWORKED SPELLCHECKER

	NAME: Lauren Kimpel
	TUid: 915354707
-----------------------------------------------------

OVERVIEW

For this assignment, we have been assigned, using skeleton socket programming
code and condition variables, an approach to the Producer-Consumer Problem.

The spellchecker works like so:

(1) First, the server is run and initialized.
(2) A client program (in the default case, netcat) will attempt to connect
to the server's designated port number.
(3) If the connection request is accepted, a user will be prompted to
input words. The program will then say whether the word belongs in a dictionary
held by the program. The dictionary can be either user-specified upon startup
or a default dictionary hard-coded into the program.

-----------------------------------------------------

BUILD NOTES:

For the Extra Credit, the folder "socket 2.0", submitted separately from the original
assignment, contains a multithreaded client, located in "client.c". This amateur client
program takes the place of netcat. It attempts to connect to the server program. 
It does this by creating a pool of NUM_THREADS, specified by the programmer.
It then enters a while-true loop and faciliates the connection from there, using the conn_server
method. Depending on programmer specifications, connections can be rejected or suspended.

The monitor itself was built using the FIFO LinkedList queue from LAB01 with some modifications:

    (1) Built-in condition variables
    (2) Built-in MAX_CAPACITY
    (3) Built-in set of locks
    (4) Built-in tracker counter that follows MAX_CAPACITY and overall queue size

-----------------------------------------------------

HOW TO RUN THIS PROGRAM:

This program depends on a makefile. To compile:

     (1) Enter the 'make' command.
     (2a) To run the program without custom values, type ./main
     (2b) To run the program without a custom dictionary, type ./main [desired port number]
     (2c) To run tye program with entirely custom values, type ./main [pnumber] [dictname]
     (3) To quit, type 'done'. The server should NOT give an error and will wait for another listener.

NOTE: If you should run the extra credit, note that its CLIENT design is "sloppier" than netcat's.
This means that it runs all its threads in one terminal window and will have unique quirks
when exiting due to this design. However, its performance output is correct.
When demoing, I can help determine which values are correct and which are not.

-----------------------------------------------------

TESTING DETAILS:

CONDITION VARIABLES
In the synchronized monitor, print statements have been included
to better follow the locking/unlocking/signaling process between
each built-in condition variable. It looks ugly, but since we are not
using a debugger like GDB, then it has proven to be relateively useful
when analyzing the potential for race conditions.

CLIENT-SERVER CONNECTIONS
Print statements were also used to ensure BIDIRECTIONAL COMMUNICATION
between client and server over a single connection.

TESTING OF THREAD CREATION
The number of new threads is customizable (programmer-only). A starting
thread number of 2 was used to modularly test thread creation and keep
thread-related matters uncomplicated. A tracker print statement follows
each thread.

ENQUEUE-DEQUEUE ERROR DETECTION
Print statements have been included to signify when the queue
is being locked, unlocked, and when threads attempt to remove from it
with/without success. Thus, no connection is retrieved from an empty queue
and no extra connections are added past maximum queue capacity.

SPELLCHECKING
Spellchecking was tested using a variety of inadequate inputs.
Thus the buffer clears after every attempt.
When the user types 'done', the program closes. 


