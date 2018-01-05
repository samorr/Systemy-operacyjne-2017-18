Flags = -Wall -Werror
C = gcc


main : Makefile   main.o simple_threads.o
	$(C) $(Flags) main.o simple_threads.o -o main

main.o : Makefile   main.c simple_threads.h
	$(C) -c $(Flags) main.c -o  main.o

simple_threads.o : Makefile simple_threads.c simple_threads.h
	$(C) -c $(Flags) simple_threads.c -o simple_threads.o

run : main
	./main

clean : main
		$(RM) main
		$(RM) main.o
		$(RM) simple_threads.o