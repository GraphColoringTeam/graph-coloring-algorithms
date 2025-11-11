#pragma once
#include "../include/Graph.h"
#include <vector>

class CPSolver {
public:
    static std::vector<int> solve(const Graph& g, int maxColors);
};
