# Traveling salesman problem

An implementation and comparative analysis of two approaches to the Traveling
Salesman Problem (TSP) in C: an exact solver based on the Held Karp dynamic
programming algorithm, and a hybrid approximate solver combining greedy
construction heuristics with 2-OPT local search. The project includes a
formal NP-hardness proof, an automated test generator covering 34 distinct
scenarios, and a full experimental evaluation comparing solution quality and
running time across both solvers.

## Overview

TSP asks for the minimum cost Hamiltonian cycle in a weighted graph: given a
set of cities and the pairwise cost of travel between them, find the
shortest possible route that visits every city exactly once and returns to
the starting point. The problem is NP-hard, so no known algorithm solves it
in polynomial time for arbitrary instances. This project implements and
contrasts two complementary strategies:

* an exact algorithm (Held Karp), which guarantees the global optimum at
  exponential time and memory cost, practical only for small instances
* an approximate algorithm (a hybrid of Nearest Neighbor, Simple Insertion,
  and 2-OPT), which trades a small, usually negligible, loss of optimality
  for polynomial running time on much larger instances

Both solvers read the same input format and share the same distance matrix
representation, allowing their outputs to be compared directly on identical
test cases.

## Repository Structure

```
src/
    Makefile        build rules for both solvers
    TSP_exact.c     exact solver (Held Karp)
    TSP_approx.c    approximate solver (hybrid heuristic)
tests/
    generate_tests.py   generates the 34 test instances
    run_tests.sh         builds both solvers, runs every test, compares results
    input/                generated .in test files
    output_exact/         Held Karp results
    output_approx/        heuristic results
RAPORT_TSP.pdf     full technical report (NP-hardness proof, algorithm
                    description, complexity analysis, experimental results)
```

## Building and Running

Compilation:

```
cd src
make
```

Test generation:

```
make generate-tests
```

or directly:

```
cd tests
python3 generate_tests.py
```

Running the full test suite, which builds both solvers, runs every test
against both implementations, and reports a comparison table:

```
make test
```

or directly:

```
cd tests
./run_tests.sh
```

Cleanup:

```
make clean          # removes executables and test outputs
make clean-tests    # removes all generated tests, including inputs
```

Each test run enforces a timeout of 10 seconds for the exact solver and 5
seconds for the approximate solver, since some generated instances are
deliberately invalid or degenerate.

### Input Format

Both solvers expect the same plain text format:

```
n m
u1 v1 w1
u2 v2 w2
...
um vm wm
```

where `n` is the number of nodes, `m` is the number of edges, and each of
the following `m` lines describes an undirected edge between nodes `u` and
`v` with cost `w`. If the same edge appears more than once, the minimum
reported cost is kept. If no Hamiltonian cycle exists, or the input is
malformed, both solvers print `Nu există soluție.` (No solution exists) and
exit cleanly.

## Exact Solver: Held Karp

`TSP_exact.c` implements the Held Karp algorithm, a dynamic programming
formulation that reduces the brute force factorial complexity of trying
every permutation to an exponential one, based on the principle of
optimality: any sub path of an optimal tour must itself be optimal.

The state is defined as `DP(S, i)`, the minimum cost of a path that starts
at node 0, visits every node in subset `S` (where `0` and `i` both belong to
`S`), and ends at node `i`. The recurrence is:

```
DP(S, i) = min over j in S, j != i of  DP(S \ {i}, j) + cost(j, i)
```

with base case `DP({0, i}, i) = cost(0, i)`. The final answer closes the
tour back to the origin:

```
OPT = min over i != 0 of  DP(V, i) + cost(i, 0)
```

### Bitmasking

Subsets of nodes are represented as integers, where bit `k` is set if and
only if node `k` belongs to the subset. This makes set membership a single
bitwise AND, and removing a node from a subset a single XOR, avoiding any
explicit set data structure. The implementation stores two tables:

* `dp[mask][last]`, the minimum cost of visiting exactly the nodes in
  `mask` and ending at node `last`
* `parinte[mask][last]` (parent), the predecessor node used to reconstruct
  the optimal path once the minimum cost is known

The main loop iterates over every reachable bitmask in increasing order,
and for every valid state tries to extend the path to every node not yet
in the mask, relaxing the cost of the resulting state:

```c
for (int masca = 1; masca <= masca_plina; masca++) {
    for (int u = 0; u < nr_noduri; u++) {
        if (dp[masca][u] == INFINIT) continue;
        for (int v = 0; v < nr_noduri; v++) {
            if ((masca & (1 << v)) == 0) {
                int masca_noua = masca | (1 << v);
                int cost_tranzitie = dp[masca][u] + distanta[u][v];
                if (cost_tranzitie < dp[masca_noua][v]) {
                    dp[masca_noua][v] = cost_tranzitie;
                    parinte[masca_noua][v] = u;
                }
            }
        }
    }
}
```

