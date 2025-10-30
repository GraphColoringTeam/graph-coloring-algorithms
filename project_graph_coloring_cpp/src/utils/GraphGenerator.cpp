#include "GraphGenerator.h"
#include <random>

std::unique_ptr<Graph> generateRandomGraph(int V, int E) {
    if (V <= 0 || E < 0) return nullptr;
    
    auto g = std::make_unique<Graph>(V);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, V - 1); // Chọn đỉnh từ 0 đến V-1

    int edges_added = 0;
    while (edges_added < E) {
        int u = distrib(gen);
        int v = distrib(gen);

        // Đảm bảo không có vòng (u != v) và không có cạnh song song
        if (u != v && !g->isAdjacent(u, v)) {
            g->addEdge(u, v);
            edges_added++;
        }
    }
    return g;
}