#ifndef SIMULATED_ANNEALING_SOLVER_H
#define SIMULATED_ANNEALING_SOLVER_H

#include "../../graph_representation/Graph.h"
#include <vector>
#include <random> // Dòng này có thể không cần ở .h nếu sa_rng được khởi tạo ở .cpp
#include <set>

using namespace std;
namespace Algorithms {
namespace SimulatedAnnealing {

class SimulatedAnnealingSolver {
private:
    double initialTemperature; // Tham số này có thể không cần thiết nếu luôn dùng của hàm solveAdaptive
    double coolingRate;        // Tham số này có thể không cần thiết nếu luôn dùng của hàm solveAdaptive
    int numIterations;         // Tham số này có thể không cần thiết nếu luôn dùng của hàm solveAdaptive
    mt19937 sa_rng;

    // 1. Thay đổi kiểu trả về của calculateEnergy thành long long
    long long calculateEnergy(const Graph& graph, const std::vector<int>& colors) const;

public:
    SimulatedAnnealingSolver(double initialTemp = 1000.0, double coolingR = 0.99, int iterations = 50000); // Cập nhật mặc định iterations ở constructor

    bool isValidColoring(const Graph& graph, const std::vector<int>& colors) const;
    int countConflicts(const Graph& graph, const std::vector<int>& colors) const;

    // Hàm solve (ít dùng khi đã có solveAdaptive để tối ưu màu)
    int solve(const Graph& graph, std::vector<int>& colors, int k = -1);

    // 2. Cập nhật tham số mặc định của solveAdaptive
    int solveAdaptive(const Graph& graph, std::vector<int>& colors, 
                      int maxIterations = 50000, // Tăng lên 50000 (hoặc 100000)
                      double initialTemp = 1000.0, 
                      double cooling = 0.99);     // Tăng cooling rate lên 0.99

    void printColoring(const std::vector<int>& colors) const;
    int solveSimple(const Graph& graph, std::vector<int>& colors, int maxIterations = 50000);
    int countColors(const std::vector<int>& colors) const;
};

} // namespace SimulatedAnnealing
} // namespace Algorithms

#endif // SIMULATED_ANNEALING_SOLVER_H