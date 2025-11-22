#include "TestRunner.h"

#include "../graph_representation/GraphLoader.h"
#include "../algorithms/backtracking/BacktrackingSolver.h"
#include "../algorithms/dsatur/DSATURSolver.h"
#include "../algorithms/constraint_programming/CPSolver.h"
#include "../algorithms/simulated_annealing/SimulatedAnnealingSolver.h"
#include "../algorithms/rlf/RLFSolver.h"

#include <chrono>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace std::filesystem;

// Alias cho chrono
using Clock = chrono::high_resolution_clock;
using DurationMs = chrono::duration<double, milli>;

// -------------------- Chạy 1 thuật toán trên 1 đồ thị --------------------
TestResult runAlgorithmOnGraph(const string& graphPath, const string& algorithmName) {
    Graph g = GraphLoader::loadGraphFromFile(graphPath);
    vector<int> colors(g.getNumVertices());
    TestResult result;
    result.algorithmName = algorithmName;
    result.graphFile = graphPath;
    result.success = true;

    auto start = Clock::now();

    try {
        if (algorithmName == "Backtracking") {
            Algorithms::Backtracking::BacktrackingSolver solver;
            solver.solve(g, colors);
            solver.printColoring(colors);
            result.numColors = *max_element(colors.begin(), colors.end()) + 1;
        } else if (algorithmName == "DSATUR") {
            Algorithms::DSATUR::DSATURSolver solver;
            solver.solve(g, colors);
            solver.printColoring(colors);  // Nếu bạn thêm hàm printColoring vào DSATUR
            result.numColors = *max_element(colors.begin(), colors.end()) + 1;
        } else if (algorithmName == "RLF") {
            Algorithms::RLF::RLFSolver solver;
            solver.solve(g, colors);
            solver.printColoring(colors);  // Nếu có
            result.numColors = *max_element(colors.begin(), colors.end()) + 1;
        } else if (algorithmName == "SimulatedAnnealing") {
            Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver solver;
            solver.solve(g, colors);
            solver.printColoring(colors);  // Nếu có
            result.numColors = *max_element(colors.begin(), colors.end()) + 1;
        } else if (algorithmName == "CP") {
            Algorithms::CP::CPSolver solver;
            solver.solve(g, colors);
            solver.printColoring(colors);  // Nếu có
            result.numColors = *max_element(colors.begin(), colors.end()) + 1;
        } else {
            throw runtime_error("Tên thuật toán không hợp lệ: " + algorithmName);
        }
    } catch (const exception& e) {
        result.success = false;
        result.numColors = -1;
        cerr << "Lỗi khi chạy " << algorithmName << " trên " << graphPath 
             << ": " << e.what() << endl;
    } catch (...) {
        result.success = false;
        result.numColors = -1;
        cerr << "Lỗi không xác định khi chạy " << algorithmName << " trên " << graphPath << endl;
    }

    auto end = Clock::now();
    result.timeMs = DurationMs(end - start).count();
    return result;
}

// -------------------- Chạy tất cả thuật toán trên toàn bộ dataset --------------------
void runAllAlgorithmsOnDataset(const vector<string>& algorithms, const string& datasetFolder) {
    vector<TestResult> allResults;

    if (!exists(datasetFolder)) {
        cerr << "Thư mục dataset không tồn tại: " << datasetFolder << endl;
        return;
    }

    for (const auto& entry : recursive_directory_iterator(datasetFolder)) {
        if (entry.is_regular_file()) {
            string graphFile = entry.path().string();
            
            if (graphFile.find(".col") != string::npos || 
                graphFile.find(".txt") != string::npos) {
                
                cout << "\n=== Xử lý file: " << graphFile << " ===" << endl;
                
                for (const auto& algo : algorithms) {
                    TestResult res = runAlgorithmOnGraph(graphFile, algo);
                    allResults.push_back(res);
                    cout << "✓ " << algo << " -> Số màu: " << res.numColors 
                         << ", Thời gian: " << res.timeMs << " ms"
                         << (res.success ? "" : " (THẤT BẠI)") << endl;
                }
            }
        }
    }

    // Lưu kết quả vào CSV
    ofstream csv("results.csv");
    if (!csv.is_open()) {
        cerr << "Không thể mở file results.csv để ghi" << endl;
        return;
    }
    
    csv << "Algorithm,GraphFile,NumColors,TimeMs,Success\n";
    for (auto& r : allResults) {
        csv << r.algorithmName << "," 
            << "\"" << r.graphFile << "\"" << ","
            << r.numColors << "," 
            << r.timeMs << "," 
            << (r.success ? "true" : "false") << "\n";
    }
    csv.close();
    cout << "\n✓ Đã lưu kết quả vào results.csv (" << allResults.size() << " kết quả)" << endl;
}
