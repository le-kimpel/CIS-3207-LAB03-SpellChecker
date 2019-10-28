CFLAGS = -g

objects = simpleServer.o utilities.o open_listenfd.o linkedlist_new.o 

main : $(objects)
	cc -o main $(objects)

simpleServer.o : utilities.h open_listenfd.h linkedlist_new.h

.PHONY : clean
clean :
	rm main $(objects)
