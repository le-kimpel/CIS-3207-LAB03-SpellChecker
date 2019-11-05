CFLAGS = -g

objects = simpleServer.o linkedlist_new.o utilities.o open_listenfd.o logger.o

main : $(objects)
	cc -pthread -o main $(objects) $(CFLAGS)

main.o : linkedlist_new.h utilities.h open_listenfd.h logger.h

.PHONY : clean
clean :
	rm main $(objects)
