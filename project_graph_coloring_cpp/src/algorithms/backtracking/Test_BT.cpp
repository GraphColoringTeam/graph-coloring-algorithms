#include <iostream>
#include <chrono>
#include "../include/Graph.h"
#include "Backtracking.h"

int main() {
    Graph g = Graph::loadFromFile("data/graph_small.txt");
    int maxColors = 4;

    auto start = std::chrono::high_resolution_clock::now();
    auto result = BacktrackingSolver::solve(g, maxColors);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "=== Backtracking Result ===\n";
    for (int i = 0; i < g.numVertices(); ++i)
        std::cout << "Vertex " << i << " -> Color " << result[i] << "\n";

    std::cout << "Execution time: " << duration << " ms\n";
}
