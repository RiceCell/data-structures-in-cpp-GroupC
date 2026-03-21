#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_set>
#include <iomanip>
#include "../src/ChainedHashTable.h"

// Helper Timer
struct Timer {
    std::chrono::high_resolution_clock::time_point startTime;

    Timer() { 
        reset(); 
    }
    void reset() { 
        startTime = std::chrono::high_resolution_clock::now(); 
    }
    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - startTime).count();
    }
};

// ---------------------------------------------------------

// Set Operation Helpers for std::unordered_set<T> (for fair comparison)
bool stdIsSubset(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    for (int x : a) {
        if (b.find(x) == b.end()) return false;
    }
    return true;
}

std::unordered_set<int> stdUnion(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> result = a;
    result.insert(b.begin(), b.end());
    return result;
}

std::unordered_set<int> stdIntersection(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> result;
    for (int x : a) {
        if (b.find(x) != b.end()) result.insert(x);
    }
    return result;
}

std::unordered_set<int> stdDifference(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> result;
    for (int x : a) {
        if (b.find(x) == b.end()) result.insert(x);
    }
    return result;
}

// ---------------------------------------------------------

// Main Benchmark Logic
int main() {
    std::vector<size_t> testSizes = { 1000, 10000, 100000, 1000000 };
    const int NUM_RUNS = 5; // Average results over 5 runs to smooth out noise

    std::cout << "Starting Benchmark...\n";
    std::cout << "Averaging over " << NUM_RUNS << " runs per size.\n\n";

    for (size_t N : testSizes) {
        std::cout << "====================================================================\n";
        std::cout << " SIZE: " << N << " Elements\n";
        std::cout << "====================================================================\n";
        std::cout << std::left << std::setw(20) << "Operation" << std::setw(25) << "| ChainedHashTable (ms)" << std::setw(25) << "| std::unordered_set (ms)" << "\n";
        std::cout << "--------------------------------------------------------------------\n";

        // Accumulators for averages
        double cAdd = 0, cFind = 0, cRemove = 0;
        double cSub = 0, cUni = 0, cInt = 0, cDiff = 0;
        size_t cHits = 0, cMisses = 0; // Trackers for items found

        double sAdd = 0, sFind = 0, sRemove = 0;
        double sSub = 0, sUni = 0, sInt = 0, sDiff = 0;
        size_t sHits = 0, sMisses = 0; // Trackers for items found

        for (int run = 0; run < NUM_RUNS; ++run) {
            // Generate deterministic data for Set A and Set B.
            std::vector<int> dataA, dataB, missData;
            std::mt19937 rng(42 + run); // Change seed per run
            std::uniform_int_distribution<int> dist(1, N * 10);

            // We make them overlap by 50% to properly test Intersection/Union/Difference.
            for (size_t i = 0; i < N; ++i) {
                dataA.push_back(dist(rng));
                if (i % 2 == 0) {
                    dataB.push_back(dataA.back()); // 50% overlap
                }
                else {
                    dataB.push_back(dist(rng));
                }

                missData.push_back(-dist(rng)); // Negative = guaranteed miss
            }

            Timer t;

            // --- Custom Data Structure Scope ---
            {
                ChainedHashTable<int> tableA, tableB;

                // ADD
                t.reset();
                for (int v : dataA) tableA.add(v);
                for (int v : dataB) tableB.add(v);
                cAdd += (t.elapsed_ms() / 2.0); // Average time to build one table

                // FIND (Hits + Misses)
                t.reset();
                for (int v : dataA) {
                    if (tableA.find(v).has_value()) cHits++;
                }
                for (int v : missData) {
                    if (!tableA.find(v).has_value()) cMisses++;
                }
                cFind += t.elapsed_ms();

                // SUBSET
                t.reset();
                tableA.isSubsetOf(tableB);
                cSub += t.elapsed_ms();

                // UNION
                t.reset();
                USet<int>* resUnion = tableA.unionWith(tableB);
                cUni += t.elapsed_ms();

                // INTERSECTION
                t.reset();
                USet<int>* resInter = tableA.intersectionWith(tableB);
                cInt += t.elapsed_ms();

                // DIFFERENCE
                t.reset();
                USet<int>* resDiff = tableA.differenceWith(tableB);
                cDiff += t.elapsed_ms();

                // REMOVE
                t.reset();
                for (int v : dataA) tableA.remove(v);
                cRemove += t.elapsed_ms();

                // Memory Cleanup (CRITICAL!)
                delete resUnion;
                delete resInter;
                delete resDiff;
            }

            // --- Standard Library Scope ---
            {
                std::unordered_set<int> stdA, stdB;

                // ADD
                t.reset();
                for (int v : dataA) stdA.insert(v);
                for (int v : dataB) stdB.insert(v);
                sAdd += (t.elapsed_ms() / 2.0);

                // FIND
                t.reset();
                for (int v : dataA) {
                    if (stdA.find(v) != stdA.end()) sHits++;
                }
                for (int v : missData) {
                    if (stdA.find(v) == stdA.end()) sMisses++;
                }
                sFind += t.elapsed_ms();

                // SUBSET
                t.reset();
                stdIsSubset(stdA, stdB);
                sSub += t.elapsed_ms();

                // UNION
                t.reset();
                auto stdResUni = stdUnion(stdA, stdB);
                sUni += t.elapsed_ms();

                // INTERSECTION
                t.reset();
                auto stdResInt = stdIntersection(stdA, stdB);
                sInt += t.elapsed_ms();

                // DIFFERENCE
                t.reset();
                auto stdResDiff = stdDifference(stdA, stdB);
                sDiff += t.elapsed_ms();

                // REMOVE
                t.reset();
                for (int v : dataA) stdA.erase(v);
                sRemove += t.elapsed_ms();
            }
        }

        // Print Average Results
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::left << std::setw(20) << "Add (Insert)" << "| " << std::setw(23) << (cAdd / NUM_RUNS) << "| " << (sAdd / NUM_RUNS) << "\n";

        // Find Section with Hit/Miss Data
        std::cout << std::left << std::setw(20) << "Find (Search) ms" << "| " << std::setw(23) << (cFind / NUM_RUNS) << "| " << (sFind / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << " -> Hits Found" << "| " << std::setw(23) << (cHits / NUM_RUNS) << "| " << (sHits / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << " -> Misses Verif." << "| " << std::setw(23) << (cMisses / NUM_RUNS) << "| " << (sMisses / NUM_RUNS) << "\n";

        std::cout << std::left << std::setw(20) << "Remove (Erase)" << "| " << std::setw(23) << (cRemove / NUM_RUNS) << "| " << (sRemove / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << "isSubsetOf" << "| " << std::setw(23) << (cSub / NUM_RUNS) << "| " << (sSub / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << "unionWith" << "| " << std::setw(23) << (cUni / NUM_RUNS) << "| " << (sUni / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << "intersection" << "| " << std::setw(23) << (cInt / NUM_RUNS) << "| " << (sInt / NUM_RUNS) << "\n";
        std::cout << std::left << std::setw(20) << "difference" << "| " << std::setw(23) << (cDiff / NUM_RUNS) << "| " << (sDiff / NUM_RUNS) << "\n";
        std::cout << "\n";
    }

    std::cout << "Benchmarking Complete!\n";
    return 0;
}
