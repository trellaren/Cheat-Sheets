# 📚 Algorithms & Big‑O Cheat Sheet

> **Tip:**
> • `O(1)` – constant time (no dependence on input size)
> • `O(log n)` – logarithmic (binary search, heap operations)
> • `O(n)` – linear (single pass)
> • `O(n log n)` – typical “fast” sorting or divide‑and‑conquer
> • `O(n²)` – quadratic (nested loops)
> • `O(2ⁿ)` / `O(n!)` – exponential / factorial (brute‑force combinatorics)

| Category                     | Algorithm                            | Typical Use                       | Time Complexity                                                   | Space Complexity                        |
| ---------------------------- | ------------------------------------ | --------------------------------- | ----------------------------------------------------------------- | --------------------------------------- |
| **Sorting**                  | QuickSort (average)                  | General‑purpose, in‑place         | **Avg:** `O(n log n)` `<br>`**Worst:** `O(n²)`                    | `O(log n)` recursion stack              |
|                              | MergeSort                            | Stable sort, linked lists         | `O(n log n)`                                                      | `O(n)` auxiliary array                  |
|                              | HeapSort                             | In‑place, O(1) memory             | `O(n log n)`                                                      | `O(1)`                                  |
|                              | Insertion Sort                       | Small or nearly sorted data       | **Avg/Worst:** `O(n²)`                                            | `O(1)`                                  |
|                              | Selection Sort                       | Simple, no extra space            | `O(n²)`                                                           | `O(1)`                                  |
|                              | Bubble Sort                          | Educational                       | `O(n²)`                                                           | `O(1)`                                  |
| **Searching**                | Binary Search (sorted array)         | Find element                      | `O(log n)`                                                        | `O(1)`                                  |
|                              | Linear Search                        | Unsorted data                     | `O(n)`                                                            | `O(1)`                                  |
|                              | Hash‑table lookup                    | Average constant time             | **Avg:** `O(1)` `<br>`**Worst:** `O(n)` (collision chain)         | Depends on table size                   |
|                              | Trie search                          | Prefix queries                    | `O(k)` (`k` = key length)                                         | `O(total nodes)`                        |
| **Graph**                    | DFS (iterative/recursive)            | Reachability, topological sort    | `O(V + E)`                                                        | `O(V)` recursion stack / explicit stack |
|                              | BFS                                  | Shortest path in unweighted graph | `O(V + E)`                                                        | `O(V)` queue                            |
|                              | Dijkstra (min‑heap)                  | Weighted shortest paths           | `O((V+E) log V)`                                                  | `O(V)`                                  |
|                              | Floyd‑Warshall                       | All‑pairs shortest paths          | `O(V³)`                                                           | `O(V²)`                                 |
|                              | Bellman‑Ford                         | Negative edges                    | `O(V·E)`                                                          | `O(V)`                                  |
|                              | Prim / Kruskal (MST)                 | Minimum spanning tree             | **Prim (heap):** `O((V+E) log V)` `<br>`**Kruskal:** `O(E log E)` | `O(V)` or `O(E)`                        |
| **Dynamic Programming (DP)** | Fibonacci (memoized)                 | Recurrence `F(n)=F(n-1)+F(n-2)`   | `O(n)`                                                            | `O(n)` stack / memo table               |
|                              | 0/1 Knapsack                         | Item selection with weight limit  | `O(n·W)`                                                          | `O(n·W)`                                |
|                              | Longest Increasing Subsequence (LIS) | Sequence analysis                 | **Naïve:** `O(n²)` `<br>`**Patience sorting:** `O(n log n)`       | `O(n)`                                  |
|                              | Edit Distance (Levenshtein)          | String similarity                 | `O(m·n)`                                                          | `O(min(m,n))` with DP optimization      |
|                              | Matrix Chain Multiplication          | Optimal parenthesization          | `O(n³)`                                                           | `O(n²)`                                 |
| **String Algorithms**        | KMP (pattern matching)               | Find pattern in text              | `O(n + m)`                                                        | `O(m)` prefix table                     |
|                              | Rabin‑Karp                           | Average linear, uses hashing      | `O(n+m)` avg `<br>O((n+m)·q)` worst                               | `O(1)` extra                            |
|                              | Boyer–Moore                          | Practical speed up                | **Avg:** sublinear                                                | `O(m)` bad‑char & good‑suffix tables    |
|                              | Trie construction                    | Prefix tree                       | `O(total chars)`                                                  | `O(total nodes)`                        |
| **Data Structures**          | Binary Search Tree (balanced)        | Ordered set/map                   | `O(log n)` insert/lookup/delete                                   | `O(n)` storage                          |
|                              | AVL / Red‑Black Tree                 | Self‑balancing BST                | `O(log n)`                                                        | `O(n)`                                  |
|                              | Hash Map                             | Key‑value store                   | **Avg:** `O(1)`                                                   | Depends on load factor                  |
|                              | Segment Tree                         | Range queries/updates             | Build:`O(n)` `<br>`Query/Update: `O(log n)`                       | `O(n)`                                  |
|                              | Fenwick Tree (BIT)                   | Prefix sums                       | Build:`O(n)` `<br>`Point Update / Prefix Query: `O(log n)`        | `O(n)`                                  |
|                              | Disjoint Set Union (Union‑Find)      | Connected components              | Amortized `α(n)` (inverse Ackermann)                              | `O(n)`                                  |
| **Combinatorial**            | Permutations / Combinations          | Counting subsets                  | Exponential (`O(2ⁿ)` or `n!`)                                     | Depends on implementation               |
|                              | Backtracking (Sudoku, N‑Queens)      | Constraint satisfaction           | Worst `O(k^n)` where k = branching factor                         | `O(n)` recursion depth                  |