Once the table is filled, the optimal tour is reconstructed backward from
the best terminating state by repeatedly following `parinte` and clearing
the corresponding bit from the mask, in `O(n)` time.

### Design Choices

The algorithm uses static, globally allocated tables (`dp[1<<20][20]`)
rather than dynamic allocation, avoiding both allocation overhead and
memory fragmentation, at the cost of fixing the maximum supported instance
size (`n <= 20`) at compile time. A dedicated Hamiltonian reachability check
is used before running the full algorithm on very small instances (`n = 1`
or `n = 2`), to sidestep degenerate cases directly.

## Approximate Solver: Hybrid Construction and 2-OPT

`TSP_approx.c` trades the optimality guarantee for polynomial running time,
combining several techniques so that no single heuristic's weaknesses
dominate the result.

### Feasibility Pre check

Before attempting to build a tour, `hamiltonian()` performs a DFS
reachability check from node 0 across all existing edges, rejecting
instances where the graph is not fully connected. This is a necessary but
not sufficient condition for the existence of a Hamiltonian cycle, which is
a known limitation discussed further in the evaluation.

### Construction Step 1: Nearest Neighbor

`cel_mai_apropiat_vecin` (nearest neighbor) builds a tour greedily: starting
from a given node, it repeatedly walks to the closest unvisited node until
every node has been visited, then closes the cycle back to the start. This
runs in `O(n^2)` time but is short sighted, since early greedy choices can
force expensive edges near the end of the tour. To reduce this risk, the
solver runs this construction from multiple starting nodes (0, 1, and 2)
and keeps the best result.

### Construction Step 2: Simple Insertion

`insertie_simpla` (simple insertion) builds a tour incrementally: it starts
from the cheapest available triangle or edge, then repeatedly inserts the
unvisited node that causes the smallest increase in total tour cost at its
best possible position. This runs in `O(n^3)`, since every insertion step
scans all unvisited nodes against all current tour positions. The solver
runs both Nearest Neighbor and Simple Insertion and keeps whichever
produces the lower cost tour as the starting point for refinement.

### Refinement: 2-OPT Local Search

`optimizare_2opt` (2-OPT optimization) iteratively improves the constructed
tour by looking for pairs of edges whose removal and reconnection in the
opposite order reduces total cost:

```
cost(A, C) + cost(B, D) < cost(A, B) + cost(C, D)
```

Whenever such a pair is found, the segment between them is reversed in
place:

```c
int cost_segment_vechi = distante[a][b] + distante[c][d];
int cost_segment_nou = distante[a][c] + distante[b][d];

if (cost_segment_nou < cost_segment_vechi) {
    for (int k = 0; k < (j - i + 1) / 2; k++) {
        int aux = traseu_lucru[i + k];
        traseu_lucru[i + k] = traseu_lucru[j - k];
        traseu_lucru[j - k] = aux;
    }
    imbunatatit = true;
}
```

This process repeats until no improving swap is found or an iteration cap
is reached (100 iterations in this implementation), which bounds the total
work at `O(K * n^2)`.

## Complexity Analysis

### Held Karp (exact)

* Time: `O(n^2 * 2^n)`
* Space: `O(n * 2^n)`
* At `n = 20`, the table holds roughly 20,971,520 states; at 4 bytes per
  entry this is approximately 80 MB, which is close to the practical limit
  for the fixed size static tables used here

### Hybrid heuristic (approximate)

* Nearest Neighbor: `O(n^2)`
* Simple Insertion: `O(n^3)`
* 2-OPT refinement: `O(K * n^2)`, with `K` capped at 100
* Overall: `O(n^3)`, and `O(n^2)` space for the distance matrix alone

At `n = 20`, this is on the order of 8,000 operations against roughly 400
million for Held Karp, which is why the heuristic remains practical well
beyond the instance sizes the exact solver can handle.

### Trade offs

Held Karp guarantees the optimum and is fully deterministic, but is memory
bound: at `n = 24` the required table would already exceed 1.2 GB, and it
offers no partial result if interrupted before completion. The heuristic
scales to hundreds or thousands of nodes in a fraction of a second and uses
only `O(n^2)` memory, but provides no guaranteed bound on its error and can
become stuck in local minima that 2-OPT alone cannot escape.

## Test Suite

The test generator, `generate_tests.py`, produces 34 deterministic test
cases (fixed random seeds) covering degenerate inputs, regular geometric
structures, random distributions, adversarial patterns, and malformed
input, organized in roughly increasing order of complexity. Each test is
written in the shared input format described above.

