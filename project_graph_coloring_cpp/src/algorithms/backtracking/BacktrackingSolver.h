#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#include "../../graph_representation/Graph.h"
#include <vector>
#include <limits>

using namespace std;

namespace Algorithms {
namespace Backtracking {

class BacktrackingSolver {
private:
    int minColors; // Số màu tối thiểu tìm được
    vector<int> bestColors; // Cách tô màu tốt nhất

    // Hàm đệ quy Backtracking
    void solveRecursive(const Graph& graph, int k, int numVertices,
                        vector<int>& colors, int vertexIdx);

public:
    BacktrackingSolver();

    // Hàm giải bài toán tô màu đồ thị bằng Backtracking
    int solve(const Graph& graph, vector<int>& colors);

    void printColoring(const vector<int>& colors) const;
};


} // namespace Backtracking
} // namespace Algorithms

#endif // BACKTRACKING_SOLVER_H