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

**Stationary probability vector (5dp)**:

0.00882 0.01071 0.01163 0.00919 0.00741 0.00898 0.00874 0.01063 0.00912 0.01322 0.01251 0.00966 0.00819 0.00846 0.00904 0.01117 0.01186 0.01094 0.01258 0.00952 0.01068 0.00892 0.00807 0.00853 0.01039 0.00964 0.01248 0.01199 0.00951 0.00928 0.01093 0.00759 0.01236 0.00760 0.01246 0.01187 0.01223 0.00723 0.01007 0.00895 0.00909 0.00837 0.01244 0.00726 0.01206 0.00924 0.01237 0.00787 0.00882 0.01156 0.00936 0.01065 0.01110 0.00861 0.00842 0.00847 0.01030 0.01089 0.01022 0.01216 0.00887 0.00960 0.01054 0.00648 0.00795 0.00917 0.00974 0.00848 0.01239 0.01146 0.00945 0.01263 0.00700 0.00805 0.00989 0.00906 0.01210 0.01127 0.00880 0.01184 0.01151 0.01176 0.01263 0.00913 0.00913 0.01324 0.00846 0.00955 0.00868 0.00769 0.01417 0.01062 0.00958 0.00801 0.00912 0.00915 0.00959 0.01015 0.01054 0.01009

**Interpretation of findings**:

The program runs x iterations of the Markov Chain algorithm with y steps per iteration. The starting state is chosen randomly each iteration. To reduce error to less than 0.1%, the stationary probabilities calculated in each iteration should be less than 0.1% different from the mean stationary probabilities across all iterations, as this would suggest that the mean stationary probability is independent of the starting state. Given 100 iterations, this condition was fulfilled for 1,000,000 steps per iteration. The mean stationary probabilities is included above.

