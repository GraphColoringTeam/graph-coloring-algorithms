#ifndef CP_SOLVER_H
#define CP_SOLVER_H

#include "../../graph_representation/Graph.h"
#include <vector>

using namespace std;

namespace Algorithms {
namespace CP {

class CPSolver {
private:
    vector<vector<int>> domains;
    bool forwardCheck(const Graph& graph, int node, int color, vector<int>& coloring);
    bool solveRecursive(const Graph& graph, vector<int>& coloring, int nodeCount);

public:
    int solve(const Graph& graph, vector<int>& coloring);

    // 🔹 Thêm hàm in kết quả
    void printColoring(const vector<int>& colors) const;
};

} // namespace CP
} // namespace Algorithms

#endif // CP_SOLVER_H