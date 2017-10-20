Flags = -Wall -Werror -std=c++11
CPP = g++


main : Makefile   main.o 
	$(CPP) $(Flags) -pthread main.cpp -o main

main.o : Makefile   main.cpp 
	$(CPP) -c $(Flags) main.cpp -o  main.o

run : main
	./main

clean : main
		$(RM) main
		$(RM) main.o
