#include <iostream>
#include <chrono>
#include "../include/Graph.h"
#include "CP_Solver.h"
#include "Backtracking.h"

int main() {
    Graph g = Graph::loadFromFile("data/graph_small.txt");
    int maxColors = 4;

    // --- Backtracking ---
    auto t1 = std::chrono::high_resolution_clock::now();
    auto colorsBT = BacktrackingSolver::solve(g, maxColors);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto timeBT = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    // --- CP Solver ---
    auto t3 = std::chrono::high_resolution_clock::now();
    auto colorsCP = CPSolver::solve(g, maxColors);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto timeCP = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();

    // --- In kết quả ---
    std::cout << "=== Comparison (BT vs CP) ===\n";
    for (int i = 0; i < g.numVertices(); ++i)
        std::cout << "Vertex " << i << ": BT=" << colorsBT[i]
                  << ", CP=" << colorsCP[i] << "\n";

    std::cout << "\nTime Backtracking: " << timeBT << " ms\n";
    std::cout << "Time CP Solver: " << timeCP << " ms\n";
}
