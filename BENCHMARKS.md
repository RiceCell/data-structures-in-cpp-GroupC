<div align="left">

# Benchmark Results

**Data Structures in C++ — Group C**
CMSC 123: Data Structures and Algorithms 

> **Compiler flags:** `-O2` across all benchmarks
> **Methodology:** 10 iterations per N, results averaged. Random inputs use fixed seed for reproducibility.
>
> **Russel Niño Buno** — Acer Nitro AN515-58, i5-12500H, 8GB RAM, Windows 11, MSYS2/MinGW64, g++ 13.1 <br>
> **Angelo Mari Manlangit** — *(to add)* <br>
> **Gian Jefferson Reyes** — *(to add)*
</div>

---

## Table of Contents

1. [➤ FILO Queue: Array Stack](#-filo-queue-array-stack)
2. [➤ FIFO Queue: Singly Linked List](#-fifo-queue-singly-linked-list)
3. [➤ Priority Queue: Meldable Heap](#-priority-queue-meldable-heap)
4. [➤ Custom vs STL](#-custom-vs-standard-template-library-stl)
5. [➤ Summary](#-summary)

---

## FILO Queue: Array Stack

```
Elements      Avg Add (s)       Avg Remove (s)    Avg Total (s)
---------------------------------------------------------------
1,000         2.33e-06          2.22e-06          4.55e-06
10,000        4.195e-05         3.609e-05         7.804e-05
100,000       0.00030918        0.00043239        0.00074157
1,000,000     0.00272174        0.00303085        0.00575259
100,000,000   0.406443          0.440052          0.846495
```

**Resize Count** — number of internal array resizes triggered:

| Elements | Resize Count |
|----------|-------------|
| 1,000 | ~10 |
| 10,000 | ~14 |
| 100,000 | ~17 |
| 1,000,000 | ~20 |
| 100,000,000 | ~27 |

> Resize count grows as log₂(N) — confirming amortized O(1) add. At 100 million elements, the array only resizes approximately 27 times total.

---

## FIFO Queue: Singly Linked List

```
Elements      Avg Enqueue (s)    Avg Dequeue (s)    Avg Total (s)
-----------------------------------------------------------------
1,000         2.178e-05          6.05e-06           2.783e-05
10,000        0.00026241         8.78e-05           0.00035021
100,000       0.00320488         0.00191426         0.00511914
1,000,000     0.0270683          0.0108363          0.0379046
100,000,000   3.38204            3.76252            7.14456
```

> SLList is significantly slower than ArrayStack due to per-node heap allocation and pointer chasing. The Node Pooling twist reduces allocation overhead after warmup — freed nodes are recycled instead of deleted, making subsequent pushes nearly allocation-free.

---

## Priority Queue: Meldable Heap

```
Elements     Avg Add (s)     Avg Remove (s)    Avg Total (s)    Avg AddMerge   Avg RmvMerge
------------------------------------------------------------------------------------------
1,000        0.00011379      0.00015537        0.00026916       6,181          8,982
10,000       0.00133925      0.0022751         0.00361435       83,024         135,930
100,000      0.0166986       0.0337382         0.0504369        1,064,317      1,817,715
1,000,000    0.22276         0.766726          0.989487         12,942,617     22,755,033
10,000,000   3.19771         14.8683           18.066           149,053,526    270,332,977
```

### Merge Counter — Empirical O(log n) Proof

| Elements | Avg AddMrg | Avg RmvMrg | AddMrg / Element | RmvMrg / Element | log₂(N) |
|----------|-----------|---------------|---------------------|------------------------|---------|
| 1,000 | 6,181 | 8,982 | 6.18 | 8.98 | 10.0 |
| 10,000 | 83,024 | 135,930 | 8.30 | 13.59 | 13.3 |
| 100,000 | 1,064,317 | 1,817,715 | 10.64 | 18.18 | 16.6 |
| 1,000,000 | 12,942,617 | 22,755,033 | 12.94 | 22.76 | 19.9 |
| 10,000,000 | 149,053,526 | 270,332,977 | 14.91 | 27.03 | 23.3 |

> Add merges per element grow as **~0.65 × log₂(N)** across all tested scales. Remove merges per element track closer to log₂(N) — slightly below at small N and slightly above at large N. This is because `remove()` merges two existing subtrees rather than a single new node against the root, resulting in deeper average traversal. Both are consistent with expected **O(log n)** behavior.

### Absorb Benchmark

Merging two heaps of N elements each:

```
Elements      Avg Absorb (s)
-----------------------------
2,000         3.4e-07
20,000        6e-07
200,000       2.03e-06
2,000,000     3.69e-06
```

> Absorbing 2 million elements takes **~3.69 microseconds**. From 2,000 to 2,000,000 elements — a 1000× increase in data — absorb time grows only ~10×, consistent with O(log n). This is the defining feature of a meldable heap: merging two massive heaps is essentially instant regardless of size. `std::priority_queue` has no equivalent — merging two STL heaps requires re-inserting every element at O(n log n) cost.

---

## Custom-made vs Standard Template Library (STL)

Benchmarked against C++ Standard Library equivalents.
Fixed random seed (67) for reproducibility across all runs.

### ArrayStack vs `std::stack`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 4.99e-06 | 1.91e-06 | 4.51e-06 | 1.18e-06 | STL +207% |
| 10,000 | 6.625e-05 | 2.564e-05 | 8.725e-05 | 9.1e-06 | STL +341% |
| 100,000 | 3.183e-04 | 2.0123e-04 | 3.5111e-04 | 1.0832e-04 | STL +116% |
| 1,000,000 | 3.25475e-03 | 2.53726e-03 | 3.32543e-03 | 1.22032e-03 | STL +75% |

> The gap **narrows significantly** as N grows — from 207% slower at 1K to only 75% slower at 1M. This is the amortized cost of our resize strategy evening out. `std::stack` wraps `std::deque` which uses a chunked memory layout, giving it better cache behavior than our single contiguous array at small sizes.

---

### SLList vs `std::queue`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 2.382e-05 | 1.39e-06 | 8.65e-06 | 4.9e-07 | STL +1627% |
| 10,000 | 2.7071e-04 | 1.515e-05 | 9.62e-05 | 4.53e-06 | STL +1764% |
| 100,000 | 2.66509e-03 | 2.3115e-04 | 1.18127e-03 | 6.056e-05 | STL +1218% |
| 1,000,000 | 2.74355e-02 | 1.98493e-03 | 1.15476e-02 | 8.849e-04 | STL +1258% |

> This is the largest gap and the most expected result. `std::queue` wraps `std::deque` — a chunked array structure that avoids per-element heap allocation entirely. Our SLList calls `new`/`delete` (or pool recycle) on every node, which is fundamentally slower due to pointer chasing and cache misses. This is the classic linked list vs array-backed structure tradeoff — same O(1) complexity, vastly different constant factors.

---

### MeldableHeap vs `std::priority_queue`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 1.1037e-04 | 1.607e-05 | 1.6614e-04 | 3.567e-05 | STL +434% |
| 10,000 | 1.2945e-03 | 1.4239e-04 | 2.33696e-03 | 5.274e-04 | STL +442% |
| 100,000 | 1.57155e-02 | 1.20592e-03 | 3.28409e-02 | 6.28853e-03 | STL +547% |
| 1,000,000 | 2.19174e-01 | 1.17709e-02 | 8.13843e-01 | 8.35353e-02 | STL +983% |

> `std::priority_queue` uses a binary heap on a contiguous `std::vector` — every operation benefits from CPU cache prefetching. Our MeldableHeap uses pointer-based tree nodes scattered in memory, making every merge step a potential cache miss. However, `std::priority_queue` **cannot merge two heaps** — there is no `absorb()` equivalent in the STL. Our MeldableHeap does it in **O(log n) — ~3.69 microseconds for 2 million elements**.

---

## Summary

### Performance vs STL

| Structure | vs STL | Why STL wins | What we have that STL doesn't |
|-----------|--------|-------------|-------------------------------|
| ArrayStack | STL +75–341% | `std::deque` chunked layout | Ghost Scrubbing, Boundary Guarding, Resize Telemetry |
| SLList | STL +1218–1764% | No per-node allocation | Node Pooling, Self-Healing cycle detection |
| MeldableHeap | STL +434–983% | Contiguous array cache locality | `absorb()` — O(log n) heap merge |

### Complexity Reference

| Structure | Add | Remove | Merge | Special |
|-----------|-----|--------|-------|---------|
| ArrayStack | O(1) amortized | O(1) amortized | — | O(n) worst case resize |
| SLList | O(1) | O(1) | — | O(n) get(i) |
| MeldableHeap | O(log n) | O(log n) | O(log n) | absorb() in O(log n) |
| `std::stack` | O(1) amortized | O(1) | — | — |
| `std::queue` | O(1) amortized | O(1) | — | — |
| `std::priority_queue` | O(log n) | O(log n) | O(n log n) | no merge |

> STL wins on raw speed in all cases — this is expected. STL containers are engineered by the C++ standards committee and optimized over decades. The value of these custom implementations lies not in beating STL, but in understanding why it exists, and offering capabilities that STL simply does not provide.

---

<div align="center">

[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

*Data Structures in C++ — Group C*
[← Back to README](./README.md)

</div>
