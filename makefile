CFLAGS = -g

objects = main.o linkedlist_new.o

main : $(objects)
	cc -o main $(objects)

main.o : linkedlist_new.h

.PHONY : clean
clean :
	rm main $(objects)
