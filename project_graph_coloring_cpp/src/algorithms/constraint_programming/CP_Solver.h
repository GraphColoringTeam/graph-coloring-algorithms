#pragma once
#include <vector>
#include "../../graph_representation/Graph.h"

class CPSolver {
public:
    static std::vector<int> solve(const Graph& graph, int maxColors);
};
