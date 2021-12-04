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
