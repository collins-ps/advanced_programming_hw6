# Homework 6

Using the provided Markov graph as input, calculate the following three things:
- Implement Kosaraju to demonstrate that the graph is irreducible.
- Implement the general graph period algorithm described in Lecture 8, part 5 and find the period of the graph.
- Estimate the stationary probabilities. Be sure to run the chain long enough (or average enough chains) to reduce the error to less than ~0.1% (using heuristics, not a formal error analysis technique). Interpret your findings.

The format of the graph is:

```
node_1 [edge_1, weight_1] [edge_2, weight_2] ...
node_2 [edge_1, weight_1] [edge_2, weight_2] ...
...
node_n [edge_1, weight_1] [edge_2, weight_2] ...
```

Note that due to precision loss you may need to renormalize the graph so that each row sums exactly to 1 with no roundoff error.

# MPCS 51100 HW6
**Name**: Phoebe Collins, UCID: 12277438

## References
I referenced geekforgeeks to implement a function to find the gcd (https://www.geeksforgeeks.org/gcd-two-array-numbers/).

## Discussion on program and performance
I did not observe any compilation or run-time errors. The Valgrind report is clean. 

### Results

**Stationary probability given 10 iterations and 100,000,000 steps (5dp)**:

0.00882 0.01070 0.01163 0.00919 0.00740 0.00900 0.00873 0.01060 0.00912 0.01321 0.01251 0.00966 0.00818 0.00845 0.00905 0.01117 0.01185 0.01094 0.01259 0.00952 0.01065 0.00893 0.00807 0.00853 0.01041 0.00963 0.01246 0.01201 0.00953 0.00930 0.01092 0.00759 0.01236 0.00759 0.01246 0.01189 0.01223 0.00723 0.01007 0.00896 0.00909 0.00838 0.01243 0.00725 0.01205 0.00925 0.01236 0.00787 0.00882 0.01156 0.00935 0.01063 0.01111 0.00861 0.00844 0.00845 0.01029 0.01090 0.01022 0.01217 0.00888 0.00960 0.01054 0.00651 0.00794 0.00918 0.00975 0.00847 0.01238 0.01147 0.00943 0.01265 0.00700 0.00806 0.00989 0.00905 0.01211 0.01126 0.00880 0.01184 0.01153 0.01178 0.01265 0.00912 0.00911 0.01323 0.00846 0.00954 0.00868 0.00770 0.01418 0.01061 0.00958 0.00801 0.00913 0.00915 0.00958 0.01015 0.01052 0.01009 

**Interpretation of findings**:

The program runs x iterations of the Markov Chain algorithm with y steps per iteration. The starting state is chosen randomly each iteration. 

To ensure error is less than 0.1%, I ran the program 5 additional times with the same variables (x = 10 and y = 100,000,000) and calculated the average percentage difference between the estimated stationary probability for each state as listed above and those in the 5 additional sets of probabilities. This was less than 0.1%.

## Compiling and running
`make graph` and then `./graph x y`, where x is an integer representing the number of iterations and y is an integer representing the number of steps. If no input is provided, default values for x and y are 10 and 1,000,000 respectively. Finally, run `make clean`.
