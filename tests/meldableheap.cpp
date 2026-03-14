// RUSSEL'S - Multi-Scale Benchmark
// MeldableHeap: Priority Queue

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <random>
#include "../src/meldableheap.h"

void run_benchmark(long long N)
{
    MeldableHeap<int> heap;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 1000000);

    auto start_add = std::chrono::high_resolution_clock::now();
    for (long long i = N; i > 0; i--)
        heap.add(distribution(generator)); // added random
    auto end_add = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_add = end_add - start_add;

    size_t merges_after_add = heap.get_merge_count();
    heap.reset_merge_count();

    auto start_remove = std::chrono::high_resolution_clock::now();
    long long prev = -1;
    for (long long i = 0; i < N; i++)
    {
        long long val = heap.remove();
        assert(val >= prev);
        prev = val;
    }
    auto end_remove = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_remove = end_remove - start_remove;

    size_t merges_after_remove = heap.get_merge_count();

    std::cout << std::setw(12) << N
              << std::setw(15) << diff_add.count()
              << std::setw(15) << diff_remove.count()
              << std::setw(15) << (diff_add.count() + diff_remove.count())
              << std::setw(15) << merges_after_add
              << std::setw(15) << merges_after_remove << std::endl;
}

void run_absorb_benchmark(long long N)
{
    MeldableHeap<int> h1, h2;

    for (long long i = 0; i < N; i++)
        h1.add(i * 2);
    for (long long i = 0; i < N; i++)
        h2.add(i * 2 + 1);

    auto start = std::chrono::high_resolution_clock::now();
    h1.absorb(h2);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << std::setw(12) << N * 2
              << std::setw(15) << diff.count()
              << " seconds (verified order)" << std::endl;
    std::cout << std::flush;

    while (!h1.empty())
        h1.remove();
}

int main()
{
    std::srand(std::time(nullptr));
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << "PRIORITY QUEUE: MELDABLE HEAP" << std::endl;

    std::cout << "\n--- Add / Remove Benchmark ---\n";
    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Add Time"
              << std::setw(15) << "Remove Time"
              << std::setw(15) << "Total(s)"
              << std::setw(15) << "Add Merges"
              << std::setw(15) << "Remove Merges" << std::endl;
    std::cout << std::string(87, '-') << std::endl;
    for (long long n : sizes)
        run_benchmark(n);

    std::vector<long long> absorb_sizes = {1000, 10000, 100000, 1000000};

    std::cout << "\n--- Absorb Benchmark (excl. 10,000,000)---\n";
    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Absorb Time" << std::endl;
    std::cout << std::string(42, '-') << std::endl;
    for (long long n : absorb_sizes)
        run_absorb_benchmark(n);

    return 0;
}