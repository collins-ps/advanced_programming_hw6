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

The program runs x iterations of the Markov Chain algorithm with y steps per iteration to estimate the stationary probabilities. The starting state is chosen randomly each of the x iterations. 

As a proxy for error, the program runs z additional times with the same x and y variables. It calculates the average percentage difference in the estimated stationary probability for each state in the results (i.e. the first run) and those in each of the z checks.

### Results

**Stationary probability given 10 iterations, 20,000,000 steps, and 5 checks (5dp)**:

0.00882 0.01071 0.01163 0.00919 0.00740 0.00901 0.00873 0.01062 0.00911 0.01321 0.01253 0.00966 0.00818 0.00845 0.00903 0.01116 0.01185 0.01094 0.01259 0.00953 0.01064 0.00893 0.00806 0.00853 0.01040 0.00963 0.01246 0.01201 0.00952 0.00929 0.01093 0.00760 0.01236 0.00759 0.01246 0.01188 0.01223 0.00722 0.01008 0.00896 0.00910 0.00837 0.01243 0.00726 0.01205 0.00925 0.01236 0.00786 0.00882 0.01155 0.00937 0.01062 0.01111 0.00860 0.00844 0.00846 0.01030 0.01091 0.01023 0.01218 0.00888 0.00960 0.01054 0.00650 0.00795 0.00917 0.00976 0.00847 0.01239 0.01148 0.00944 0.01264 0.00699 0.00807 0.00989 0.00905 0.01210 0.01126 0.00879 0.01186 0.01152 0.01178 0.01264 0.00913 0.00910 0.01323 0.00846 0.00955 0.00867 0.00770 0.01418 0.01061 0.00959 0.00801 0.00912 0.00915 0.00958 0.01014 0.01054 0.01011 

Average percentage difference (proxy for error): 0.079%.

## Compiling and running
`make graph` and then `./graph x y z`, where x is an integer representing the number of iterations, y is an integer representing the number of steps, and z is the integer representing number of checks. If no input is provided, default values for x, y, and z are 10, 1,000,000, and 0 respectively. Finally, run `make clean`.
