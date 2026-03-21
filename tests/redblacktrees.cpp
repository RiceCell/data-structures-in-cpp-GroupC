/* Angelo's interactive benchmarking :0

WHAT TO DO:
1. Run the benchmark
2. User inputs whether they want to test times regularly or do a random search
3. If random search, User inputs a numerical seed and number to find
3.1. list autofills with random integers in each index: [0-1000]
4. If regular, it will run add first
5. Times the adding, searching, removal of items
Note: adding values will simultaneously add in front and back for random search
6. Outputs those values

Red-Black-Trees Special:
1. Make 10 random values
2. Display all the leaves
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <random>
#include "../src/redblacktrees.h"

// for regular benchmark times
void run_regular_benchmark(long long N) {
    // 1  mil is cancelled
    if (N == 1000000)
        return;
        
    RedBlackTree<int> list;

    // Time ADD
    auto start_add = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        list.insert(i);
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_add = end_add - start_add;

    // Time REMOVE
    auto start_remove = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        list.remove(i);
    }
    auto end_remove = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_remove = end_remove - start_remove;

    std::cout << std::setw(12) << N
              << std::setw(15) << diff_add.count()
              << std::setw(15) << diff_remove.count()
              << std::setw(15) << (diff_add.count() + diff_remove.count()) << "seconds" << std::endl;
}

// with searching and random values
void run_randomizer_benchmark(long long N, int findInput, unsigned int seed) {
    RedBlackTree<int> list;

    std::mt19937 gen(seed); 
    std::uniform_int_distribution<> dis(0, N);

    // Time ADD
    auto start_add = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
    {
        list.insert(dis(gen));
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_add = end_add - start_add;

    // Time FINDING
    auto start_find = std::chrono::high_resolution_clock::now();
    bool found = list.find(findInput);
    auto end_find = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_find = end_find - start_find;

    std::cout << std::setw(12) << N
              << std::setw(15) << diff_add.count()
              << std::setw(15) << diff_find.count()
              << std::setw(7)  << ((found) ? "true" : "false")
              << std::setw(12) << list.currentFindCount()
              << std::setw(15) << (diff_add.count() + diff_find.count()) << "seconds" << std::endl;
}

int regular_mode(std::vector<long long> sizes) {
    std::cout << "\nSORTED LIST: RED-BLACK TREES" << std::endl;

    std::cout << std::left << std::setw(12) << "Elements"
            << std::setw(15) << "Add Time"
            << std::setw(15) << "Remove Time"
            << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(57, '-') << std::endl; 

    for (long long n : sizes)
    {
        run_regular_benchmark(n);
    }

    return 0;
}

int randomizer_mode(std::vector<long long> sizes) {
    unsigned int seed, number; // for randomizing values
    std::cout << "ENTER NUMERICAL RANDOMIZER SEED: ";
    std::cin  >> seed;

    std::cout << "ENTER NUMBER TO FIND [0-1000]: ";
    std::cin  >> number;

    std::cout << "\nSORTED LIST: RED-BLACK TREES" << std::endl;

    std::cout << std::left << std::setw(12) << "Elements"
              << std::setw(15) << "Add Time"
              << std::setw(15) << "Find Time"
              << std::setw(7)  << "Found"
              << std::setw(12) << "Find Count"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(69, '-') << std::endl; 

    for (long long n : sizes)
    {
        run_randomizer_benchmark(n, number, seed);
    }

    return 0;
}

int special_mode() {
    unsigned int seed, number; // for randomizing values
    std::cout << "ENTER NUMERICAL RANDOMIZER SEED: ";
    std::cin  >> seed;

    std::cout << "ENTER MAX NUMBER (N) [0-N]: ";
    std::cin  >> number;

    RedBlackTree<int> list;

    std::mt19937 gen(seed); 
    std::uniform_int_distribution<> dis(0, number);

    for (int i = 0; i < 10; i++) {
        list.insert(dis(gen));
    }

    std::cout << "\nSORTED LIST: RED-BLACK TREES\n\n";
    std::cout << "LEAVES: \n";
    list.displayLeaves();
    std::cout << std::endl;
    
    return 0;
}

int main() {
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000};
    // 1,000 - 10,000 - 100,000 - 1,000,000

    unsigned int benchmarkMode;
    std::cout << "RED-BLACK TREE BENCHMARKING\n";
    std::cout << "RUN 0.REGULAR OR 1.RANDOMIZER OR 2.SPECIAL?: ";
    std::cin  >> benchmarkMode;

    // if choice == 0
    if (benchmarkMode == 0) {
        return regular_mode(sizes);
    }
    else if (benchmarkMode == 1) {
        return randomizer_mode(sizes);
    }
    else {
        return special_mode();
    }

    return 0;
}