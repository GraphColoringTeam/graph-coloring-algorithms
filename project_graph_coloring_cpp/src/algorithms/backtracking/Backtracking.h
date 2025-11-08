#pragma once
#include <vector>
#include "../../graph_representation/Graph.h"

class BacktrackingSolver {
public:
    // Trả về vector chứa màu của từng đỉnh
    static std::vector<int> solve(const Graph& graph, int maxColors);

private:
    static bool isSafe(int v, const Graph& graph, const std::vector<int>& colors, int c);
    static bool graphColoringUtil(const Graph& graph, int m, std::vector<int>& colors, int v);
};
