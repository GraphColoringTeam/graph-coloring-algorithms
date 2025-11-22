#include "DSATURSolver.h"
#include "../../utils/utils.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

namespace Algorithms {
namespace DSATUR {

DSATURSolver::DSATURSolver() {}

int DSATURSolver::solve(const Graph& graph, vector<int>& colors) {
    int numVertices = graph.getNumVertices();
    if (numVertices == 0) {
        colors.clear();
        return 0;
    }

    colors.assign(numVertices, -1);
    vector<bool> colored(numVertices, false);
    vector<int> saturation(numVertices, 0); // Cache bậc bão hòa
    vector<vector<bool>> usedColors(numVertices); // Màu đã dùng của neighbors
    
    // Khởi tạo usedColors
    for (int i = 0; i < numVertices; ++i) {
        usedColors[i].resize(numVertices, false);
    }

    int maxColor = -1;
    int numColored = 0;

    while (numColored < numVertices) {
        int bestVertex = -1;
        int maxSat = -1;
        int maxDeg = -1;

        // Tìm đỉnh tốt nhất
        for (int i = 0; i < numVertices; ++i) {
            if (!colored[i]) {
                if (saturation[i] > maxSat || 
                   (saturation[i] == maxSat && graph.getDegree(i) > maxDeg)) {
                    maxSat = saturation[i];
                    maxDeg = graph.getDegree(i);
                    bestVertex = i;
                }
            }
        }

        if (bestVertex == -1) break;

        // Tìm màu nhỏ nhất
        int color = 0;
        while (usedColors[bestVertex][color]) {
            color++;
        }

        // Tô màu
        colors[bestVertex] = color;
        colored[bestVertex] = true;
        numColored++;
        maxColor = max(maxColor, color);

        // Cập nhật saturation cho neighbors
        for (int neighbor : graph.getNeighbors(bestVertex)) {
            if (!usedColors[neighbor][color]) {
                usedColors[neighbor][color] = true;
                saturation[neighbor]++;
            }
        }
    }

    return maxColor + 1;
}

// 🔹 Định nghĩa hàm printColoring
void DSATURSolver::printColoring(const vector<int>& colors) const {
    cout << "Coloring result:\n";
    for (size_t i = 0; i < colors.size(); i++) {
        cout << "Vertex " << i << " -> Color " << colors[i] << "\n";
    }
}

} // namespace DSATUR
} // namespace Algorithms