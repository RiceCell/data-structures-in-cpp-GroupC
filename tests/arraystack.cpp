// RUSSEL'S - Multi-Scale Benchmark

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "../src/arraystack.h"

void run_benchmark(long long N)
{
    ArrayStack<int> stack;

    // Time ADD
    auto start_add = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        stack.add(stack.size(), i);
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_add = end_add - start_add;

    // Time REMOVE
    auto start_remove = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        stack.remove(stack.size() - 1);
    }
    auto end_remove = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_remove = end_remove - start_remove;

    // Print results in a table format
    std::cout << std::setw(12) << N
              << std::setw(15) << diff_add.count()
              << std::setw(15) << diff_remove.count()
              << std::setw(15) << (diff_add.count() + diff_remove.count()) << "seconds" << std::endl;
}

int main()
{
    std::vector<long long> sizes = {10000, 100000, 1000000, 100000000, 1000000000};
    // 10,000 - 100,000 - 1,000,000 - 100,000,000 - 1,000,000,000

    std::cout << std::left << std::setw(12) << "Elements"
              << std::setw(15) << "Add Time"
              << std::setw(15) << "Remove Time"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(57, '-') << std::endl;

    for (long long n : sizes)
    {
        run_benchmark(n);
    }

    return 0;
}