/* Angelo's interactive benchmarking :0 (from Russel's benchmarker)

WHAT TO DO:
1. Run the benchmark
2. User inputs a numerical seed and number to find
3. Deque autofills with random integers in each index: [0-1000]
4. Times the adding, searching, removal of items
Note: adding values will simultaneously add in front and back
5. Outputs those values
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "../src/arraydeque.h"

void run_benchmark(long long N, int containInput) {
    ArrayDeque<int> deque;

    // Time ADD
    auto start_add = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        if (i % 2 == 0)
            deque.addFirst(std::rand() % 1001);
        else
            deque.addLast(std::rand() % 1001);
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_add = end_add - start_add;

    // Time CONTAINS
    auto start_contain = std::chrono::high_resolution_clock::now();
    deque.contains(containInput);
    auto end_contain = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_contain = end_contain - start_contain;

    // Time REMOVE, N/2 because it removes first and last in each iteration
    auto start_remove = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < (N / 2); i++)
    {
            deque.removeFirst();
            deque.removeLast();
    }
    auto end_remove = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_remove = end_remove - start_remove;

    std::cout << std::setw(12) << N
              << std::setw(15) << diff_add.count()
              << std::setw(15) << diff_contain.count()
              << std::setw(15) << diff_remove.count()
              << std::setw(15) << (diff_add.count() + diff_contain.count() + diff_remove.count()) << "seconds" << std::endl;
}

int main() {
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000, 100000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000 - 100,000,000

    
    unsigned int seed, number; // for randomizing values
    std::cout << "ARRAY DEQUE BENCHMARKING\n";
    std::cout << "ENTER NUMERICAL RANDOMIZER SEED: ";
    std::cin  >> seed;

    std::srand(seed);

    std::cout << "ENTER NUMBER TO FIND [0-1000]: ";
    std::cin  >> number;

    std::cout << "\nDOUBLE ENDED QUEUE: ARRAY DEQUE" << std::endl;

    std::cout << std::left << std::setw(12) << "Elements"
              << std::setw(15) << "Add Time"
              << std::setw(15) << "Check Time"
              << std::setw(15) << "Remove Time"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(72, '-') << std::endl; 

    for (long long n : sizes)
    {
        run_benchmark(n, number);
    }

    return 0;
}