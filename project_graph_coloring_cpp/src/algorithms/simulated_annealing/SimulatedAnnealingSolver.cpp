#include "SimulatedAnnealingSolver.h"
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set> // Đảm bảo include set để dùng std::set
#include "../dsatur/DSATURSolver.h" // Thêm include DSATUR

using namespace std;

namespace Algorithms {
namespace SimulatedAnnealing {

// =====================
// Constructor
SimulatedAnnealingSolver::SimulatedAnnealingSolver(double initialTemp, double coolingR, int iterations)
    : initialTemperature(initialTemp), 
      coolingRate(coolingR), 
      numIterations(iterations),
      sa_rng(random_device{}()) {  // Khởi tạo sa_rng trong initializer list
}

// =====================
// Energy - Sửa đổi quan trọng ở đây!
// 1. Kiểu trả về thành long long
// 2. Hàm năng lượng kết hợp cả conflicts và số màu
long long SimulatedAnnealingSolver::calculateEnergy(const Graph& graph, const vector<int>& colors) const {
    int conflicts = 0;
    int n = graph.getNumVertices();
    for (int i = 0; i < n; ++i) {
        for (int neighbor : graph.getNeighbors(i)) {
            if (i < neighbor && colors[i] == colors[neighbor]) {
                conflicts++;
            }
        }
    }

    // Đếm số màu duy nhất
    std::set<int> uniqueColors;
    for (int color : colors) {
        uniqueColors.insert(color);
    }
    int numUsedColors = uniqueColors.size();

    // SỬA QUAN TRỌNG: Đơn giản hóa energy function
    // Nếu có conflicts → energy rất lớn, nếu không → chỉ tính số màu
    if (conflicts > 0) {
        return 1000000 + conflicts;  // Ưu tiên fix conflicts trước
    } else {
        return numUsedColors;  // Sau đó mới tối ưu số màu
    }
}

// =====================
// Kiểm tra hợp lệ (Đã cập nhật để dùng long long energy)
bool SimulatedAnnealingSolver::isValidColoring(const Graph& graph, const vector<int>& colors) const {
    return countConflicts(graph, colors) == 0; // Vẫn dùng countConflicts để kiểm tra valid
}

// Hàm này đếm số xung đột, không phải energy. Vẫn giữ nguyên.
int SimulatedAnnealingSolver::countConflicts(const Graph& graph, const vector<int>& colors) const {
    int n = graph.getNumVertices();
    int conflicts = 0;
    for (int i = 0; i < n; ++i) {
        for (int neighbor : graph.getNeighbors(i)) {
            if (i < neighbor && colors[i] == colors[neighbor]) {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// =====================
// Solve cố định k (Bạn có thể bỏ qua hàm này nếu chỉ dùng solveAdaptive)
// Đã thay đổi currentEnergy, bestEnergy, neighborEnergy sang long long.
// Đã loại bỏ break khi bestEnergy == 0, để nó vẫn tiếp tục tối ưu số màu.
int SimulatedAnnealingSolver::solve(const Graph& graph, vector<int>& colors, int k) {
    int n = graph.getNumVertices();
    if (n == 0) { colors.clear(); return 0; }
    if (k <= 0) k = n;

    colors.resize(n);
    uniform_int_distribution<int> color_dist(0, k - 1);
    for (int i = 0; i < n; ++i) colors[i] = color_dist(sa_rng);

    vector<int> current = colors;
    long long currentEnergy = calculateEnergy(graph, current); // Dùng long long
    vector<int> best = current;
    long long bestEnergy = currentEnergy; // Dùng long long

    double temperature = initialTemperature;
    uniform_int_distribution<int> vertex_dist(0, n - 1);
    uniform_real_distribution<double> prob_dist(0.0, 1.0);

    for (int iter = 0; iter < numIterations && temperature > 1e-8; ++iter) {
        // KHÔNG DỪNG SỚM! Cần tiếp tục tối ưu số màu.
        // if (bestEnergy == 0) break; 

        vector<int> neighbor = current;
        int v = vertex_dist(sa_rng);
        int oldColor = neighbor[v], newColor;
        do { newColor = color_dist(sa_rng); } while (newColor == oldColor);
        neighbor[v] = newColor;

        long long neighborEnergy = calculateEnergy(graph, neighbor); // Dùng long long

        // Quyết định chấp nhận. Sử dụng bestEnergy trong exp.
        if (neighborEnergy <= currentEnergy || prob_dist(sa_rng) < exp((double)(currentEnergy - neighborEnergy) / temperature)) {
            current = neighbor;
            currentEnergy = neighborEnergy;
            if (currentEnergy < bestEnergy) { // Sửa thành currentEnergy < bestEnergy
                best = current; 
                bestEnergy = currentEnergy; 
            }
        }
        temperature *= coolingRate;
    }

    colors = best;

    // Đếm số màu thực tế đã sử dụng trong solution tốt nhất
    std::set<int> used;
    for (int color : colors) {
        used.insert(color);
    }
    return used.size();
}

// =====================
// Adaptive SA (Đây là hàm chính chúng ta sẽ sử dụng và cần sửa đổi nhiều nhất)
// Đã cập nhật tham số mặc định và logic
int SimulatedAnnealingSolver::solveAdaptive(const Graph& graph, vector<int>& colors, int maxIterations, double initialTemp, double cooling) {
    int n = graph.getNumVertices();
    if (n == 0) { colors.clear(); return 0; }

    // Khởi tạo colors ban đầu với số màu tối đa là n (số đỉnh)
    // để đảm bảo luôn có thể tìm một tô màu hợp lệ
    colors.resize(n);
    // Tính số màu khởi đầu hợp lý
    int maxDegree = 0;
    for (int i = 0; i < n; ++i) {
        maxDegree = max(maxDegree, graph.getDegree(i));
    }
    int initialColors = max(1, maxDegree + 1); // Đảm bảo ít nhất 1 màu
    
    uniform_int_distribution<int> color_dist(0, initialColors - 1);

    cout << "🎯 SA Adaptive: " << n << " vertices, " << graph.getNumEdges() << " edges, "
     << "max degree " << maxDegree << ", starting with " << initialColors << " colors" << endl;
    
    for (int i = 0; i < n; ++i) colors[i] = color_dist(sa_rng);

    vector<int> current = colors;
    long long currentEnergy = calculateEnergy(graph, current); // Dùng long long
    
    vector<int> best = current; // Giữ solution tốt nhất
    long long bestEnergy = currentEnergy; // Năng lượng tốt nhất

    double temperature = initialTemp;
    uniform_int_distribution<int> vertex_dist(0, n - 1);
    uniform_real_distribution<double> prob_dist(0.0, 1.0);

    for (int iter = 0; iter < maxIterations && temperature > 1e-8; ++iter) {
        // KHÔNG DỪNG SỚM KHI bestEnergy == 0!
        // if (bestEnergy == 0) break; // XÓA DÒNG NÀY

        vector<int> neighbor = current;
        int v = vertex_dist(sa_rng);
        int oldColor = neighbor[v];
        int newColor;
        
        // Đổi màu đỉnh v. Đảm bảo newColor nằm trong khoảng 0 đến n-1.
        do {
            newColor = color_dist(sa_rng); 
        } while (newColor == oldColor);
        
        neighbor[v] = newColor;

        long long neighborEnergy = calculateEnergy(graph, neighbor); // Dùng long long

        // Quyết định chấp nhận
        if (neighborEnergy <= currentEnergy || 
            prob_dist(sa_rng) < exp((double)(currentEnergy - neighborEnergy) / temperature)) {
            current = neighbor;
            currentEnergy = neighborEnergy;
            
            // Cập nhật best solution nếu tìm thấy năng lượng tốt hơn
            if (currentEnergy < bestEnergy) {
                best = current;
                bestEnergy = currentEnergy;
            }
        }

        temperature *= cooling;
    }

    colors = best; // Gán solution tốt nhất tìm được cho colors
    
    // Đếm số màu thực tế từ giải pháp tốt nhất
    std::set<int> used;
    for (int color : colors) {
        used.insert(color);
    }
    
    return used.size();
}

// =====================
// Simple SA với khởi tạo từ DSATUR
int SimulatedAnnealingSolver::solveSimple(const Graph& graph, vector<int>& colors, int maxIterations) {
    int n = graph.getNumVertices();
    if (n == 0) { colors.clear(); return 0; }

    cout << "🎯 SA Simple: Starting with DSATUR baseline..." << endl;
    
    // Bước 1: Dùng DSATUR để có coloring khởi đầu tốt
    Algorithms::DSATUR::DSATURSolver dsatur;
    int dsaturColors = dsatur.solve(graph, colors);
    cout << "🎯 SA Simple: DSATUR found " << dsaturColors << " colors" << endl;

    vector<int> best = colors;
    long long bestEnergy = calculateEnergy(graph, colors);
    
    double temperature = 1000.0;
    double cooling = 0.995;
    uniform_int_distribution<int> vertex_dist(0, n - 1);
    uniform_int_distribution<int> color_dist(0, dsaturColors + 2); // Chỉ thử thêm 2 màu
    uniform_real_distribution<double> prob_dist(0.0, 1.0);

    for (int iter = 0; iter < maxIterations; ++iter) {
        vector<int> neighbor = best;
        int v = vertex_dist(sa_rng);
        int oldColor = neighbor[v];
        int newColor;
        
        do {
            newColor = color_dist(sa_rng);
        } while (newColor == oldColor);
        
        neighbor[v] = newColor;
        long long neighborEnergy = calculateEnergy(graph, neighbor);

        if (neighborEnergy < bestEnergy || 
            prob_dist(sa_rng) < exp((double)(bestEnergy - neighborEnergy) / temperature)) {
            best = neighbor;
            bestEnergy = neighborEnergy;
        }

        temperature *= cooling;
        
        // In tiến độ
        if (iter % 10000 == 0) {
            int currentColors = countColors(best);
            cout << "   SA Progress: " << iter << "/" << maxIterations 
                 << " - Colors: " << currentColors << ", Energy: " << bestEnergy << endl;
        }
    }

    colors = best;
    int finalColors = countColors(colors);
    cout << "🎯 SA Simple: Final - " << finalColors << " colors, " 
         << countConflicts(graph, colors) << " conflicts" << endl;
    
    return finalColors;
}

// Hàm đếm màu đơn giản
int SimulatedAnnealingSolver::countColors(const vector<int>& colors) const {
    std::set<int> uniqueColors;
    for (int color : colors) {
        uniqueColors.insert(color);
    }
    return uniqueColors.size();
}

// =====================
// Print result
void SimulatedAnnealingSolver::printColoring(const vector<int>& colors) const {
    cout << "Coloring result (Simulated Annealing):\n";
    for (size_t i = 0; i < colors.size(); i++)
        cout << "Vertex " << i << " -> Color " << colors[i] << "\n";
}

} // namespace SimulatedAnnealing
} // namespace Algorithms