# 📚 Data Structures Cheat Sheet

> A quick‑reference guide for the most common data structures, their core operations, and typical time/space complexities.
> **Use this as a “cheat sheet” during interviews, exams, or while coding—no deep theory, just what you need to remember.**

---

## 1️⃣ Arrays (Static / Dynamic)

| Operation                     | Best‑case      | Worst‑case    |
| ----------------------------- | -------------- | ------------- |
| Access `A[i]`                 | O(1)           | –             |
| Search                        | O(n)           | –             |
| Insert at end (dynamic array) | Amortized O(1) | O(n) (resize) |
| Insert at position            | O(n)           | –             |
| Delete at position            | O(n)           | –             |

_Use‑cases_: Random access, fixed‑size data, contiguous memory for cache friendliness.

---

## 2️⃣ Linked Lists

| Type       | Core Ops                                       | Complexity |
| ---------- | ---------------------------------------------- | ---------- |
| **Singly** | Insert/Delete at head                          | O(1)       |
|            | Search/Insert/Delete at tail (no tail pointer) | O(n)       |
|            | Reverse (iterative)                            | O(n)       |
| **Doubly** | Insert/Delete at any node (given reference)    | O(1)       |
|            | Search                                         | O(n)       |

_Use‑cases_: Frequent insertions/deletions, when you don’t need random access.

---

## 3️⃣ Stacks

| Operation  | Complexity |
| ---------- | ---------- |
| Push       | O(1)       |
| Pop        | O(1)       |
| Peek / Top | O(1)       |
| IsEmpty    | O(1)       |

_Implementation_: Array (dynamic), singly linked list.
_Use‑cases_: Recursion, backtracking, expression evaluation.

---

## 4️⃣ Queues

| Operation    | Complexity |
| ------------ | ---------- |
| Enqueue      | O(1)       |
| Dequeue      | O(1)       |
| Peek / Front | O(1)       |
| IsEmpty      | O(1)       |

_Implementation_: Circular array, singly linked list.
_Use‑cases_: BFS, task scheduling.

---

## 5️⃣ Double‑Ended Queue (Deque)

| Operation        | Complexity |
| ---------------- | ---------- |
| InsertFront/Back | O(1)       |
| DeleteFront/Back | O(1)       |
| PeekFront/Back   | O(1)       |

_Use‑cases_: Sliding window problems, two‑way traversal.

---

## 6️⃣ Hash Table / Dictionary

| Operation     | Complexity (average) | Worst‑case |
| ------------- | -------------------- | ---------- |
| Insert        | O(1)                 | O(n)       |
| Delete        | O(1)                 | O(n)       |
| Search/Lookup | O(1)                 | O(n)       |

_Collision Resolution_: Separate chaining, open addressing (linear probing, quadratic, double hashing).
_Use‑cases_: Constant‑time key/value lookups.

---

## 7️⃣ Binary Trees

### 7.1 Basic Binary Tree

- **Nodes**: value + left/right pointers.
- **Traversal**: In‑order, Pre‑order, Post‑order (recursive or iterative).
- **Complexities**:
  - Height = O(n) worst case; O(log n) average for balanced trees.

### 7.2 Binary Search Tree (BST)

| Operation            | Complexity                 |
| -------------------- | -------------------------- |
| Insert/Search/Delete | O(h) where h = tree height |

_Balanced BSTs_: AVL, Red‑Black → guarantee `h = O(log n)`.

---

## 8️⃣ Balanced Trees

| Structure            | Key Properties                                                                          | Typical Operations                   | Complexity |
| -------------------- | --------------------------------------------------------------------------------------- | ------------------------------------ | ---------- |
| **AVL Tree**         | Height difference ≤ 1 between subtrees                                                  | Insert/Delete/Search                 | O(log n)   |
| **Red‑Black Tree**   | Red/black coloring, root black, no two reds in a row, same number of blacks on any path | Insert/Delete/Search                 | O(log n)   |
| **B‑Tree (order m)** | Each node 2–m children; all leaves at same depth                                        | Disk‑friendly search, insert, delete | O(logₘ n)  |

_Use‑cases_: STL `map`/`set`, database indexes.

---

## 9️⃣ Heaps

### 9.1 Binary Heap (Array Representation)

- | **Max‑Heap / Min‑Heap**: Parent ≥ / ≤ children. | Operation | Complexity |
  | ----------------------------------------------- | --------- | ---------- |
  | Insert                                          | O(log n)  |
  | DeleteMin/Max                                   | O(log n)  |
  | PeekMin/Max                                     | O(1)      |
  | Build‑heap (from array)                         | O(n)      |

### 9.2 Binomial & Fibonacci Heaps

- **Fibonacci Heap**: `Insert` O(1), `ExtractMin` O(log n), `DecreaseKey` O(1).
  _Use‑cases_: Dijkstra’s algorithm, Prim’s MST.

---

## 🔟 Graphs

| Representation       | Storage           | Typical Complexity |
| -------------------- | ----------------- | ------------------ |
| **Adjacency Matrix** | n² boolean/weight | Space O(n²)        |
| **Adjacency List**   | n + e             | Space O(n+e)       |

### Common Algorithms

- **DFS / BFS**: `O(V + E)`
- **Dijkstra** (binary heap): `O((V+E) log V)`
- **Bellman‑Ford**: `O(VE)`
- **Kruskal**: `O(E log E)`
- **Prim** (with binary heap): `O((V+E) log V)`

---

## 11️⃣ Trie (Prefix Tree)

| Operation            | Complexity                |
| -------------------- | ------------------------- |
| Insert/Search Prefix | O(k) where k = key length |
| Delete               | O(k)                      |

_Use‑cases_: Autocomplete, dictionary lookups, longest prefix matching.

---

## 12️⃣ Disjoint Set Union (Union–Find)

| Operation | Complexity (amortized)   |
| --------- | ------------------------ |
| Find      | α(n) (inverse Ackermann) |
| Union     | α(n)                     |

_Use‑cases_: Kruskal’s algorithm, connectivity queries.

---

## 🛠️ Quick Implementation Snippets

```python
# Array
arr = [1, 2, 3]
x = arr[0]          # O(1)

# Linked List Node (singly)
class Node:
    def __init__(self,val):
        self.val=val; self.next=None

head = Node(5); head.next = Node(10)

# Stack with list
stack = []
stack.append(x)     # push
x = stack.pop()     # pop

# Queue with deque
from collections import deque
q = deque()
q.append(x)
x = q.popleft()

# HashMap (dict)
h = {'a':1}
value = h.get('b', 0)

# Binary Search Tree Node
class BSTNode:
    def __init__(self,val):
        self.val=val; self.left=self.right=None

# Heap (min-heap)
import heapq
heap = []
heapq.heappush(heap, 3)
min_val = heapq.heappop(heap)
```

---

## 📌 Key Takeaways

| Concept            | When to Use                                |
| ------------------ | ------------------------------------------ |
| **Array**          | Fixed size, random access.                 |
| **Linked List**    | Frequent insert/delete; need dynamic size. |
| **Stack / Queue**  | LIFO / FIFO patterns.                      |
| **Hash Table**     | Fast key/value lookups.                    |
| **BST (Balanced)** | Ordered data with log‑time ops.            |
| **Heap**           | Priority queues, scheduling.               |
| **Graph**          | Relationships, networks.                   |
| **Trie**           | Prefix searches.                           |
| **Union–Find**     | Connectivity, cycle detection.             |

Keep this sheet handy for quick reference—especially during coding interviews or when you need to decide the best data structure for a problem!
