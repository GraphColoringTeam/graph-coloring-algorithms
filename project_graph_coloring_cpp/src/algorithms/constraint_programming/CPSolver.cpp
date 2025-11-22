#include "CPSolver.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace Algorithms {
namespace CP {

int CPSolver::solve(const Graph& graph, vector<int>& coloring) {
    int n = graph.getNumVertices();
    if (n == 0) return 0;

    coloring.assign(n, -1);

    // Khởi tạo domain mỗi node = {0,1,2,...,n-1}
    domains.assign(n, vector<int>());
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < n; c++) {
            domains[i].push_back(c);
        }
    }

    if (solveRecursive(graph, coloring, 0)) {
        // Tìm số màu tối đa thực sự dùng
        int maxColor = 0;
        for (int c : coloring) {
            maxColor = max(maxColor, c);
        }
        return maxColor + 1;
    }

    return -1; // không tô được
}

bool CPSolver::solveRecursive(const Graph& graph, vector<int>& coloring, int node) {
    if (node == graph.getNumVertices()) return true;

    // Duyệt domain
    for (int color : domains[node]) {
        if (forwardCheck(graph, node, color, coloring)) {
            coloring[node] = color;

            if (solveRecursive(graph, coloring, node + 1))
                return true;

            coloring[node] = -1; // backtrack
        }
    }
    return false;
}

bool CPSolver::forwardCheck(const Graph& graph, int node, int color, vector<int>& coloring) {
    // Kiểm tra neighbor đã tô cùng màu → fail
    for (int nei : graph.getNeighbors(node)) {
        if (coloring[nei] == color) return false;
    }
    return true;
}

// Định nghĩa hàm printColoring
void CPSolver::printColoring(const vector<int>& colors) const {
    cout << "Coloring result (Constraint Programming):\n";
    for (size_t i = 0; i < colors.size(); i++) {
        cout << "Vertex " << i << " -> Color " << colors[i] << "\n";
    }
}

} // namespace CP
} // namespace Algorithms