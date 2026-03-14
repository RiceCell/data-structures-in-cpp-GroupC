// STL Comparison Benchmark
// Compares custom implementations against C++ Standard Template Libraries (STL) equivalents

#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <stack>
#include <queue>
#include <random>
#include "../src/arraystack.h"
#include "../src/sllist.h"
#include "../src/meldableheap.h"

std::vector<int> generate_random(long long N)
{
    std::default_random_engine gen(67); // fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(1, 1000000);
    std::vector<int> data(N);
    for (auto &x : data)
        x = dist(gen);
    return data;
}

void print_header(const std::string &title)
{
    std::cout << "\n"
              << std::string(72, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(72, '=') << "\n";
    std::cout << std::left
              << std::setw(16) << "Elements"
              << std::setw(20) << "Custom Push (s)"
              << std::setw(20) << "STL Push (s)"
              << std::setw(16) << "Custom Pop (s)"
              << std::setw(16) << "STL Pop (s)"
              << std::setw(12) << "Winner"
              << std::endl;
    std::cout << std::string(100, '-') << "\n";
}

std::string winner(double custom_total, double stl_total)
{
    double diff = (stl_total - custom_total) / stl_total * 100.0;
    if (diff > 1.0)
        return "Custom +" + std::to_string((int)diff) + "%";
    else if (diff < -1.0)
        return "STL +" + std::to_string((int)-diff) + "%";
    else
        return "Tie";
}

// ARRAY STACK vs std::stack

void compare_stack(long long N)
{
    auto data = generate_random(N);

    // Custom ArrayStack
    ArrayStack<int> custom;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        custom.add(custom.size(), x);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        custom.remove(custom.size() - 1);
    auto t3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> custom_push = t2 - t1;
    std::chrono::duration<double> custom_pop = t3 - t2;

    // STL std::stack
    std::stack<int> stl;
    auto t4 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        stl.push(x);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        stl.pop();
    auto t6 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> stl_push = t5 - t4;
    std::chrono::duration<double> stl_pop = t6 - t5;

    double custom_total = custom_push.count() + custom_pop.count();
    double stl_total = stl_push.count() + stl_pop.count();

    std::cout << std::left
              << std::setw(16) << N
              << std::setw(20) << custom_push.count()
              << std::setw(20) << stl_push.count()
              << std::setw(16) << custom_pop.count()
              << std::setw(16) << stl_pop.count()
              << std::setw(12) << winner(custom_total, stl_total)
              << "\n";
}

// SLLIST vs std::queue

void compare_queue(long long N)
{
    auto data = generate_random(N);

    // Custom SLList
    SLList<int> custom;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        custom.push(x);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        custom.pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> custom_push = t2 - t1;
    std::chrono::duration<double> custom_pop = t3 - t2;

    // STL std::queue
    std::queue<int> stl;
    auto t4 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        stl.push(x);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        stl.pop();
    auto t6 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> stl_push = t5 - t4;
    std::chrono::duration<double> stl_pop = t6 - t5;

    double custom_total = custom_push.count() + custom_pop.count();
    double stl_total = stl_push.count() + stl_pop.count();

    std::cout << std::left
              << std::setw(16) << N
              << std::setw(20) << custom_push.count()
              << std::setw(20) << stl_push.count()
              << std::setw(16) << custom_pop.count()
              << std::setw(16) << stl_pop.count()
              << std::setw(12) << winner(custom_total, stl_total)
              << "\n";
}

// MELDABLEHEAP vs std::priority_queue

void compare_heap(long long N)
{
    auto data = generate_random(N);

    // Custom MeldableHeap
    MeldableHeap<int> custom;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        custom.add(x);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        custom.remove();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> custom_push = t2 - t1;
    std::chrono::duration<double> custom_pop = t3 - t2;

    // STL std::priority_queue (min-heap via greater<int>)
    std::priority_queue<int, std::vector<int>, std::greater<int>> stl;
    auto t4 = std::chrono::high_resolution_clock::now();
    for (int x : data)
        stl.push(x);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; i++)
        stl.pop();
    auto t6 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> stl_push = t5 - t4;
    std::chrono::duration<double> stl_pop = t6 - t5;

    double custom_total = custom_push.count() + custom_pop.count();
    double stl_total = stl_push.count() + stl_pop.count();

    std::cout << std::left
              << std::setw(16) << N
              << std::setw(20) << custom_push.count()
              << std::setw(20) << stl_push.count()
              << std::setw(16) << custom_pop.count()
              << std::setw(16) << stl_pop.count()
              << std::setw(12) << winner(custom_total, stl_total)
              << "\n";
}

int main()
{
    std::vector<long long> sizes = {1000, 10000, 100000, 1000000};

    std::cout << "\nSTL COMPARISON BENCHMARK\n";
    std::cout << "Custom implementations vs C++ Standard Library with fixed random seed.\n";

    print_header("ArrayStack vs std::stack");
    for (long long n : sizes)
        compare_stack(n);

    print_header("SLList vs std::queue");
    for (long long n : sizes)
        compare_queue(n);

    print_header("MeldableHeap vs std::priority_queue (min-heap)");
    for (long long n : sizes)
        compare_heap(n);

    return 0;
}