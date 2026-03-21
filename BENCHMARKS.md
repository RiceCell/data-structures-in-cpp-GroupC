<div align="left">

# Benchmark Results

**Data Structures in C++ — Group C**
CMSC 123: Data Structures and Algorithms 

> **Compiler flags:** `-O2` across all benchmarks
> **Methodology:** 10 iterations per N, results averaged. Random inputs use fixed seed for reproducibility.
>
> **Russel Niño Buno** — Acer Nitro AN515-58, i5-12500H, 8GB RAM, Windows 11, MSYS2/MinGW64, g++ 13.1 <br>
> **Angelo Mari Manlangit** — Lenovo Thinkpad T14 Gen 3, AMD Ryzen 5 PRO 5650U with Radeon Graphics, 16GB RAM, Windows 11, MSYS2/MinGW64, g++ 13.1  <br>
> **Gian Jefferson Reyes** — Extensa 215-55, i5-1235U, 8GB RAM, Windows 11
</div>

---

## Table of Contents

1. [➤ FILO Queue: Array Stack](#-filo-queue-array-stack)
2. [➤ FIFO Queue: Singly Linked List](#-fifo-queue-singly-linked-list)
3. [➤ Priority Queue: Meldable Heap](#-priority-queue-meldable-heap)
5. [➤ Deque: Array Deque](#deque-array-deque)
6. [➤ List: Doubly-Linked List](#list-doubly-linked-list)
7. [➤ Sorted Set: Skiplist](#sorted-set-skiplist)
8. [➤ Sorted Set: Red-Black Trees](#sorted-set-red-black-trees)
9. [➤ Graph: Adjacency Matrix](#graph-adjacency-matrix)
9. [➤ Custom vs STL](#-custom-vs-standard-template-library-stl)
10. [➤ Summary](#-summary)

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

## Deque: Array Deque
**Regular Mode** - average time of adding and remove 1 to N values

```
Elements    Avg Add Time (s)   Avg Remove Time (s)   Avg Total (s)
------------------------------------------------------------------
1000        1.663e-05          1.419e-05             3.082e-05        
10000       0.00019187         0.0001851             0.00037697     
100000      0.00148547         0.00095102            0.00243649      
1000000     0.0121042          0.00761675            0.019721      
100000000   1.30462            0.830724              2.13534        
```

**Random Mode** - adding, searching, and removing N items with random values
```
SEED: 20
NUMBER TO FIND: 20

Elements    Add Time (s)   Check Time (s)   Found Number (s)   Remove Time (s)   Total (s)
------------------------------------------------------------------------------------------
1000        ~4.98e-05      ~3.1e-06         true               ~1.55e-05         ~6.84e-05       
10000       ~0.0002646     ~4.8e-06         true               ~0.0001317        ~0.0004011      
100000      ~0.0029728     ~5.6e-06         true               ~0.0011686        ~0.004147       
1000000     ~0.0259146     ~9e-07           true               ~0.0097084        ~0.0356239      
100000000   ~2.89581       ~9e-07           true               ~1.09112          ~3.98693       
```

> The Array Deque has a really quick add and removal time compared to other Array-based Data Structures as the addition and removal of values are only accessible to the first or last index, as the Deque is Double Ended. This makes the processes in the array significantly quicker since it only concerns itself with the boundaries of the deque. Surprisingly, being able to find a certain value is relatively quick. Although, this might also have to do with the position in which the number to find is in so the quick time could indicate that that number is near the earlier indexes. Further testings need to be done in order to correctly make a statement about it.

---

## List: Doubly-Linked List

**Regular Mode** - average time of pushing and popping 1 to N values

```
Elements    Avg Enqueue Time (s)   Avg Dequeue Time (s)   Avg Total (s)
-----------------------------------------------------------------------
1000        1.262e-05              5.23e-06               1.785e-05 
10000       0.0001157              6.529e-05              0.00018099     
100000      0.00149918             0.00061647             0.00211565    
1000000     0.0148384              0.0167264              0.0315648   
100000000   1.76502                1.97526                3.74028 
```

**Traversal Mode** - pushing, moving forwards and backwards N times, and removal
```
Elements    Enqueue Time (s)   Moving Forward (s)   Moving Backward (s)   Dequeue Time (s)   Total (s)
-------------------------------------------------------------------------------------------------------
1000        ~9.48e-05          ~4.6e-06             ~3.4e-06              ~2.27e-05          ~0.0001255      
10000       ~0.0006355         ~4.83e-05            ~3.9e-05              ~0.0003065         ~0.0010293      
100000      ~0.006522          ~0.000773            ~0.0004379            ~0.0014615         ~0.0091944      
1000000     ~0.0657628         ~0.0115529           ~0.0090122            ~0.0170636         ~0.103391      
100000000   ~6.63476           ~1.05611             ~1.10625              ~2.46785           ~11.265
```

> The Doubly-Linked List have similar times to its Singly-Linked counterpart. However, you may notice that the average enqueue time is **1.77 secs** in the Regular Mode but takes **~6.63 secs** in the Traversal Mode. The time difference between the two are incredibly huge. Behind the scenes, it actually took **~7 secs** for the Enqueue time in the first iteration. However, due to its Node Pooling feature, it was able to save Enqueue time by recycling previously removed nodes as in the benchmark, the values of each iteration to get the average times are the same throughout (from 0 to N - 1).

---

## Sorted Set: Skiplist

**Regular Mode** - average time of adding and remove 1 to N values

```
Elements    Avg Add Time (s)   Avg Remove Time (s)   Avg Total (s)
------------------------------------------------------------------
1000        0.00028755         8.46e-05              0.00037215
10000       0.00340626         0.00097727            0.00438353
100000      0.070961           0.0118663             0.0828273
1000000     8.9263             0.125442              9.05174
```

**Randomizer Mode** - adding, searching, and removing N items with random values 
```
SEED: 20
NUMBER TO FIND: 20

Elements    Add Time (s)   Find Time (s)   Found?  Find Count  Total (s)
-------------------------------------------------------------------------
1000        ~0.0004004     ~2e-07          true    3           ~0.0004006
10000       ~0.0047681     ~4e-07          true    4           ~0.0047685
100000      ~0.10455       ~4e-07          true    3           ~0.104551
1000000     ~6.40796       ~1.8e-06        true    5           ~6.40797
```

> Skiplists' key feature is in its searching process, making it faster to go through multiple nodes via the express edges in the higher levels. This is most evident within the `find()` function as the it is consistent with the O(log n) complexity. For example, at N = 1000, log(1000) would be = **3**, which is the same value as the *Find Count*.

---

## Sorted Set: Red-Black Trees

**Regular Mode** - average time of adding and remove 1 to N values
```
Elements    Avg Add Time (s)   Avg Remove Time (s)   Avg Total (s)
------------------------------------------------------------------
1000        0.00012862         0.00038751            0.00051613
10000       0.00213398         0.0897772             0.0919112
100000      0.0203826          17.3044               17.3248
```
*Note: 1 million elements was removed because it took way too long to finish the remove ;_;*

**Randomizer Mode** - adding, searching, and removing N items with random values
```
SEED: 20
NUMBER TO FIND: 20

Elements    Add Time (s)   Find Time (s)   Found?  Find Count  Total (s)
-------------------------------------------------------------------------
1000        ~0.0001942     ~2e-07          true    10          ~0.0001944
10000       ~0.0021392     ~4e-07          true    13          ~0.0021396
100000      ~0.0340155     ~3e-07          true    10          ~0.0340158
1000000     ~0.87734       ~1e-06          true    20          ~0.877341
```

**Special Mode** - displays the leaves and their respective colors of the red-black tree after addding random items
```
SEED: 20
MAX VALUE (N): 1000

LEAVES:
35(RED) 588(BLACK) 622(RED) 640(RED) 816(BLACK) 888(BLACK) 892(BLACK) 898(RED) 953(RED) 955(BLACK)
```

> Red-Black Trees when compared to its Sorted Set brother, Skiplists, it has a faster time in adding elements but slower in removing elements. This is because of how much procedures goes into balancing the trees when removing compared to adding elements. It also takes longer to find a certain node in higher elements as it has to go through multiple nodes and compare its value with what to find and choose whether to go left or right and repeats that process until the node or NIL is found. However, it is much more organized and simpler to implement a display function that prints the leaves than a skiplist.

---

## Graph: Adjacency Matrix
```
Vertices  Add Edges (ms)  outDegree (ms)  inDegree (ms)  outEdges (ms)  inEdges (ms)
------------------------------------------------------------------------------------
100       ~0.01           ~0.12           ~0.14          ~0.27          ~0.27
500       ~0.12           ~2.28           ~2.28          ~3.32          ~4.34
1000      ~0.26           ~9.86           ~22.19         ~12.21         ~21.31
2500      ~0.83           ~26.24          ~81.81         ~29.96         ~81.29
5000      ~6.10           ~53.50          ~184.59        ~56.70         ~184.94
```
> This Adjacency Matrix's key performance characteristic lies in its memory access patterns. This is most evident when comparing the "out" operations (outDegreeOf, outEdges) against the "in" operations (inDegreeOf, inEdges) as the graph scales. Because the 1D edgeMatrix vector maps 2D coordinates using index = i * numOfVertices + j, reading outgoing edges iterates through contiguous memory blocks. However, reading incoming edges via hasEdge(j, i) forces the CPU to jump forward by numOfVertices bytes on every single loop iteration, resulting in severe cache misses. For example, at N = 5000, inDegreeOf (~184.59 ms) is nearly 3.5x slower than outDegreeOf (~53.50 ms) despite both performing the exact same number of operations.

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

### `ChainedHashTable` vs `std::unsorted_set`
| Elements  | Custom Add (ms) | STL Add (ms) | Custom Find (ms) | STL Find (ms) | Search Winner |
|-----------|-----------------|--------------|------------------|---------------|---------------|
| 1,000     | 0.705           | 0.611        | 0.156            | 0.819         | Custom +425%  |
| 10,000    | 8.518           | 7.354        | 2.257            | 8.586         | Custom +280%  |
| 100,000   | 81.685          | 71.411       | 32.787           | 90.347        | Custom +175%  |
| 1,000,000 | 795.981         | 794.050      | 480.257          | 1080.212      | Custom +125%  |
> `std::unordered_set` uses an identity hash for integers (where f(x) = x) and relies heavily on linked-node memory pooling, giving it a slight edge in insertion speed at smaller sizes. However, our `ChainedHashTable` dominates in search operations (up to 5x faster). This is because we implemented a robust `FastAvalancheHash` (utilizing a **MurmurHash3** 64-bit mix), which drastically reduces collision rates compared to the STL's default integer hash.
> Note on `isSubsetOf`: The STL benchmark appears nearly instantaneous (~0.003 ms) because the external standard library helper function uses an early return (short-circuiting) the moment it finds a missing element. Conversely, our `ChainedHashTable::isSubsetOf` uses a `forEach` lambda loop that fully iterates through the entire underlying array stack, even after the subset condition has already failed. Implementing an early break in our custom iteration would close this artificial gap.

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
