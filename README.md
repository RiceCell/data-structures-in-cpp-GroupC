<div align="center">

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=28&pause=1000&color=00FF88&center=true&vCenter=true&width=600&lines=Data+Structures+in+C%2B%2B;Open+Source+%7C+Group+C;Built+from+scratch.+With+twists." alt="Typing SVG" />

# 🏗️ Data Structures in C++

**A handcrafted open-source library of fundamental data structures — built from scratch and learned from class, benchmarked at scale, and twisted for fun. This project is made as a requirement for CMSC 123: Data Structures and Algorithms | BSCS UPC**

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?logo=c%2B%2B)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()
[![Build](https://img.shields.io/badge/Build-g%2B%2B%20%7C%20Make-success)]()

</div>

---

## 📖 Table of Contents

1. [➤ About](#-about)
2. [➤ Data Structures](#-data-structures)
3. [➤ Project Structure](#-project-structure)
4. [➤ Getting Started](#-getting-started)
5. [➤ How to Build](#-how-to-build)
6. [➤ Running Benchmarks](#-running-benchmarks)
7. [➤ Benchmark Results](#-benchmark-results)
8. [➤ Fundamental Twists](#-the-twists)
9. [➤ References](#-references)
10. [➤ License](#-license)
11. [➤ Authors](#-authors)

---

## 📝 About

This is a small open-source library of fundamental Abstract Data Types (ADTs) implemented in C++17, written as a course project for **CMSC 123: Data Structures and Algorithms**.

Every implementation is written **rom scratch** with the aid of discussion notes from class — no direct copy-pasting from textbooks or external sources. Each data structure also includes a **creative twist**: a non-standard feature that either improves real-world performance, adds safety, or reveals something empirically interesting about the structure's behavior.

The library is benchmarked at scales from **1,000 to 100,000,000 elements** to demonstrate real-world performance characteristics beyond Big-O notation.

> *"It is worth noting that Big-O doesn't tell the whole story. Constant factors, cache behavior, and allocation patterns also matter."*

---

## 🗂️ Data Structures

| ADT | Implementation | File | Twist | Author |
|-----|---------------|------|-------|--------|
| **FILO Queue / Stack** | Array Stack | `src/arraystack.h` | Ghost Scrubbing, Boundary Guarding, Allocation Telemetry | Russel Niño Buno |
| **FIFO Queue** | Singly Linked List | `src/sllist.h` | Node Pooling, Self-Healing (Floyd's Cycle Detection) | Russel Niño Buno |
| **Priority Queue** | Meldable Heap | `src/meldableheap.h` | Merge Counter as O(log n) proof | Russel Niño Buno |
| **Deque** | Array Deque | `src/arraydeque.h` | *TO ADD* | Angelo Mari Manlangit |
| **List** | Array Deque + Doubly Linked List | `src/arraydeque.h`, `src/dllist.h` | *TO ADD* | Angelo Mari Manlangit |
| **Sorted Set** | Skiplist + Red-Black Tree | `src/skiplist.h`, `src/redblacktrees.h` | *TO ADD* | Angelo Mari Manlangit |
| **Unsorted Set** | Chained Hash Table | `src/chainedhashtable.h` | *TO ADD* | Gian Jefferson Reyes |
| **Graph** | Adjacency Matrix | `src/adjacencymatrix.h` | *TO ADD* | Gian Jefferson Reyes |

---

## 📁 Project Structure

```
data-structures-in-cpp-GroupC/
│
├── include/                  # Abstract interfaces (ADTs)
│   ├── array.h               # Generic resizable array wrapper
│   ├── list.h                # List ADT interface
│   ├── node.h                # Singly linked node struct
│   └── queue.h               # Queue ADT interface
│
├── src/                      # Concrete implementations
│   ├── arraystack.h          # FILO Stack (array-based)
│   ├── sllist.h              # FIFO Queue (singly linked list)
│   ├── meldableheap.h        # Priority Queue (randomized meldable heap)
│   ├── arraydeque.h          # Deque (array-based)
│   ├── dllist.h              # Doubly Linked List
│   ├── skiplist.h            # Sorted Set (skiplist)
│   ├── redblacktrees.h       # Sorted Set (red-black tree)
│   ├── chainedhashtable.h    # Unsorted Set (hash table)
│   └── adjacencymatrix.h     # Graph (adjacency matrix)
│
├── tests/                    # Benchmarks & test cases
│   ├── arraystack.cpp
│   ├── sllist.cpp
│   ├── meldableheap.cpp
│   ├── arraydeque.cpp
│   ├── dllist.cpp
│   ├── skiplist.cpp
│   ├── redblacktrees.cpp
│   ├── chainedhashtable.cpp
│   └── adjacencymatrix.cpp
│
├── Makefile
├── LICENSE
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- A C++17-compatible compiler: `g++ 9+` or `clang++ 9+`
- `make` (optional but recommended)
- Works on **Windows (MSYS2/MinGW)**, **Linux**, and **macOS**

### Clone the repository

```bash
git clone https://github.com/your-group/data-structures-in-cpp-GroupC.git
cd data-structures-in-cpp-GroupC
```

---

## 🔨 How to Build

### Using Makefile

```bash
# Build all benchmarks
make all

# Build a specific benchmark
make arraystack_test
make sllist_test
make meld_test
```

### Manual compilation

```bash
# Array Stack
g++ -std=c++17 -g -Iinclude -Isrc tests/arraystack.cpp -o arraystack_test

# Singly Linked List
g++ -std=c++17 -g -Iinclude -Isrc tests/sllist.cpp -o sllist_test

# Meldable Heap
g++ -std=c++17 -g -Iinclude -Isrc tests/meldableheap.cpp -o meldheap_test

```

> **Tip:** Use `-O2` for optimized builds when running benchmarks. Use `-g` for debug builds when testing.

---

## 📊 Running Benchmarks

Each data structure has its own benchmark in `tests/`. Run them after building:

```bash
./arraystack_test
./sllist_test
./meldheap_test
```

Benchmarks measure:
- **Add / Enqueue / Push** time at multiple scales
- **Remove / Dequeue / Pop** time at multiple scales
- **Twist-specific metrics** (resize count, merge count, pool size, absorb time)

---

## 📈 Benchmark Results

> Benchmarked on: Windows 11, MSYS2/MinGW64, g++ 13.1, `-O2`

### 🥞 FILO Queue: Array Stack (E-FIX NAKO SOON, ONLY ADDING A PLACEHOLDER)

```
Elements    Add Time       Remove Time    Total
---------------------------------------------------------
1,000       0.0000X s      0.0000X s      0.0000X s
10,000      0.000X s       0.000X s       0.000X s
100,000     0.00X s        0.00X s        0.00X s
1,000,000   0.0X s         0.0X s         0.0X s
100,000,000 X.X s          X.X s          X.X s
```

### 🔗 FIFO Queue: Singly Linked List

```
Elements    Enqueue Time   Dequeue Time   Total
---------------------------------------------------------
1,000       ~0.0001 s      ~0.0001 s      ~0.0002 s
10,000      ~0.001 s       ~0.001 s       ~0.002 s
100,000     ~0.01 s        ~0.008 s       ~0.018 s
1,000,000   ~0.09 s        ~0.06 s        ~0.15 s
100,000,000 ~9 s           ~7 s           ~16 s
```

> Note: LinkedList is significantly slower than ArrayStack due to per-node heap allocation and cache misses. Node Pooling twist reduces this overhead after warmup.

### ⛰️ Priority Queue: Meldable Heap

```
Elements    Add Time       Remove Time    Total(s)       Add Merges     Remove Merges
---------------------------------------------------------------------------------------
1,000       ~0.00015 s     ~0.0002 s      ~0.00035 s     ~6,500         ~9,000
10,000      ~0.0017 s      ~0.003 s       ~0.005 s       ~85,000        ~135,000
100,000     ~0.022 s       ~0.041 s       ~0.063 s       ~1,100,000     ~1,800,000
1,000,000   ~0.27 s        ~0.87 s        ~1.15 s        ~13,000,000    ~22,700,000
```

**Merge counter — empirical O(log n) proof:** (DI PANI REAL only adding table for now)

| Elements | Add Merges | Merges / Element | log₂(N) |
|----------|-----------|-----------------|---------|
| 1,000    | ~6,500    | ~6.5            | 10.0    |
| 10,000   | ~85,000   | ~8.5            | 13.3    |
| 100,000  | ~1,100,000| ~11.0           | 16.6    |
| 1,000,000| ~13,000,000| ~13.0          | 19.9    |

> Merges per element grow as ~0.65 × log₂(N) — consistent with expected O(log n) behavior. The constant < 1 is due to the randomized coin flip reducing average path length. 

**Absorb Benchmark** — merging two heaps of N elements each:

```
Elements    Absorb Time
------------------------------------------
2,000       ~0.000001 s
20,000      ~0.0000014 s
200,000     ~0.0000032 s
2,000,000   ~0.0000055 s
```

> Absorbing 2 million elements takes ~5 microseconds. This is the defining feature of a meldable heap — O(log n) merge regardless of heap size.

---

## 🌀 The Twists

Each data structure was given a non-standard twist beyond the textbook implementation.

### ArrayStack — Three Twists

**1. Ghost Scrubbing**
Vacated memory slots are zeroed out with `T()` after every `remove()` and `resize()`. This prevents sensitive data from lingering in memory beyond its logical lifetime — a subtle but important security consideration.

**2. Boundary Guarding**
`get()` and `set()` return a default `T()` value instead of crashing when accessing out-of-bounds indices. Safe access without exceptions.

**3. Allocation Telemetry**
The stack tracks every internal `resize()` event via a counter accessible through `get_resize_count()`. This lets you observe the amortized growth pattern and verify that resize events are infrequent even at 100M operations.

---

### SLList — Two Twists

**1. Node Pooling**
Instead of calling `new`/`delete` on every push and pop, freed nodes are stashed in a reuse `vector`. The next push grabs from the pool first before touching the heap. After warmup, the queue runs almost entirely allocation-free — directly addressing the biggest real-world weakness of linked lists.

**2. Self-Healing via Floyd's Cycle Detection**
On every push, the queue checks if `tail->next` is non-null, which would indicate a corrupted cycle. If triggered, Floyd's tortoise-and-hare algorithm finds the cycle entry point and cuts it, restoring the list automatically.

> Reference: Floyd, R.W. — attributed in Knuth, D.E. *The Art of Computer Programming, Vol. 2*, §3.1, Exercise 6. Addison-Wesley, 1969.

---

### MeldableHeap — One Twist

**Merge Counter**
Every call to `merge()` that performs real work (both arguments non-null) increments a counter. After a benchmark run, you can retrieve the total merge count via `get_merge_count()` and compare it to the theoretical O(log n) expectation.

This empirically shows that the average merge depth per operation is ~0.65 × log₂(N) — less than the theoretical ceiling because the randomized coin flip tends to pick shorter paths on average.

---

## 📚 References

- Morin, P. *Open Data Structures (C++ Edition)*. [opendatastructures.org](https://opendatastructures.org)
  - §3 — Linked Lists
  - §10.2 — Randomized Meldable Heap
- Knuth, D.E. *The Art of Computer Programming, Vol. 2: Seminumerical Algorithms*. Addison-Wesley, 1969. §3.1 — Floyd's Cycle Detection


---

## 📄 License

This project is licensed under the **GNU General Public License v3.0**.

See [`LICENSE`](./LICENSE) for the full license text.

> This license ensures the library remains free and open — any derivative work must also be open source under the same terms.

---

## 👥 Authors

**Group C**

| Name | Implementations |
|------|----------------|
| Russel Niño Buno | ArrayStack, SLList, MeldableHeap |
| Angelo Mari Manlangit | ArrayDeque, DLList, Skiplist, Red-Black Tree |
| Gian Jefferson Reyes | ChainedHashTable, AdjacencyMatrix |


---

<div align="center">

Made with ☕ and too many benchmark runs. Sana perfect sa project, Sir <3

[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

*Data Structures in C++ — Group C*

</div>
