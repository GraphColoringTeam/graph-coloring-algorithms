#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace Algorithms {
namespace Backtracking {

BacktrackingSolver::BacktrackingSolver() : minColors(numeric_limits<int>::max()) {}

void BacktrackingSolver::solveRecursive(const Graph& graph, int k, int numVertices,
                                      vector<int>& colors, int vertexIdx) {
    // Nếu đã tìm thấy giải pháp tốt hơn, dừng nhánh này
    if (minColors <= k) return;
    
    if (vertexIdx == numVertices) {
        minColors = k;
        bestColors = colors;
        return;
    }

    // Tìm các màu có thể dùng cho đỉnh hiện tại
    vector<bool> available(k, true);
    for (int neighbor : graph.getNeighbors(vertexIdx)) {
        if (colors[neighbor] != -1) {
            available[colors[neighbor]] = false;
        }
    }

    // Thử các màu có sẵn
    for (int c = 0; c < k; ++c) {
        if (available[c]) {
            colors[vertexIdx] = c;
            solveRecursive(graph, k, numVertices, colors, vertexIdx + 1);
            colors[vertexIdx] = -1;
            
            // Nếu đã tìm thấy giải pháp, dừng sớm
            if (minColors < k) return;
        }
    }
}

int BacktrackingSolver::solve(const Graph& graph, vector<int>& colors) {
    minColors = numeric_limits<int>::max();
    bestColors.clear();

    int numVertices = graph.getNumVertices();
    if (numVertices == 0) {
        colors.clear();
        return 0;
    }

    colors.assign(numVertices, -1);
    bestColors.assign(numVertices, -1);

    // Tìm từ số màu nhỏ đến lớn để dừng sớm
    for (int k = 1; k <= numVertices; ++k) {
        solveRecursive(graph, k, numVertices, colors, 0);
        if (minColors != numeric_limits<int>::max()) {
            colors = bestColors;
            return minColors;
        }
    }

    return -1; // Không thể tô màu
}

// Định nghĩa hàm printColoring
void BacktrackingSolver::printColoring(const vector<int>& colors) const {
    cout << "Coloring result (Backtracking):\n";
    for (size_t i = 0; i < colors.size(); i++) {
        cout << "Vertex " << i << " -> Color " << colors[i] << "\n";
    }
}

} // namespace Backtracking
} // namespace Algorithms