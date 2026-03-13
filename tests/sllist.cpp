// RUSSEL'S - Multi-Scale Benchmark

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "../src/sllist.h"

void run_benchmark(long long N)
{
    SLList<int> queue;

    // Time ENQUEUE (push)
    auto start_push = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        queue.push(i);
    }
    auto end_push = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_push = end_push - start_push;

    // Time DEQUEUE (pop)
    auto start_pop = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        queue.pop();
    }
    auto end_pop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_pop = end_pop - start_pop;

    std::cout << std::setw(12) << N
              << std::setw(15) << diff_push.count()
              << std::setw(15) << diff_pop.count()
              << std::setw(15) << (diff_push.count() + diff_pop.count()) << "seconds" << std::endl;
}

int main()
{
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 100000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000 - 100,000,000

    std::cout << std::left << std::setw(12) << "Elements"
              << std::setw(15) << "Enqueue Time"
              << std::setw(15) << "Dequeue Time"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(57, '-') << std::endl;

    for (long long n : sizes)
    {
        run_benchmark(n);
    }

    return 0;
}