The categories covered include:

* degenerate and invalid sizes (negative or missing node counts, a single
  node, two nodes)
* regular geometric shapes (equilateral triangle, square, circle, grid,
  star, spiral)
* statistical distributions (uniform random points, clustered points)
* numeric edge cases (very large costs, very small costs, mixed scales,
  powers of two)
* structural stress cases (duplicate edges, sparse graphs, incomplete
  graphs without a Hamiltonian cycle, grids with missing edges)
* malformed input handling (truncated files, non numeric tokens, missing
  header, whitespace only files)

`run_tests.sh` compiles both solvers, runs every generated test through
each of them under a timeout, and prints a summary table with the exact
cost, the approximate cost, their ratio, and a match or mismatch indicator
per test.

## Experimental Results

Measured on an Intel Core i7 10710U (6 cores, 12 threads), 16 GB RAM,
Ubuntu 24.04 under WSL2, compiled with GCC 13.3.0 at `-O2`.

Across the 34 tests:

* 24 tests (70.6 percent) show an exact match between the two solvers
* 29 tests (85.3 percent) show an approximation error of 5 percent or less
* 1 test shows the heuristic reporting no solution where one exists

The exact solver's running time grows with instance size as expected from
its exponential complexity, ranging from about 0.01 seconds for trivial
cases up to roughly 0.5 seconds at `n = 20`. The approximate solver's
running time stays close to 0.01 seconds regardless of instance size or
structure, consistent with its polynomial complexity, giving a measured
speedup over the exact solver ranging from roughly 7 times on small
instances to over 50 times at `n = 20`.

Two results stand out during evaluation:

* On a test with pseudo random, structurally unremarkable edge costs, the
  heuristic's tour cost is about 39 percent above optimal, illustrating how
  greedy construction can be misled when no exploitable geometric or
  numeric pattern exists.
* On a test built from two dense clusters connected by very few edges, the
  heuristic reports no solution even though a Hamiltonian cycle exists.
  This is a direct consequence of the DFS based feasibility check only
  verifying graph connectivity, not the actual existence of a Hamiltonian
  cycle, which is a known limitation of that pre check rather than of the
  construction or refinement steps themselves.

Full per test tables, timing breakdowns, and further discussion are
available in the accompanying report.

## Conclusions

Neither solver dominates the other unconditionally; the right choice
depends on instance size, the cost of sub optimality, and available
resources.

The exact solver is the right choice when the instance is small (`n <= 20`),
when the mathematical guarantee of optimality matters (safety critical
applications, VLSI routing, certain medical scheduling problems), when
sufficient memory is available, or when generating ground truth values to
evaluate a heuristic against.

The approximate solver is the right choice for large or very large
instances where the exact approach is not physically feasible, for
latency sensitive applications such as real time routing, for
resource constrained environments such as embedded or mobile systems, and
whenever a solution within a small percentage of optimal is an acceptable
trade for a large speedup.

A practical TSP solver would combine both: dispatch to Held Karp
automatically for small instances to guarantee optimality, fall back to the
hybrid heuristic for larger ones, and optionally strengthen the heuristic
further with multiple restarts of 2-OPT from different starting tours, or
validate its output against a theoretical lower bound to give the caller a
quantified confidence interval on solution quality.

## References

1. Held, M., and Karp, R. M. (1962). A dynamic programming approach to
   sequencing problems. Journal of the Society for Industrial and Applied
   Mathematics.
2. Kirkpatrick, S., Gelatt, C. D., and Vecchi, M. P. (1983). Optimization by
   Simulated Annealing. Science.
3. Cormen, T. H., Leiserson, C. E., Rivest, R. L., and Stein, C. (2009).
   Introduction to Algorithms (3rd ed.). MIT Press.
4. Applegate, D. L., Bixby, R. E., Chvatal, V., and Cook, W. J. (2006). The
   Traveling Salesman Problem: A Computational Study. Princeton University
   Press.
5. Johnson, D. S., and McGeoch, L. A. (1997). The traveling salesman
   problem: A case study in local optimization. Local search in
   combinatorial optimization.
6. Lin, S., and Kernighan, B. W. (1973). An effective heuristic algorithm
   for the traveling salesman problem. Operations Research.
7. Croes, G. A. (1958). A method for solving traveling salesman problems.
   Operations Research.
8. Garey, M. R., and Johnson, D. S. (1979). Computers and Intractability: A
   Guide to the Theory of NP Completeness. W. H. Freeman.
9. Karp, R. M. (1972). Reducibility Among Combinatorial Problems. In
   Complexity of Computer Computations. Plenum Press.
10. GNU Project. The GNU C Library Reference Manual.
