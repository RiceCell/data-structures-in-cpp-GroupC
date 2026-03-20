<div align="center">

# Benchmark Results

**Data Structures in C++ — Group C**
CMSC 123: Data Structures and Algorithms | BS Computer Science, UP Cebu

> Benchmarked on: Windows 11, MSYS2/MinGW64, g++ 13.1, `-O2`
> Random inputs use fixed seed for reproducibility.

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
Elements      Add Time (s)      Remove Time (s)   Total (s)
------------------------------------------------------------
1,000         ~1.13e-05         ~4.2e-06          ~1.55e-05
10,000        ~7.17e-05         ~4.35e-05         ~0.0001152
100,000       ~0.0003166        ~0.0003475        ~0.0006641
1,000,000     ~0.0028325        ~0.002509         ~0.0053415
10,000,000    ~0.494428         ~0.621236         ~1.11566
```

**Resize Count** — number of internal array resizes triggered:

| Elements | Resize Count |
|----------|-------------|
| 1,000 | ~10 |
| 10,000 | ~14 |
| 100,000 | ~17 |
| 1,000,000 | ~20 |
| 10,000,000 | ~24 |

> Resize count grows as log₂(N) — confirming amortized O(1) add. At 10 million elements, the array only resizes ~24 times total.

---

## FIFO Queue: Singly Linked List

```
Elements      Enqueue Time (s)   Dequeue Time (s)   Total (s)
--------------------------------------------------------------
1,000         ~7.05e-05          ~1.32e-05          ~8.37e-05
10,000        ~0.0003012         ~8.16e-05          ~0.0003828
100,000       ~0.0028018         ~0.001054          ~0.0038558
1,000,000     ~0.0262555         ~0.0106014         ~0.0368569
10,000,000    ~4.6761            ~16.0114           ~20.6875
```

> SLList is significantly slower than ArrayStack due to per-node heap allocation and pointer chasing. The Node Pooling twist reduces allocation overhead after warmup — freed nodes are recycled instead of deleted, making subsequent pushes nearly allocation-free.

---

## Priority Queue: Meldable Heap

```
Elements     Add Time (s)    Remove Time (s)   Total (s)    Add Merges     Remove Merges
----------------------------------------------------------------------------------------
1,000        ~0.0001402      ~0.0001563        ~0.0002965   6,563          9,094
10,000       ~0.001421       ~0.0023863        ~0.0038073   85,243         135,748
100,000      ~0.0176354      ~0.0347571        ~0.0523925   1,106,468      1,818,090
1,000,000    ~0.224961       ~0.84833          ~1.07329     13,092,266     22,754,210
10,000,000   ~5.88856        ~20.347           ~26.2356     149,584,776    270,311,261
```

### Merge Counter — Empirical O(log n) Proof

| Elements | Add Merges | Remove Merges | Add Merges / Element | Remove Merges / Element | log₂(N) |
|----------|-----------|---------------|---------------------|------------------------|---------|
| 1,000 | 6,563 | 9,094 | 6.56 | 9.09 | 10.0 |
| 10,000 | 85,243 | 135,748 | 8.52 | 13.57 | 13.3 |
| 100,000 | 1,106,468 | 1,818,090 | 11.06 | 18.18 | 16.6 |
| 1,000,000 | 13,092,266 | 22,754,210 | 13.09 | 22.75 | 19.9 |
| 10,000,000 | 149,584,776 | 270,311,261 | 14.96 | 27.03 | 23.3 |

> Add merges per element grow as **~0.65 × log₂(N)** across all tested scales. Remove merges per element track closer to log₂(N) — slightly below at small N and slightly above at large N. This is because `remove()` merges two existing subtrees rather than a single new node against the root, resulting in deeper average traversal. Both are consistent with expected **O(log n)** behavior.

### Absorb Benchmark

Merging two heaps of N elements each:

```
Elements      Absorb Time (s)
------------------------------
2,000         ~4e-07
20,000        ~6e-07
200,000       ~1.6e-06
2,000,000     ~3.8e-06
```

> Absorbing 2 million elements takes **~3.8 microseconds**. From 2,000 to 2,000,000 elements — a 1000× increase in data — absorb time grows only ~10×, consistent with O(log n). This is the defining feature of a meldable heap: merging two massive heaps is essentially instant regardless of size. `std::priority_queue` has no equivalent — merging two STL heaps requires re-inserting every element at O(n log n) cost.

---

## Custom-made vs Standard Template Library (STL)

Benchmarked against C++ Standard Library equivalents.
Fixed random seed (42) for reproducibility across all runs.

### ArrayStack vs `std::stack`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 1.36e-05 | 1.4e-06 | 6.5e-06 | 8e-07 | STL +813% |
| 10,000 | 7.76e-05 | 2.49e-05 | 1.306e-04 | 9.6e-06 | STL +503% |
| 100,000 | 3.359e-04 | 2.332e-04 | 3.182e-04 | 7.89e-05 | STL +109% |
| 1,000,000 | 2.767e-03 | 2.515e-03 | 2.879e-03 | 8.877e-04 | STL +65% |

> The gap **narrows significantly** as N grows — from 813% slower at 1K to only 65% slower at 1M. This is the amortized cost of our resize strategy evening out. `std::stack` wraps `std::deque` which uses a chunked memory layout, giving it better cache behavior than our single contiguous array at small sizes.

---

### SLList vs `std::queue`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 3.45e-05 | 1.6e-06 | 7.6e-06 | 5e-07 | STL +1904% |
| 10,000 | 2.594e-04 | 1.66e-05 | 9.45e-05 | 4.5e-06 | STL +1577% |
| 100,000 | 2.761e-03 | 3.871e-04 | 1.230e-03 | 1.114e-04 | STL +700% |
| 1,000,000 | 2.745e-02 | 2.069e-03 | 1.331e-02 | 8.038e-04 | STL +1318% |

> This is the largest gap and the most expected result. `std::queue` wraps `std::deque` — a chunked array structure that avoids per-element heap allocation entirely. Our SLList calls `new`/`delete` (or pool recycle) on every node, which is fundamentally slower due to pointer chasing and cache misses. This is the classic linked list vs array-backed structure tradeoff — same O(1) complexity, vastly different constant factors.

---

### MeldableHeap vs `std::priority_queue`

| Elements | Custom Push (s) | STL Push (s) | Custom Pop (s) | STL Pop (s) | Winner |
|----------|----------------|-------------|----------------|------------|--------|
| 1,000 | 1.301e-04 | 7.77e-05 | 1.587e-04 | 4.06e-05 | STL +144% |
| 10,000 | 1.289e-03 | 1.847e-04 | 2.508e-03 | 5.38e-04 | STL +425% |
| 100,000 | 1.843e-02 | 1.395e-03 | 3.785e-02 | 6.734e-03 | STL +592% |
| 1,000,000 | 2.085e-01 | 1.171e-02 | 7.633e-01 | 8.527e-02 | STL +902% |

> `std::priority_queue` uses a binary heap on a contiguous `std::vector` — every operation benefits from CPU cache prefetching. Our MeldableHeap uses pointer-based tree nodes scattered in memory, making every merge step a potential cache miss. However, `std::priority_queue` **cannot merge two heaps** — there is no `absorb()` equivalent in the STL. Our MeldableHeap does it in **O(log n) — ~3.8 microseconds for 2 million elements**.

---

## Summary

### Performance vs STL

| Structure | vs STL | Why STL wins | What we have that STL doesn't |
|-----------|--------|-------------|-------------------------------|
| ArrayStack | STL +65–813% | `std::deque` chunked layout | Ghost Scrubbing, Boundary Guarding, Resize Telemetry |
| SLList | STL +700–1904% | No per-node allocation | Node Pooling, Self-Healing cycle detection |
| MeldableHeap | STL +144–902% | Contiguous array cache locality | `absorb()` — O(log n) heap merge |

### Complexity Reference

| Structure | Add | Remove | Merge | Special |
|-----------|-----|--------|-------|---------|
| ArrayStack | O(1) amortized | O(1) amortized | — | O(n) worst case resize |
| SLList | O(1) | O(1) | — | O(n) get(i) |
| MeldableHeap | O(log n) | O(log n) | O(log n) | absorb() in O(log n) |
| `std::stack` | O(1) amortized | O(1) | — | — |
| `std::queue` | O(1) amortized | O(1) | — | — |
| `std::priority_queue` | O(log n) | O(log n) | O(n log n) | no merge |

> STL wins on raw speed in all cases — this is expected. STL containers are engineered by the C++ standards committee and optimized over decades. The value of these custom implementations lies not in beating STL, but in understanding *why* the gap exists, and offering capabilities that STL simply does not provide.

---

<div align="center">

[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

*Data Structures in C++ — Group C*
[← Back to README](./README.md)

</div>
