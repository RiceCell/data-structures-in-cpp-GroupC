#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <iomanip>
#include "AdjacencyMatrix.h"
#include "arraystack.h"

// Helper Timer
class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) { }

    double elapsed() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

// ---------------------------------------------------------

// Main Benchmark Routine
void runBenchmark(size_t numVertices, size_t numRuns) {
    size_t numEdgesToAdd = numVertices * 20;

    // Querying up to 1000 nodes, or all of them if the graph is smaller
    size_t numNodeQueries = std::min<size_t>(numVertices, 1000);

    std::cout << "====================================================\n";
    std::cout << "Vertices: " << numVertices << " | Edges: " << numEdgesToAdd << " | Node Queries: " << numNodeQueries << " | Runs: " << numRuns << "\n";
    std::cout << "====================================================\n";

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist(0, numVertices - 1);

    std::vector<std::pair<size_t, size_t>> edgesToAdd(numEdgesToAdd);
    std::vector<size_t> nodesToQuery(numNodeQueries);

    for (auto& e : edgesToAdd) {
        e = { dist(rng), dist(rng) };
    }
    for (auto& n : nodesToQuery) {
        n = dist(rng);
    }

    // --- Adjacency Matrix Benchmark ---
    double tAdd = 0, tOutDeg = 0, tInDeg = 0, tOutEdg = 0, tInEdg = 0;

    for (size_t run = 0; run < numRuns; ++run) {
        AdjacencyMatrix matrixGraph(numVertices);

        Timer addT;
        for (const auto& e : edgesToAdd) {
            matrixGraph.addEdge(e.first, e.second);
        }
        tAdd += addT.elapsed();

        Timer outDegT;
        for (size_t n : nodesToQuery) {
            volatile size_t d = matrixGraph.outDegreeOf(n);
        }
        tOutDeg += outDegT.elapsed();

        Timer inDegT;
        for (size_t n : nodesToQuery) {
            volatile size_t d = matrixGraph.inDegreeOf(n);
        }
        tInDeg += inDegT.elapsed();

        Timer outEdgT;
        for (size_t n : nodesToQuery) {
            ArrayStack<size_t> edges;
            matrixGraph.outEdges(n, edges);
        }
        tOutEdg += outEdgT.elapsed();

        Timer inEdgT;
        for (size_t n : nodesToQuery) {
            ArrayStack<size_t> edges;
            matrixGraph.inEdges(n, edges);
        }
        tInEdg += inEdgT.elapsed();
    }

    std::cout << "  Avg Add Edges:   " << std::fixed << std::setprecision(2) << (tAdd / numRuns) << " ms\n";
    std::cout << "  Avg outDegreeOf: " << (tOutDeg / numRuns) << " ms\n";
    std::cout << "  Avg inDegreeOf:  " << (tInDeg / numRuns) << " ms\n";
    std::cout << "  Avg outEdges:    " << (tOutEdg / numRuns) << " ms\n";
    std::cout << "  Avg inEdges:     " << (tInEdg / numRuns) << " ms\n\n";
}

int main() {
    std::vector<size_t> sizes = { 100, 500, 1000, 2500, 5000 };
    const size_t NUM_RUNS = 5;

    for (size_t size : sizes) {
        runBenchmark(size, NUM_RUNS);
    }

    return 0;
}