// MeldableHeap: Priority Queue
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <random>
#include "../src/meldableheap.h"

const int RUNS = 10;

void run_benchmark(long long N)
{
    double total_add = 0.0;
    double total_remove = 0.0;
    size_t total_merges_add = 0;
    size_t total_merges_remove = 0;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 1000000);

    for (int run = 0; run < RUNS; run++)
    {
        MeldableHeap<int> heap;

        auto start_add = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < N; i++)
            heap.add(distribution(generator));
        auto end_add = std::chrono::high_resolution_clock::now();

        total_merges_add += heap.get_merge_count();
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

        total_merges_remove += heap.get_merge_count();

        total_add += std::chrono::duration<double>(end_add - start_add).count();
        total_remove += std::chrono::duration<double>(end_remove - start_remove).count();
    }

    double avg_add = total_add / RUNS;
    double avg_remove = total_remove / RUNS;
    double avg_total = avg_add + avg_remove;
    size_t avg_merges_add = total_merges_add / RUNS;
    size_t avg_merges_remove = total_merges_remove / RUNS;

    std::cout << std::setw(12) << N
              << std::setw(15) << avg_add
              << std::setw(15) << avg_remove
              << std::setw(15) << avg_total
              << std::setw(15) << avg_merges_add
              << std::setw(15) << avg_merges_remove << std::endl;
}

void run_absorb_benchmark(long long N)
{
    double total_absorb = 0.0;

    for (int run = 0; run < RUNS; run++)
    {
        MeldableHeap<int> h1, h2;

        for (long long i = 0; i < N; i++)
            h1.add(i * 2);
        for (long long i = 0; i < N; i++)
            h2.add(i * 2 + 1);

        auto start = std::chrono::high_resolution_clock::now();
        h1.absorb(h2);
        auto end = std::chrono::high_resolution_clock::now();

        total_absorb += std::chrono::duration<double>(end - start).count();

        while (!h1.empty())
            h1.remove();
    }

    double avg_absorb = total_absorb / RUNS;

    std::cout << std::setw(12) << N * 2
              << std::setw(15) << avg_absorb
              << " " << std::endl;
    std::cout << std::flush;
}

int main()
{
    std::srand(std::time(nullptr));
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << "PRIORITY QUEUE: MELDABLE HEAP" << std::endl;
    std::cout << " " << std::endl;

    std::cout << "\n--- Add / Remove Benchmark ---\n";
    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Avg Add"
              << std::setw(15) << "Avg Remove"
              << std::setw(15) << "Avg Total"
              << std::setw(15) << "Avg AddMrg"
              << std::setw(15) << "Avg RmvMrg" << std::endl;
    std::cout << std::string(87, '-') << std::endl;
    for (long long n : sizes)
        run_benchmark(n);

    std::vector<long long> absorb_sizes = {1000, 10000, 100000, 1000000};

    std::cout << "\n--- Absorb Benchmark ---\n";
    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Avg Absorb" << std::endl;
    std::cout << std::string(42, '-') << std::endl;
    for (long long n : absorb_sizes)
        run_absorb_benchmark(n);

    return 0;
}
