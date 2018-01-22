Flags = -Wall -Werror -g -std=gnu99
C = gcc


main : Makefile   main.o simple_threads.o simple_semaphores.o
	$(C) $(Flags) main.o simple_threads.o simple_semaphores.o -o main

main.o : Makefile   main.c simple_threads.h
	$(C) -c $(Flags) main.c -o  main.o

simple_semaphores.o : Makefile simple_threads.h simple_semaphores.c simple_semaphores.h
	$(C) -c $(Flags) simple_semaphores.c -o simple_semaphores.o

simple_threads.o : Makefile simple_threads.c simple_threads.h
	$(C) -c $(Flags) simple_threads.c -o simple_threads.o

run : main
	./main

clean : main
		$(RM) main
		$(RM) main.o
		$(RM) simple_threads.o