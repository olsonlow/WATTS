P01.out : Project1.o dfa.o SetLimits.o
	gcc -o P01.out Project1.o dfa.o SetLimits.o -g

Project1.o : Project1.c dfa.h
	gcc -c Project1.c -g

dfa.o : dfa.c dfa.h
	gcc -c dfa.c -g

SetLimits.o : SetLimits.c SetLimits.h
	gcc -c SetLimits.c -g

clean :
	rm -f *.o *.out *.*~ *~

run :
	./P01.out
