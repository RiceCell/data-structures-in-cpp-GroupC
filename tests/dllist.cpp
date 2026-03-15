/* Angelo's interactive benchmarking :0 (from Russel's benchmarker)

WHAT TO DO:
1. Run the benchmark
2. User inputs whether they want to test times regularly or do traversal check
3. If traversal check, the list will move back and forth and time it
4. If regular, it will time the push and pop
5. Outputs those values
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include "../src/dllist.h"

void run_traversal_benchmark(long long N) {
    DLList<int> queue;

    // Time ENQUEUE (push)
    auto start_push = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        queue.push(i);
    }
    auto end_push = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_push = end_push - start_push;

    // Time TRAVERSAL FORWARDS
    auto start_move_forward = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        queue.moveNextNode();
    }
    auto end_move_forward = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_move_forward = end_move_forward - start_move_forward;

    // Time TRAVERSAL BACKWARDS
    auto start_move_backward = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        queue.movePreviousNode();
    }
    auto end_move_backward = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_move_backward = end_move_backward - start_move_backward;

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
              << std::setw(17) << diff_move_forward.count()
              << std::setw(17) << diff_move_backward.count()
              << std::setw(15) << diff_pop.count()
              << std::setw(15) << (diff_push.count() + diff_move_forward.count() + diff_move_backward.count() + diff_pop.count()) << "seconds" << std::endl;
}

void run_benchmark(long long N)
{
    DLList<int> queue;

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

int traversal_mode(std::vector<long long> sizes) {
    std::cout << std::left << std::setw(12) << "Elements"
              << std::setw(15) << "Enqueue Time"
              << std::setw(17) << "Moving Forward"
              << std::setw(17) << "Moving Backward"
              << std::setw(15) << "Dequeue Time"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(91, '-') << std::endl;

    for (long long n : sizes)
    {
        run_traversal_benchmark(n);
    }

    return 0;
}

int regular_mode(std::vector<long long> sizes) {
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

int main()
{
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 100000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000 - 100,000,000

    unsigned int benchmarkMode;
    std::cout << "DOUBLY LINKED LIST BENCHMARKING\n";
    std::cout << "RUN 0.REGULAR OR 1.TRAVERSAL?: ";
    std::cin  >> benchmarkMode;

    std::cout << "\nFIFO QUEUE: DOUBLY LINKED LIST" << std::endl;

    if (benchmarkMode) {
        return traversal_mode(sizes);
    }
    else {
        return regular_mode(sizes);
    }

    return 0;
}