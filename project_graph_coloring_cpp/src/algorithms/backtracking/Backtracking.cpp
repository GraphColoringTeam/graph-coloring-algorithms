#include "Backtracking.h"
#include <iostream>

bool BacktrackingSolver::isSafe(int v, const Graph& graph, const std::vector<int>& colors, int c) {
    for (int u : graph.getAdj(v)) {
        if (colors[u] == c) return false;
    }
    return true;
}

bool BacktrackingSolver::graphColoringUtil(const Graph& graph, int m, std::vector<int>& colors, int v) {
    if (v == graph.numVertices()) return true; // tất cả đỉnh đã tô

    for (int c = 1; c <= m; ++c) {
        if (isSafe(v, graph, colors, c)) {
            colors[v] = c;
            if (graphColoringUtil(graph, m, colors, v + 1))
                return true;
            colors[v] = 0; // backtrack
        }
    }
    return false;
}

std::vector<int> BacktrackingSolver::solve(const Graph& graph, int maxColors) {
    std::vector<int> colors(graph.numVertices(), 0);
    graphColoringUtil(graph, maxColors, colors, 0);
    return colors;
}
