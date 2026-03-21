// FIFO: Singly Linked List

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "../src/sllist.h"

const int RUNS = 10;

void run_benchmark(long long N)
{
    double total_push = 0.0;
    double total_pop = 0.0;

    for (int run = 0; run < RUNS; run++)
    {
        SLList<int> queue;

        // Time ENQUEUE (push)
        auto start_push = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < N; i++)
            queue.push(i);
        auto end_push = std::chrono::high_resolution_clock::now();

        // Time DEQUEUE (pop)
        auto start_pop = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < N; i++)
            queue.pop();
        auto end_pop = std::chrono::high_resolution_clock::now();

        total_push += std::chrono::duration<double>(end_push - start_push).count();
        total_pop += std::chrono::duration<double>(end_pop - start_pop).count();
    }

    double avg_push = total_push / RUNS;
    double avg_pop = total_pop / RUNS;
    double avg_total = avg_push + avg_pop;

    std::cout << std::setw(12) << N
              << std::setw(15) << avg_push
              << std::setw(15) << avg_pop
              << std::setw(15) << avg_total << "seconds" << std::endl;
}

int main()
{
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 100000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000 - 100,000,000

    std::cout << "FIFO QUEUE: SINGLY LINKED LIST" << std::endl;
    std::cout << " " << std::endl;

    std::cout << std::left
              << std::setw(12) << "Elements"
              << std::setw(15) << "Avg Enqueue"
              << std::setw(15) << "Avg Dequeue"
              << std::setw(15) << "Avg Total" << std::endl;
    std::cout << std::string(57, '-') << std::endl;

    for (long long n : sizes)
        run_benchmark(n);

    return 0;
}
