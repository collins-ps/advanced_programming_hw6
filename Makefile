graph: graph.c 
	gcc -O3 graph graph.c -lm

clean:
	\rm -f *.o graph *~ *#
