graph: graph.c 
	gcc -o graph graph.c -lm

clean:
	\rm -f *.o graph *~ *#
