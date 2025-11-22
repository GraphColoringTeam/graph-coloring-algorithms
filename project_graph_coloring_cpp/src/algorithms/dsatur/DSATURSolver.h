#ifndef DSATUR_SOLVER_H
#define DSATUR_SOLVER_H

#include "../../graph_representation/Graph.h"
#include <vector>
#include <set>
#include <iostream>  // thêm để cout

using namespace std;

namespace Algorithms {
namespace DSATUR {

class DSATURSolver {
private:
    int calculateSaturationDegree(int vertex, const Graph& graph, const vector<int>& colors) const;

public:
    DSATURSolver();

    int solve(const Graph& graph, vector<int>& colors);

    // 🔹 Thêm hàm in kết quả
    void printColoring(const vector<int>& colors) const;
};

} // namespace DSATUR
} // namespace Algorithms

#endif // DSATUR_SOLVER_H
