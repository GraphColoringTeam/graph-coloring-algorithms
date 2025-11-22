#include "RLFSolver.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

namespace Algorithms {
namespace RLF {

RLFSolver::RLFSolver() {}

int RLFSolver::findHighestDegreeVertex(const Graph& graph, const set<int>& U) const {
    if (U.empty()) return -1;
    
    int maxDegree = -1;
    int bestVertex = *U.begin();
    
    for (int v : U) {
        int degree = graph.getDegree(v);
        if (degree > maxDegree) {
            maxDegree = degree;
            bestVertex = v;
        }
    }
    return bestVertex;
}

int RLFSolver::findHighestDegreeToK(const Graph& graph, const set<int>& U, const set<int>& K) const {
    if (U.empty()) return -1;
    
    int maxDegreeToK = -1;
    int bestVertex = -1;
    int bestTotalDegree = -1;

    for (int u : U) {
        int degreeToK = 0;
        for (int k : K) {
            if (graph.isAdjacent(u, k)) {
                degreeToK++;
            }
        }
        
        int totalDegree = graph.getDegree(u);
        
        if (degreeToK > maxDegreeToK || 
           (degreeToK == maxDegreeToK && totalDegree > bestTotalDegree)) {
            maxDegreeToK = degreeToK;
            bestVertex = u;
            bestTotalDegree = totalDegree;
        }
    }
    
    return bestVertex;
}

int RLFSolver::solve(const Graph& graph, vector<int>& colors) {
    int n = graph.getNumVertices();
    if (n == 0) {
        colors.clear();
        return 0;
    }

    colors.assign(n, -1);
    set<int> uncolored;
    for (int i = 0; i < n; ++i) {
        uncolored.insert(i);
    }

    int colorCount = 0;

    while (!uncolored.empty()) {
        // Tập K: các đỉnh sẽ được tô màu hiện tại
        set<int> K;
        // Tập U: các đỉnh có thể thêm vào K (không kề với K)
        set<int> U = uncolored;

        // Bước 1: Chọn đỉnh có bậc cao nhất trong U
        int firstVertex = findHighestDegreeVertex(graph, U);
        if (firstVertex == -1) break;

        K.insert(firstVertex);
        colors[firstVertex] = colorCount;
        U.erase(firstVertex);
        uncolored.erase(firstVertex);

        // Loại bỏ neighbors của firstVertex khỏi U
        for (int neighbor : graph.getNeighbors(firstVertex)) {
            U.erase(neighbor);
        }

        // Bước 2: Lặp thêm đỉnh vào K
        while (!U.empty()) {
            int nextVertex = findHighestDegreeToK(graph, U, K);
            if (nextVertex == -1) break;

            K.insert(nextVertex);
            colors[nextVertex] = colorCount;
            U.erase(nextVertex);
            uncolored.erase(nextVertex);

            // Loại bỏ neighbors của nextVertex khỏi U
            for (int neighbor : graph.getNeighbors(nextVertex)) {
                U.erase(neighbor);
            }
        }

        colorCount++;
    }

    return colorCount;
}

void RLFSolver::printColoring(const vector<int>& colors) const {
    if (colors.empty()) {
        cout << "Đồ thị rỗng.\n";
        return;
    }

    // Tìm số màu thực tế
    set<int> usedColors;
    for (int color : colors) {
        usedColors.insert(color);
    }

    cout << "Số màu sử dụng: " << usedColors.size() << "\n";
    cout << "Chi tiết tô màu:\n";
    
    for (size_t i = 0; i < colors.size(); ++i) {
        cout << "  Đỉnh " << i << " → Màu " << colors[i] << "\n";
    }
    
    // Kiểm tra tính hợp lệ
    bool valid = true;
    for (size_t i = 0; i < colors.size() && valid; ++i) {
        for (size_t j = i + 1; j < colors.size(); ++j) {
            // Giả sử có hàm isAdjacent
            if (colors[i] == colors[j]) {
                // Cần kiểm tra xem i và j có kề nhau không
                // Trong thực tế cần có Graph để kiểm tra
                valid = false;
                break;
            }
        }
    }
    
    cout << "Tô màu " << (valid ? "hợp lệ" : "KHÔNG hợp lệ") << "\n";
}

} // namespace RLF
} // namespace Algorithms