---

## Quick Reference

- **`n`** – size of input array / number of vertices
- **`m`** – length of pattern or string
- **`V, E`** – vertices and edges in a graph
- **`W`** – capacity/weight limit in knapsack problems
- **`α(n)`** – inverse Ackermann function (≈ 4 for all practical n)

### Common Patterns

| Pattern                   | Example                  | Complexity                         |
| ------------------------- | ------------------------ | ---------------------------------- |
| Linear scan               | `for i in range(n): ...` | `O(n)`                             |
| Nested loops              | `for i …; for j …`       | `O(n²)` (or higher)                |
| Divide & conquer          | QuickSort, MergeSort     | `T(n)=2T(n/2)+O(n)` → `O(n log n)` |
| Recursion + memoization   | Fibonacci DP             | `O(n)` time, `O(n)` space          |
| Dynamic programming table | 0/1 Knapsack             | `O(n·W)` time & space              |

---

## Tips for Estimating Complexity

1. **Count the dominant term** – ignore lower‑order terms and constants.
2. **Identify recursion depth** – often logarithmic if input is halved each call.
3. **Look at loops nesting** – product of loop ranges gives complexity.
4. **Consider data structure operations** – e.g., `push/pop` on a stack are `O(1)`.
5. **Account for worst‑case vs average‑case** – e.g., QuickSort is `O(n log n)` avg, but `O(n²)` worst.

---

### Quick Cheat Sheet (Print‑Friendly)

```
Algorithm           Avg Time      Worst Time    Space
----------------------------------------------------
QuickSort            O(n log n)    O(n^2)        O(log n)
MergeSort            O(n log n)    O(n log n)    O(n)
HeapSort             O(n log n)    O(n log n)    O(1)

Binary Search        O(log n)      O(log n)      O(1)
Linear Search        O(n)          O(n)          O(1)

DFS/BFS              O(V+E)        O(V+E)        O(V)
Dijkstra             O((V+E) log V) O((V+E) log V) O(V)

Fibonacci (DP)       O(n)          O(n)          O(n)
0/1 Knapsack         O(nW)         O(nW)         O(nW)
LIS (patience sort)  O(n log n)    O(n log n)    O(n)

KMP                 O(n+m)        O(n+m)        O(m)
Rabin‑Karp          O(n+m) avg    O((n+m)q) worst O(1)
```

Feel free to copy this table into a note or print it for quick reference during coding interviews or exams!
