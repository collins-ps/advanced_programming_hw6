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

**Stationary probability given 50 iterations and 1,000,000 steps (5dp)**:

0.00882 0.01067 0.01164 0.00918 0.00741 0.00901 0.00873 0.01062 0.00910 0.01322 0.01253 0.00968 0.00816 0.00845 0.00905 0.01119 0.01186 0.01093 0.01260 0.00953 0.01066 0.00893 0.00808 0.00852 0.01041 0.00963 0.01246 0.01201 0.00952 0.00930 0.01093 0.00760 0.01238 0.00760 0.01246 0.01188 0.01222 0.00725 0.01008 0.00897 0.00910 0.00835 0.01243 0.00725 0.01205 0.00925 0.01236 0.00788 0.00881 0.01155 0.00936 0.01065 0.01111 0.00861 0.00844 0.00845 0.01029 0.01090 0.01022 0.01215 0.00889 0.00960 0.01055 0.00649 0.00793 0.00919 0.00976 0.00847 0.01237 0.01147 0.00944 0.01264 0.00700 0.00806 0.00988 0.00904 0.01209 0.01128 0.00880 0.01185 0.01154 0.01179 0.01264 0.00912 0.00912 0.01324 0.00845 0.00955 0.00867 0.00769 0.01413 0.01062 0.00959 0.00800 0.00913 0.00912 0.00958 0.01016 0.01053 0.01009 

**Interpretation of findings**:

The program runs x iterations of the Markov Chain algorithm with y steps per iteration. The starting state is chosen randomly each iteration. To reduce error to less than 0.1%, the stationary probabilities calculated in each iteration should be less than 0.1% different from the mean stationary probabilities across all iterations, as this would suggest that the mean stationary probability is independent of the starting state. Given 50 iterations, this condition was fulfilled for 1,000,000 steps per iteration. The mean stationary probabilities is included above.

## Compiling and running
`make graph` and then `./graph x y`, where x is an integer representing the number of iterations and y is an integer representing the number of steps. If no input is provided, default values for x and y are 50 and 100000 respectively. Finally, run `make clean`.
