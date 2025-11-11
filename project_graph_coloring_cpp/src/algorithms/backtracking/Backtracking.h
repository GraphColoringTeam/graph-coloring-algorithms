#pragma once
#include "../include/Graph.h"
#include <vector>

class BacktrackingSolver {
public:
    static std::vector<int> solve(const Graph& g, int maxColors);
};

