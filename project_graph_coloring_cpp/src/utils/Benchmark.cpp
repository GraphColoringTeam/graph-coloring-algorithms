// src/utils/Benchmark.cpp
#include "Benchmark.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <cmath>
#include <memory>
#include <algorithm> // max_element
#include <sys/resource.h>

using namespace std;

// Include all algorithm headers
#include "../algorithms/backtracking/BacktrackingSolver.h"
#include "../algorithms/dsatur/DSATURSolver.h"
#include "../algorithms/simulated_annealing/SimulatedAnnealingSolver.h"
#include "../algorithms/constraint_programming/CPSolver.h"
#include "../algorithms/rlf/RLFSolver.h"

// =====================
// BenchmarkResult Constructor - THÊM ĐOẠN NÀY
// =====================

BenchmarkResult::BenchmarkResult() : 
    vertices(0), 
    edges(0), 
    colors_used(0), 
    runtime_ms(0.0), 
    is_valid(false), 
    iterations(0), 
    solution_quality(""), 
    stability_std_dev(0.0), 
    memory_usage_kb(0) {
}

// =====================
// Memory Usage Tracking
// =====================

// Hàm lấy memory usage hiện tại (KB)
size_t getMemoryUsageKB() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss; // Returns KB on Linux systems
    }
    return 0; // Return 0 if failed
}

BenchmarkResult Benchmark::runTest(const string& algorithm_name, 
                                   Graph& graph, 
                                   const string& graph_file) {
    BenchmarkResult result;
    result.algorithm_name = algorithm_name;
    result.graph_name = graph_file;
    result.vertices = graph.getNumVertices(); // sửa: lấy số lượng đỉnh
    result.edges = graph.getNumEdges();       // sửa: lấy số lượng cạnh
    result.iterations = 0;           // THÊM: Khởi tạo
    result.solution_quality = "";    // THÊM: Khởi tạo
    result.stability_std_dev = 0.0;  // THÊM: Khởi tạo
    
    // THÊM: Đo memory trước khi chạy thuật toán
    size_t memory_before = getMemoryUsageKB();
    
    auto start = chrono::high_resolution_clock::now();
    
    // Gọi thuật toán tương ứng
    vector<int> coloring;
    if (algorithm_name == "Backtracking") {
        Algorithms::Backtracking::BacktrackingSolver solver;
        solver.solve(graph, coloring);  // nhớ dùng tham số coloring
        } else if (algorithm_name == "DSATUR") {
            Algorithms::DSATUR::DSATURSolver solver;
            solver.solve(graph, coloring);
        } else if (algorithm_name == "SimulatedAnnealing") {
            Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver solver;
            // TRUYỀN THAM SỐ RÕ RÀNG
            solver.solveSimple(graph, coloring, 50000);
        } else if (algorithm_name == "ConstraintProgramming") {
            Algorithms::CP::CPSolver solver;
            solver.solve(graph, coloring);
        } else if (algorithm_name == "RLF") {
            Algorithms::RLF::RLFSolver solver;
            solver.solve(graph, coloring);
            } else {
                throw runtime_error("Tên thuật toán không hợp lệ: " + algorithm_name);
            }
            
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    // THÊM: Đo memory sau khi chạy và tính memory usage
    size_t memory_after = getMemoryUsageKB();
    result.memory_usage_kb = (memory_after > memory_before) ? (memory_after - memory_before) : 0;
    
    result.runtime_ms = duration.count() / 1000.0; // convert to milliseconds

    if (!coloring.empty())
        result.colors_used = *max_element(coloring.begin(), coloring.end()) + 1;
    else
        result.colors_used = 0;

    result.is_valid = validateColoring(graph, coloring);
    
    return result;
}

bool Benchmark::validateColoring(const Graph& graph, const vector<int>& coloring) {
    int n = graph.getNumVertices(); // số lượng đỉnh
    for (int u = 0; u < n; u++) {
        const auto& neighbors = graph.getNeighbors(u); // lấy danh sách kề
        for (int v : neighbors) {
            if (coloring[u] == coloring[v]) return false; // hai đỉnh kề cùng màu
        }
    }
    return true;
}

void Benchmark::exportToCSV(const vector<BenchmarkResult>& results, 
                            const string& filename) {
    ofstream file(filename);
    // SỬA HEADER: thêm các fields mới
    file << "Algorithm,Graph,Vertices,Edges,Colors,Time(ms),Valid,Iterations,SolutionQuality,StabilityStdDev,MemoryUsageKB\n";
    
    for (const auto& result : results) {
        file << result.algorithm_name << ","
             << result.graph_name << ","
             << result.vertices << ","
             << result.edges << ","
             << result.colors_used << ","
             << result.runtime_ms << ","
             << (result.is_valid ? "Yes" : "No") << ","
             << result.iterations << ","
             << result.solution_quality << ","
             << result.stability_std_dev << ","
             << result.memory_usage_kb << "\n";
    }
    file.close();
    cout << "Exported results to: " << filename << endl;
}

double Benchmark::calculateStandardDeviation(const vector<double>& values) {
    if (values.empty()) return 0.0;
    double mean = accumulate(values.begin(), values.end(), 0.0) / values.size();
    double variance = 0.0;
    for (double value : values) {
        variance += (value - mean) * (value - mean);
    }
    return sqrt(variance / values.size());
}