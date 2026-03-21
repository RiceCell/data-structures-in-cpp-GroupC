#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "../src/arraystack.h"

const int RUNS = 10;

void run_benchmark(long long N)
{
    double total_add = 0.0;
    double total_remove = 0.0;

    for (int run = 0; run < RUNS; run++)
    {
        ArrayStack<int> stack;

        // Time ADD
        auto start_add = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < N; i++)
            stack.add(stack.size(), i);
        auto end_add = std::chrono::high_resolution_clock::now();

        // Time REMOVE
        auto start_remove = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < N; i++)
            stack.remove(stack.size() - 1);
        auto end_remove = std::chrono::high_resolution_clock::now();

        total_add += std::chrono::duration<double>(end_add - start_add).count();
        total_remove += std::chrono::duration<double>(end_remove - start_remove).count();
    }

    double avg_add = total_add / RUNS;
    double avg_remove = total_remove / RUNS;
    double avg_total = avg_add + avg_remove;

    std::cout << std::setw(12) << N
              << std::setw(15) << avg_add
              << std::setw(15) << avg_remove
              << std::setw(15) << avg_total << "seconds" << std::endl;
}

int main()
{
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 100000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000 - 100,000,000

    std::cout << "FILO QUEUE: ARRAY STACK" << std::endl;
    std::cout << " " << std::endl;

    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Avg Add"
              << std::setw(15) << "Avg Remove"
              << std::setw(15) << "Avg Total" << std::endl;
    std::cout << std::string(57, '-') << std::endl;

    for (long long n : sizes)
        run_benchmark(n);

    return 0;
}
