# Algorithms and Data Structures

This repository contains implementations of fundamental algorithms and data structures written in C++.  
The implementations were developed as part of coursework and exercises in algorithms and data structures.

The goal of this repository is to demonstrate the practical implementation of classical algorithms used in computer science, including graph algorithms, hash tables, and priority queue structures.

---

# Repository Structure

```
Algorithms-and-Data-Structures
│
├── binomial_heaps
├── dfs_bfs_topological-sorting_scc
├── hashtables_chaining_and_open_addressing
├── prim_bellman-ford_dijkstra
└── README.md
```

Each folder contains an implementation of a specific data structure or algorithm category.

---

# 1. Binomial Heaps

## Description

The folder `binomial_heaps` contains an implementation of a **Binomial Heap**, a priority queue data structure that supports efficient merging of heaps.

A binomial heap consists of a set of **binomial trees**, where each tree satisfies the heap property.

### Key Properties

- Each binomial tree follows the **min-heap property**
- Trees are ordered by increasing degree
- No two trees have the same degree
- Efficient merge operations

### Supported Operations

Typical binomial heap operations include:

```
insert(key)
findMin()
extractMin()
merge(heap)
decreaseKey()
delete(key)
```

### Time Complexity

| Operation | Complexity |
|--------|--------|
| Insert | O(log n) |
| Find minimum | O(log n) |
| Extract minimum | O(log n) |
| Merge | O(log n) |

---

# 2. Graph Algorithms (DFS, BFS, Topological Sorting, SCC)

## Description

The folder `dfs_bfs_topological-sorting_scc` contains implementations of fundamental **graph traversal and analysis algorithms**.

These algorithms are commonly used in many areas such as:

- dependency resolution
- scheduling problems
- network analysis
- compiler design

### Implemented Algorithms

#### Depth First Search (DFS)

DFS explores a graph by going as deep as possible before backtracking.

Applications:

- cycle detection
- connected components
- topological sorting
- strongly connected components

Time complexity:

```
O(V + E)
```

---

#### Breadth First Search (BFS)

BFS explores a graph level by level starting from a given node.

Applications:

- shortest paths in unweighted graphs
- connectivity analysis
- level traversal

Time complexity:

```
O(V + E)
```

---

#### Topological Sorting

Topological sorting produces a **linear ordering of vertices** in a **directed acyclic graph (DAG)** such that:

```
if there is an edge u → v
then u appears before v in the ordering
```

Applications:

- task scheduling
- dependency resolution
- build systems

---

#### Strongly Connected Components (SCC)

A **strongly connected component** is a maximal subset of vertices where every vertex can reach every other vertex.

Typical algorithms:

- Kosaraju algorithm
- Tarjan algorithm

Time complexity:

```
O(V + E)
```

---

# 3. Hash Tables (Chaining and Open Addressing)

## Description

The folder `hashtables_chaining_and_open_addressing` contains implementations of hash tables using two different collision resolution strategies.

### Hash Table Concept

A hash table maps a key to a value using a **hash function**.

```
index = hash(key)
```

If multiple keys map to the same index, a **collision** occurs.

---

## Separate Chaining

In chaining, each table index stores a **linked list of elements**.

Example:

```
Index | Elements
--------------
0     | (1, A)
1     | (3, B) -> (7, C)
2     | empty
```

Advantages:

- simple implementation
- table never completely full

Disadvantages:

- additional memory overhead

---

## Open Addressing

In open addressing all elements are stored directly inside the table.

If a collision occurs, another position is searched.

### Linear Probing

```
h(k), h(k)+1, h(k)+2 ...
```

### Quadratic Probing

```
h(k) + i²
```

### Double Hashing

Uses a second hash function:

```
h(k, i) = (h1(k) + i * h2(k)) mod n
```

---

# 4. Shortest Path Algorithms (Prim, Bellman-Ford, Dijkstra)

## Description

The folder `prim_bellman-ford_dijkstra` contains implementations of classical **graph optimization algorithms**.

These algorithms are widely used in:

- navigation systems
- routing algorithms
- network optimization

---

## Dijkstra Algorithm

Computes the shortest path from a source node to all other nodes in a graph with **non-negative edge weights**.

Time complexity:

```
O((V + E) log V)
```

---

## Bellman-Ford Algorithm

Computes shortest paths even if **negative edge weights** exist.

It can also detect **negative cycles**.

Time complexity:

```
O(V * E)
```

---

## Prim Algorithm

Prim’s algorithm computes a **Minimum Spanning Tree (MST)** of a weighted graph.

The MST connects all vertices with the minimal total edge weight.

Time complexity:

```
O(E log V)
```

---

# Compilation

The programs are written in **C++** and can be compiled using `g++`.

Example compilation:

```
g++ program.cpp -o program
```

Example:

```
g++ hashtest.cxx -o hashtest
```

---

# Running the Programs

After compilation the program can be executed from the terminal.

Example:

```
./hashtest 10 c
```

Meaning:

```
10  -> size of hash table
c   -> chaining
```

Possible modes:

```
c -> chaining
l -> linear probing
q -> quadratic probing
d -> double hashing
```

Example commands during execution:

```
p 1 2 Max
g 1 2
d
r 1 2
q
```

Command explanation:

| Command | Meaning |
|------|------|
| p x y name | insert element |
| g x y | get element |
| r x y | remove element |
| d | dump table |
| q | quit |

